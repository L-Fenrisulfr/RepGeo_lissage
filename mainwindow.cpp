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


float MainWindow::barycentriqueArea(VertexHandle v, MyMesh *_mesh)
{

    QVector <int> neigborList;
    QVector<float> produitVect;
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
