#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::approximationCot(MyMesh *_mesh)
{

}

QVector<MyMesh::VertexHandle> MainWindow::sommetOpp(MyMesh *_mesh,
                              MyMesh::VertexHandle pointV,
                              MyMesh::FaceHandle face1,
                              MyMesh::FaceHandle face2)
{
    double distMax = 0;
    QVector<MyMesh::VertexHandle> points;
    for (MyMesh::FaceEdgeIter fe = _mesh->fe_begin(face1);
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
    }
    return points;
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
        float a = V1[0]*V2[0+3];
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

    ui->widget->loadMesh((GLfloat*)mesh.points(), cols, mesh.n_vertices() * 3, IndiceArray, mesh.n_faces() * 3);
}
