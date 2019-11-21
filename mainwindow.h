#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

namespace Ui {
class MainWindow;
}

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

struct MyTraits : public OpenMesh::DefaultTraits{HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);};
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>  MyMesh;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    double cot(double angle);
    float faceArea(MyMesh* _mesh, int faceID);
    float barycentricArea(MyMesh* _mesh, int vertexID);
    float barycentriqueArea(VertexHandle v, MyMesh *_mesh);
    MyMesh::Point approximationCot(MyMesh * _mesh,MyMesh::VertexHandle vh);
    QVector<MyMesh::VertexHandle> sommetOpp(MyMesh *_mesh,MyMesh::VertexHandle pointV, MyMesh::FaceHandle face1, MyMesh::FaceHandle face2);
    MyMesh::VertexHandle getSomOpp(MyMesh * _mesh,MyMesh::FaceHandle face1,
                                   MyMesh::FaceHandle face2, MyMesh::VertexHandle vh);
    double angle(MyMesh * _mesh, MyMesh::FaceHandle fh, MyMesh::VertexHandle vertex);

    /**
     * @brief getTangent
     * @param angle en degrées
     * @return tangente en radians
     */
    double getTangent(double angle);

    /**
     * @brief getEdgeIdx
     * @param _mesh
     * @param v_idx, un vertex
     * @param vi_idx, un autre vertex
     * @return id de l'edge en commun
     */
    EdgeHandle getEdgeHandle(MyMesh * _mesh, unsigned int v_idx, unsigned int vi_idx);

    FaceHandle getFaceHandle(MyMesh * _mesh, HalfedgeHandle heh_idx);

    void getHalfEdgesHandle(MyMesh *_mesh, EdgeHandle eh, HalfedgeHandle * heh);

    double somCots(MyMesh * _mesh, unsigned int v_idx, unsigned int vi_idx);
    MyMesh mesh_;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_cotangent_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
