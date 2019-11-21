#include "mainwindow.h"
#include "ui_mainwindow.h"
#define PI 3.14159265
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::cot(double angle)
{
    return 1/tan(angle);
}

MyMesh::Point MainWindow::approximationCot(MyMesh *_mesh,MyMesh::VertexHandle vh)
{
    MyMesh::Point sum(0.0,0.0,0.0);

        //float a = barycentriqueArea(vh,_mesh);

        QVector<MyMesh::FaceHandle> faces;
        QVector<MyMesh::VertexHandle> vertexs;
        for(MyMesh::VertexFaceIter vf = _mesh->vf_begin(vh); vf.is_valid(); vf++)
        {
            faces.push_back(*vf);
        }
        for(int i = 0;i<faces.size()-1;i++)
        {
            if(faces.size()>1)
            {
                MyMesh::VertexHandle opp = getSomOpp(_mesh,faces.at(i),faces.at(i+1),vh);
                //MyMesh::VertexHandle opp = vh;
                vertexs = sommetOpp(&mesh_,vh,faces.at(i),faces.at(i+1));
                sum += (angle(_mesh,faces.at(i),vertexs.at(0)) +
                        angle(_mesh,faces.at(i),vertexs.at(1)) )*
                        (_mesh->point(vh)-_mesh->point(opp));
            }
        }
        //sum = 1/(2*a) * sum;

    return sum;
}
double MainWindow::getTangent(double angle)
{
    return 1 / tan ( angle * PI / 180.0 );
}

EdgeHandle MainWindow::getEdgeHandle(MyMesh * _mesh, unsigned int v_idx, unsigned int vi_idx)
{
    //QVector<MyMesh::EdgeHandle> vect_eh;
    for (MyMesh::VertexEdgeIter ve_it = _mesh->ve_iter(_mesh->vertex_handle(v_idx)); ve_it.is_valid(); ve_it++) {
        //vect_eh.push_back(*ve_it);
        for (MyMesh::VertexEdgeIter ve_it_to_compare = _mesh->ve_iter(_mesh->vertex_handle(vi_idx)); ve_it_to_compare.is_valid(); ve_it_to_compare++) {
            if (ve_it == ve_it_to_compare)
                return *ve_it;
        }
    }
}

FaceHandle MainWindow::getFaceHandle(MyMesh *_mesh, HalfedgeHandle heh)
{
    return _mesh->face_handle(heh);
}

void MainWindow::getHalfEdgesHandle(MyMesh *_mesh, EdgeHandle eh, HalfedgeHandle * heh)
{
    heh[0] = _mesh->halfedge_handle(eh, 0);
    heh[1] = _mesh->halfedge_handle(eh, 1);
}

// angle, trouvé opposé
double MainWindow::somCots(MyMesh * _mesh, unsigned int v_idx, unsigned int vi_idx)
{
    // trouvé les angles-vertex
    EdgeHandle eh = getEdgeHandle(_mesh, v_idx, vi_idx);

    HalfedgeHandle heh[2];
    getHalfEdgesHandle(_mesh, eh, heh);
    getFaceHandle(_mesh, heh[0]);
}

float MainWindow::faceArea(MyMesh* _mesh, int faceID)
{

    QVector<MyMesh::Point> * listPoints = new QVector<MyMesh::Point>();
    for (MyMesh::FaceVertexIter curVert = _mesh->fv_iter(_mesh->face_handle(static_cast<unsigned int>(faceID))); curVert.is_valid(); curVert ++) {
        listPoints->push_back(_mesh->point(*curVert));
    }

    float aire = ( ((listPoints->at(1) - listPoints->at(0)) % (listPoints->at(2) - listPoints->at(0))) / 2 ).norm();

    listPoints->clear();
    delete listPoints;

    return aire;
}

float MainWindow::barycentricArea(MyMesh* _mesh, int vertexID)
{
    float barycentric_area = 0.0;

    for(MyMesh::VertexFaceIter curFace = _mesh->vf_iter(_mesh->vertex_handle(static_cast<unsigned int>(vertexID))); curFace.is_valid(); curFace++)
        barycentric_area += faceArea(_mesh, curFace->idx());

    return barycentric_area/3;
}

float MainWindow::barycentriqueArea(VertexHandle v, MyMesh *_mesh)
{

    QVector <int> neigborList;
    int produitVect[3];
    float area;

    for(MyMesh::VertexFaceIter vf_iter = _mesh->vf_iter(v); vf_iter.is_valid(); vf_iter ++)
    {
        FaceHandle vf = *vf_iter;
        for(MyMesh::FaceVertexIter fv_iter = _mesh->fv_iter(vf); fv_iter.is_valid(); fv_iter ++)
        {
            VertexHandle vh2 = *fv_iter;
            if(v != vh2)
            {
                //créer les 2 vecteur de la face et déterminer 1/2 aire du quad corrspondant
                //edge of Face
                neigborList.push_back(v.idx());
                neigborList.push_back(vh2.idx());
                qDebug()<<v.idx()<<vh2.idx();
            }
            //on récupère les 2 vecteurs U et V de la faces courante

            //vectorU
            float xu = _mesh->point(_mesh->vertex_handle(0))[0]; - _mesh->point(_mesh->vertex_handle(1))[0];
            float yu = _mesh->point(_mesh->vertex_handle(0))[1] - _mesh->point(_mesh->vertex_handle(1))[1];
            float zu = _mesh->point(_mesh->vertex_handle(0))[2] - _mesh->point(_mesh->vertex_handle(1))[2];
            //vectorV
            float xv = _mesh->point(_mesh->vertex_handle(2))[0] - _mesh->point(_mesh->vertex_handle(3))[0];
            float yv = _mesh->point(_mesh->vertex_handle(2))[1] - _mesh->point(_mesh->vertex_handle(3))[1];
            float zv = _mesh->point(_mesh->vertex_handle(2))[2] - _mesh->point(_mesh->vertex_handle(3))[2];

            //produit vect U et V <=> à l'air du quad U*V
            produitVect[0] = abs(yu*zv -  zu*yv);
            produitVect[1] = abs(xu*zv - zu*xv);
            produitVect[2] = abs(xu*yv - yu*xv);

            //aire de la face courante
            area += 0.5*sqrt(produitVect[0]*produitVect[0]+produitVect[1]*produitVect[1]+produitVect[2]*produitVect[2]);

        }
    }
    return area/3.0;
}

QVector<MyMesh::VertexHandle> MainWindow::sommetOpp(MyMesh *_mesh,
                              MyMesh::VertexHandle pointV,
                              MyMesh::FaceHandle face1,
                              MyMesh::FaceHandle face2)
{
    double distMax = 0;
    QVector<MyMesh::VertexHandle> points;
    /*for (MyMesh::FaceEdgeIter fe = _mesh->fe_begin(face1);
         fe.is_valid();fe++)
    {
        MyMesh::HalfedgeHandle he = _mesh->halfedge_handle(fe);
        if(_mesh->to_vertex_handle(he) == pointV)
        {
            points.push_back(_mesh->from_vertex_handle(he));
        }
        else if(_mesh->from_vertex_handle(he) == pointV)
        {
            points.push_back(_mesh->to_vertex_handle(he));
        }

    }
    for (MyMesh::FaceEdgeIter fe = _mesh->fe_begin(face2);
         fe.is_valid();fe++)
    {
        MyMesh::HalfedgeHandle he = _mesh->halfedge_handle(fe);
        if(_mesh->to_vertex_handle(he) == pointV)
        {
            bool exist = false;
            for(int i = 0; i< points.size();i++)
            {
                if(points.at(i) == _mesh->from_vertex_handle(he))
                {
                    exist == true;
                    points.remove(i);
                }
            }
            if(!exist)
            {
                points.push_back(_mesh->from_vertex_handle(he));
            }

        }
        else if(_mesh->from_vertex_handle(he) == pointV)
        {

            for(int i = 0; i< points.size();i++)
            {
                if(points.at(i) == _mesh->to_vertex_handle(he))
                {
                    bool exist = false;
                    for(int i = 0; i< points.size();i++)
                    {
                        if(points.at(i) == _mesh->to_vertex_handle(he))
                        {
                            exist = true;
                            points.remove(i);
                        }
                    }
                    if(!exist)
                    {
                        points.push_back(_mesh->to_vertex_handle(he));
                        //exist = false;
                    }
                }
            }
        }
    }*/
    QVector<MyMesh::VertexHandle> vectf1;
    QVector<MyMesh::VertexHandle> vectf2;
    for(MyMesh::FaceVertexIter fv = _mesh->fv_iter(face1);
        fv.is_valid();fv++)
    {
        vectf1.push_back(*fv);
    }
    for(MyMesh::FaceVertexIter fv = _mesh->fv_iter(face2);
        fv.is_valid();fv++)
    {
        vectf2.push_back(*fv);
    }
    points.push_back(vectf1.at(0));
    points.push_back(vectf1.at(1));
    for(int i = 0 ; i< vectf1.size(); i++)
    {
        for(int j = 0 ; j< vectf2.size(); j++)
        {
            if(vectf1.at(i) == vectf1.at(j))
            {
                //qDebug()<<"FIND OPP";

                vectf1.remove(i);
                vectf2.remove(j);
                //break;
            }
        }
    }
    for(int i = 0 ; i< vectf1.size(); i++)
    {
        points.push_back(vectf1.at(i));

    }
    for(int j = 0 ; j< vectf2.size(); j++)
    {
        points.push_back(vectf2.at(j));
    }
    return points;
}

MyMesh::VertexHandle MainWindow::getSomOpp(MyMesh *_mesh, MyMesh::FaceHandle face1,
                                           MyMesh::FaceHandle face2, MyMesh::VertexHandle vh)
{
    QVector<MyMesh::VertexHandle> vectf1;
    QVector<MyMesh::VertexHandle> vectf2;
    MyMesh::VertexHandle opp;
    for(MyMesh::FaceVertexIter fv = _mesh->fv_iter(face1);
        fv.is_valid();fv++)
    {
        vectf1.push_back(*fv);
    }
    for(MyMesh::FaceVertexIter fv = _mesh->fv_iter(face2);
        fv.is_valid();fv++)
    {
        vectf2.push_back(*fv);
    }
    opp = vectf1.at(0);
    for(int i = 0 ; i< vectf1.size(); i++)
    {
        for(int j = 0 ; j< vectf2.size(); j++)
        {
            if(vectf1.at(i) == vectf1.at(j) && vectf1.at(i)!=vh)
            {
                //qDebug()<<"FIND OPP";
                opp = vectf1.at(i);
                return opp;
                //break;
            }
        }
    }
    return opp;
}

float MainWindow::angleEE(MyMesh* _mesh, int vertexID,  int faceID)
{

    QVector<MyMesh::Point> * listPoints = new QVector<MyMesh::Point>();
    for (MyMesh::FaceVertexIter curVert = _mesh->fv_iter(_mesh->face_handle(static_cast<unsigned int>(faceID))); curVert.is_valid(); curVert ++) {
        if (*curVert != _mesh->vertex_handle(static_cast<unsigned int>(vertexID))) {
            listPoints->push_back(_mesh->point(*curVert));
        }
    }

    MyMesh::Point originPoint = _mesh->point(_mesh->vertex_handle(static_cast<unsigned int>(vertexID)));
    //qDebug() << __FUNCTION__ << "norm :" << static_cast<double>(((listPoints->at(0) - originPoint) | (listPoints->at(1) - originPoint)).norm());

    float norm_v1 = (listPoints->at(0) - originPoint).norm();
    float norm_v2 = (listPoints->at(1) - originPoint).norm();

    float angle = static_cast<float>( acos( ((listPoints->at(0) - originPoint)/norm_v1) | ((listPoints->at(1) - originPoint)/norm_v2) ) );


    //qDebug() << __FUNCTION__ << "angle :" << angle;
    listPoints->clear();
    delete listPoints;

    return angle;
}

double MainWindow::angle(MyMesh *_mesh, MyMesh::FaceHandle fh, MyMesh::VertexHandle vertex)
{
    double angle = 0;
    QVector<MyMesh::VertexHandle> list_edge;
    for(MyMesh::FaceEdgeIter fe = _mesh->fe_begin(fh);fe.is_valid();fe++)
    {
        MyMesh::HalfedgeHandle he = _mesh->halfedge_handle(fe);
        if(_mesh->from_vertex_handle(he) == vertex)
        {
            list_edge.push_back(_mesh->to_vertex_handle(he));
        }
        if(_mesh->to_vertex_handle(he) == vertex)
        {
            list_edge.push_back(_mesh->from_vertex_handle(he));
        }
    }
    if(list_edge.size()>1)
    {
        MyMesh::Point p0 = _mesh->point(vertex);
        MyMesh::Point p1 = _mesh->point(list_edge.at(0));
        MyMesh::Point p2 = _mesh->point(list_edge.at(1));

        MyMesh::Point V1 = p1 - p0;
        MyMesh::Point V2 = p2 - p0;

        QVector<float> norme;

        float tmp = V1[0]*V1[0]+V1[1]*V1[1]+V1[2]*V1[2];
        norme.append(sqrt(tmp)); //sqrt(x^2+y^2+z^2) : C'est la norme du premier vecteur

        tmp = V2[0]*V2[0]+V2[1]*V2[1]+V2[2]*V2[2];
        norme.append(sqrt(tmp)); //C'est la norme du second vecteurs

        //normalisation des vecteur :  chacun des vecteur / par la norme

        V1[0] = V1[0]/norme[0];
        V1[1] = V1[1]/norme[0];
        V1[2] = V1[2]/norme[0];

        V2[0] = V2[0]/norme[1];
        V2[1] = V2[1]/norme[1];
        V2[2] = V2[2]/norme[1];
        float a = V1[0]*V2[0];//+3];
        float b = V1[1]*V2[1];
        float c = V1[2]*V2[2];

        float prodScal = a+b+c;
        abs(prodScal); //garantie que le produit scalaire soit positif

        angle = acos(prodScal)/**180/M_PI*/;
         //en radians
    }
    return angle;
}

void MainWindow::on_pushButton_clicked()
{
    // Cet exemple montre comment afficher un cube (sans passer par une structure de maillage) directement dans un MeshViewerWidget

    // une liste de sommets (x, y, z)
    GLfloat verts[24] = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f
    };

    // une liste de couleur (r, g, b)
    GLfloat cols[24] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f
    };

    // une liste de triangles coresspondants aux sommets de verts
    GLuint IndiceArray[36] = {
        0,1,2,    2,1,3,
        4,5,6,    6,5,7,
        3,1,5,    5,1,7,
        0,2,6,    6,2,4,
        6,7,0,    0,7,1,
        2,3,4,    4,3,5
    };

    ui->widget->loadMesh(verts, cols, 24, IndiceArray, 36);
}

void MainWindow::on_pushButton_2_clicked()
{
    // Cet exemple montre comment ouvrir un .obj, et l'afficher dans le MeshViewerWidget
    MyMesh mesh;
    OpenMesh::IO::read_mesh(mesh, "../meshFiles/bunnyLowPoly.obj");

    GLuint* IndiceArray = new GLuint[mesh.n_faces() * 3];

    MyMesh::ConstFaceIter fIt(mesh.faces_begin()), fEnd(mesh.faces_end());
    MyMesh::ConstFaceVertexIter fvIt;
    int i = 0;
    for (; fIt!=fEnd; ++fIt)
    {
        fvIt = mesh.cfv_iter(*fIt);
        IndiceArray[i] = fvIt->idx(); i++; ++fvIt;
        IndiceArray[i] = fvIt->idx(); i++; ++fvIt;
        IndiceArray[i] = fvIt->idx(); i++;
    }

    GLfloat* cols = new GLfloat[mesh.n_vertices() * 3];
    for(int c = 0; c < mesh.n_vertices() * 3; c = c + 3)
    {
        cols[c] = 0.5; cols[c+1] = 0.5; cols[c+2] = 0.5;
    }
    this->mesh_ = mesh;
    ui->widget->loadMesh((GLfloat*)mesh.points(), cols, mesh.n_vertices() * 3, IndiceArray, mesh.n_faces() * 3);
}

void MainWindow::on_pushButton_cotangent_clicked()
{
    MyMesh::Point dsum(0,0,0);
    for(MyMesh::VertexIter v = mesh_.vertices_begin();v!=mesh_.vertices_end();v++)
    {
        float a = barycentricArea(&mesh_, v->idx());
        dsum+=1/(2*a) * approximationCot(&mesh_,*v);
    }
    qDebug()<<"DELTA SUM :"<<dsum[0];
}
