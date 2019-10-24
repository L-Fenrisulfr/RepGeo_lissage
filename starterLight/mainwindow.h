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
    void approximationCot(MyMesh * _mesh);
    QVector<MyMesh::VertexHandle> sommetOpp(MyMesh *_mesh,MyMesh::VertexHandle pointV, MyMesh::FaceHandle face1, MyMesh::FaceHandle face2);
    double angle(MyMesh * _mesh, MyMesh::FaceHandle fh, MyMesh::VertexHandle vertex);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
