#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QTimer>
#include "ffdgrid.h"
#include "TriMesh.h"

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

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
signals:
public slots:
    void update();
};

#endif // GLWIDGET_H
