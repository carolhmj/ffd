#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QList>
#include <QVector4D>
#include <QString>
#include <vector>
#include "gridmodel.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
protected:
    QTimer time;
    float lastMouseRot[2];
    float lastMouseMov[2];
    float angles[2];
    bool lbpress;
    bool rbpress;
    bool showGridOpt;
    GridModel *model;
    bool createdModel;
    QList<tnw::Light> luzes;
    bool moveSelectedPointInDir[3];

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent *);
signals:
    void displayPoints(std::vector<QVector4D> points);
public slots:
    void update();
    void openModel();
    void changeNumGridPoints(int ns, int nt, int nu);
    void resetView();
    void showGrid(bool show);
    void selectGridPoint(QVector3D selectedPoint);
};

#endif // GLWIDGET_H
