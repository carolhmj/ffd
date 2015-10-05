#include "glwidget.h"
#include <QTimer>
#include <iostream>

//using namespace trimesh;
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
    QVector4D p(0,0,0,1);
    QVector4D S(1,0,0,0);
    QVector4D T(0,1,0,0);
    QVector4D U(0,0,1,0);
    grid = new FFDGrid(p,S,T,U,2,2,2);

    //model = new Model("bun_zipper.ply", 10, 10, 10);
    model = new tnw::Model("AstronautaCor.obj");
    model->aplicarTransformacao(tnw::translacao(-model->getPontoMedio()));
    model->aplicarTransformacao(tnw::escala(0.5,0.5,0.5));
    tnw::Light l = tnw::Light(1.0,0.0,0.0);
    l.setKs(0,0,0);
    l.setKd(1,1,1);
    luzes.append(l);
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

    float amb[3] = {0.2,0.2,0.2};
    model->desenhar(amb,luzes);
    grid->draw(1);
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

        tnw::TransformMatrix r = tnw::rotacaoX(angles[0]);
        tnw::TransformMatrix tmi = tnw::translacao(-model->getPontoMedio());
        tnw::TransformMatrix tmf = tnw::translacao(model->getPontoMedio());
        tnw::TransformMatrix tgi = tnw::translacao(-grid->getMedianPoint());
        tnw::TransformMatrix tgf = tnw::translacao(grid->getMedianPoint());

        model->aplicarTransformacao(tmf*r*tmi);
        grid->applyTransform(tgf*r*tgi);
        lastMouse[0] = event->pos().x();

        float delta = (event->pos().y() - lastMouse[1]) / this->height();
        angles[1] += delta;
        if (angles[1] >= 360){
            angles[1] -= 360;
        }
        tnw::TransformMatrix r = tnw::rotacaoY(angles[1]);
        tnw::TransformMatrix tmi = tnw::translacao(-model->getPontoMedio());
        tnw::TransformMatrix tmf = tnw::translacao(model->getPontoMedio());
        tnw::TransformMatrix tgi = tnw::translacao(-grid->getMedianPoint());
        tnw::TransformMatrix tgf = tnw::translacao(grid->getMedianPoint());

        model->aplicarTransformacao(tmf*r*tmi);
        grid->applyTransform(tgf*r*tgi);
        lastMouse[1] = event->pos().y();
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
