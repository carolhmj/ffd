#include "glwidget.h"
#include "Vec.h"
#include <QTimer>
#include <iostream>

using namespace trimesh;
using namespace std;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    QObject::connect(&time, SIGNAL(timeout()),this,SLOT(update()));
}

GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{
    //Por enquanto a inicialização bem básica
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    vec p(0,0,0);
    vec S(1,0,0);
    vec T(0,1,0);
    vec U(0,0,1);
    grid = new FFDGrid(p,S,T,U,2,2,2);
    lastMouse[0] = 0;
    lastMouse[1] = 0;
    angles[0] = 0;
    angles[1] = 0;
    time.start(60);
}

void GLWidget::resizeGL(int w, int h)
{
    if(h<w) {
        glViewport((w-h)/2,0,h,h);
    }
    else {
        glViewport(0,(h-w)/2,w,w);
    }
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->grid->draw(1);
}

void GLWidget::update()
{
    this->repaint();
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton){
        rbpress = true;
    }
    if (event->button() == Qt::LeftButton){
        lbpress = true;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    if (rbpress) {
        float delta = (event->pos().x() - lastMouse[0]) / this->width() ;
        angles[0] += delta;
        if (angles[0] >= 360) {
            angles[0] -= 360;
        }
        glRotatef(delta, 0,1,0);
        //lastMouse[0] = event->pos().x();
    }
    if (lbpress){
        float delta = (event->pos().y() - lastMouse[1]) / this->height();
        angles[1] += delta;
        if (angles[1] >= 360){
            angles[1] -= 360;
        }
        glRotatef(delta, 1,0,0);
        //lastMouse[1] = event->pos().y();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton){
        rbpress = false;
    }
    if (event->button() == Qt::LeftButton){
        lbpress = false;
    }
}
