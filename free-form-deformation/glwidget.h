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
    float lastMouse[2];
    float angles[2];
    bool lbpress;
    bool rbpress;
    bool showGridOpt;
    GridModel *model;
    bool createdModel;
    QList<tnw::Light> luzes;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
signals:
    void displayPoints(std::vector<QVector4D> points);
public slots:
    void update();
    void openModel();
    void changeNumGridPoints(int ns, int nt, int nu);
    void showGrid(bool show);
};

#endif // GLWIDGET_H
