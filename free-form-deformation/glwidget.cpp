#include "glwidget.h"
#include <QTimer>
#include <iostream>
#include <QFileDialog>

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
//    QVector4D p(0,0,0,1);
//    QVector4D S(1,0,0,0);
//    QVector4D T(0,1,0,0);
//    QVector4D U(0,0,1,0);
//    grid = new FFDGrid(p,S,T,U,2,2,2);

//    //model = new Model("bun_zipper.ply", 10, 10, 10);
//    model = new tnw::Model("AstronautaCor.obj");
//    model->aplicarTransformacao(tnw::translacao(-model->getPontoMedio()));
//    model->aplicarTransformacao(tnw::escala(0.5,0.5,0.5));
    createdModel = false;
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
    if (createdModel) {
        this->model->getModel()->desenhar(amb, this->luzes);
        if (showGridOpt){
            this->model->getGrid()->draw(1);
        }
    }
}

void GLWidget::update()
{
    this->repaint();
}

void GLWidget::openModel()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Model"), "", tr("*.obj files"));
    this->model = new GridModel(fileName, 5, 5, 5);
    tnw::TransformMatrix m = tnw::escala(0.5,0.5,0.5);
    this->model->getModel()->aplicarTransformacao(m);
    this->model->getGrid()->applyTransform(m);
    displayPoints(this->model->getGrid()->getPoints().getData());
    createdModel = true;
}

void GLWidget::changeNumGridPoints(int ns, int nt, int nu)
{
    FFDGrid *oldGrid = this->model->getGrid();
    FFDGrid *newGrid = new FFDGrid(oldGrid->getP(), oldGrid->getS(), oldGrid->getT(), oldGrid->getU(), ns, nt, nu);
    this->model->substituteGrid(newGrid);
}

void GLWidget::showGrid(bool show)
{
    this->showGridOpt = show;
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
        float deltaX = (event->pos().x() - lastMouse[0]) / this->width() ;
        angles[0] += deltaX;
        if (angles[0] >= 360) {
            angles[0] -= 360;
        }

        tnw::TransformMatrix rX = tnw::rotacaoX(angles[0]);
        tnw::TransformMatrix tmiX = tnw::translacao(-model->getModel()->getPontoMedio());
        tnw::TransformMatrix tmfX = tnw::translacao(model->getModel()->getPontoMedio());
        tnw::TransformMatrix tgiX = tnw::translacao(-model->getGrid()->getMedianPoint());
        tnw::TransformMatrix tgfX = tnw::translacao(model->getGrid()->getMedianPoint());

        model->getModel()->aplicarTransformacao(tmfX*rX*tmiX);
        model->getGrid()->applyTransform(tgfX*rX*tgiX);
        lastMouse[0] = event->pos().x();

        float deltaY = (event->pos().y() - lastMouse[1]) / this->height();
        angles[1] += deltaY;
        if (angles[1] >= 360){
            angles[1] -= 360;
        }
        tnw::TransformMatrix rY = tnw::rotacaoY(angles[1]);
        tnw::TransformMatrix tmiY = tnw::translacao(-model->getModel()->getPontoMedio());
        tnw::TransformMatrix tmfY = tnw::translacao(model->getModel()->getPontoMedio());
        tnw::TransformMatrix tgiY = tnw::translacao(-model->getGrid()->getMedianPoint());
        tnw::TransformMatrix tgfY = tnw::translacao(model->getGrid()->getMedianPoint());

        model->getModel()->aplicarTransformacao(tmfY*rY*tmiY);
        model->getGrid()->applyTransform(tgfY*rY*tgiY);
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
