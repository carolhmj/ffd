#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include "ffdgrid.h"
#include "TriMesh.h"
#include "Vec.h"
#include "GLCamera.h"

using namespace trimesh;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
protected:
    QTimer time;
    FFDGrid* grid;
    TriMesh* model;
    GLCamera camera;
    float lastMouse[2];
    float angles[2];
    bool lbpress;
    bool rbpress;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
signals:
public slots:
    void update();
};

#endif // GLWIDGET_H
