#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QList>
#include <QVector4D>
#include "ffdgrid.h"
#include "model.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
protected:
    QTimer time;
    float lastMouse[2];
    float angles[2];
    bool lbpress;
    bool rbpress;
    tnw::Model *model;
    FFDGrid *grid;
    QList<tnw::Light> luzes;

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
