#ifndef MESHVIEWERWIDGET_H
#define MESHVIEWERWIDGET_H

#define GL_GLEXT_PROTOTYPES
#include <QDebug>
#include <QGLWidget>
#include "QMouseEvent"
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glut.h>


using namespace OpenMesh;

const double TRACKBALL_RADIUS = 0.6;

class MeshViewerWidget : public QGLWidget
{
    Q_OBJECT
public:
    MeshViewerWidget(QWidget* _parent=0);
    MeshViewerWidget(QGLFormat& _fmt, QWidget* _parent );

    // events GL
    void initializeGL();
    void resizeGL(int _w, int _h);
    void paintGL();

    // gestion de la vue et de la trackball
    void update_projection_matrix();
    void view_all();
    void set_scene_pos( const OpenMesh::Vec3f& _cog, float _radius );
    bool map_to_sphere(const QPoint& _point, OpenMesh::Vec3f& _result);
    void translate( const OpenMesh::Vec3f& _trans );
    void rotate( const OpenMesh::Vec3f& _axis, float _angle );

    void reloadPOV();
    void loadMesh(GLfloat* verts, GLfloat* colors, int nVerts, GLuint* triangles, int nTriangles);

    // buffer pour les VBO
    GLuint DataBuffers[2];
    // Nombre de triangles du mesh (pour le call du draw)
    int triToDraw;
    // pour savoir si les buffer sont init
    bool init;

    // variables de gestion de la vue et de la trackball
    OpenMesh::Vec3f  center_;
    float            radius_;

    GLdouble    projection_matrix_[16], modelview_matrix_[16];

    QPoint           last_point_2D_;
    OpenMesh::Vec3f  last_point_3D_;
    bool             last_point_ok_;
protected:

    // Qt mouse events
    virtual void mousePressEvent( QMouseEvent* );
    virtual void mouseReleaseEvent( QMouseEvent* );
    virtual void mouseMoveEvent( QMouseEvent* );
    virtual void wheelEvent( QWheelEvent* );
};

#endif // MESHVIEWERWIDGET_H