#include "glwidget.h"
#include <QTimer>
#include <iostream>
#include <QFileDialog>
#include <QVector3D>

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
    glEnable(GL_CULL_FACE);
    createdModel = false;
    tnw::Light l = tnw::Light(0.0,0.0,4.0);
    l.setKs(0,0,0);
    l.setKd(1,1,1);
    luzes.append(l);
    lastMouseRot[0] = 0;
    lastMouseRot[1] = 0;
    lastMouseMov[0] = 0;
    lastMouseMov[1] = 0;
    angles[0] = 0;
    angles[1] = 0;
    moveSelectedPointInDir[0] = false;
    moveSelectedPointInDir[1] = true;

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
        //this->model->getModel()->desenhar();
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Model"), "", tr("*.obj files with a *.mtl file"));
//    QVector4D p(0,0,0,1);
//    QVector4D S(1.5,0,0,0);
//    QVector4D T(0,1.5,0,0);
//    QVector4D U(0,0,1.5,0);

    createdModel = true;

    QVector4D p(2,0,2,1);
    QVector4D S(-2,0,0,0);
    QVector4D T(0,2,0,0);
    QVector4D U(0,0,-2,0);
    this->model = new GridModel(fileName, p, S, T, U, 8, 8, 8);
    //this->model = new GridModel("cube.obj", p, S, T, U, 1, 1, 1);
    std::cout << "=======vertice 1========\n";
    QList<QVector4D> l = this->model->getModel()->getVertices();
    std::cout << l[0].x() << "|" << l[0].y() << "|" << l[0].z() << "|" << l[0].w() << "\n";
    flush(cout);
}

void GLWidget::changeNumGridPoints(int ns, int nt, int nu)
{
    FFDGrid *oldGrid = this->model->getGrid();
    FFDGrid *newGrid = new FFDGrid(oldGrid->getP(), oldGrid->getS(), oldGrid->getT(), oldGrid->getU(), ns, nt, nu);
    this->model->substituteGrid(newGrid);
}

void GLWidget::resetView()
{
    float angleToRotateX;
    if (angles[0] > 0) {
        angleToRotateX = -angles[0];
    } else {
        angleToRotateX = angles[0];
    }

    tnw::TransformMatrix rX = tnw::rotacaoX(angleToRotateX);
    tnw::TransformMatrix tmiX = tnw::translacao(-model->getModel()->getPontoMedio());
    tnw::TransformMatrix tmfX = tnw::translacao(model->getModel()->getPontoMedio());
    tnw::TransformMatrix tgiX = tnw::translacao(-model->getGrid()->getMedianPoint());
    tnw::TransformMatrix tgfX = tnw::translacao(model->getGrid()->getMedianPoint());

    model->getModel()->aplicarTransformacao(tmfX*rX*tmiX);
    model->getGrid()->applyTransform(tgfX*rX*tgiX);

    float angleToRotateY;
    if (angles[0] > 0) {
        angleToRotateY = -angles[1];
    } else {
        angleToRotateY = angles[1];
    }

    tnw::TransformMatrix rY = tnw::rotacaoY(angleToRotateY);
    tnw::TransformMatrix tmiY = tnw::translacao(-model->getModel()->getPontoMedio());
    tnw::TransformMatrix tmfY = tnw::translacao(model->getModel()->getPontoMedio());
    tnw::TransformMatrix tgiY = tnw::translacao(-model->getGrid()->getMedianPoint());
    tnw::TransformMatrix tgfY = tnw::translacao(model->getGrid()->getMedianPoint());

    model->getModel()->aplicarTransformacao(tmfY*rY*tmiY);
    model->getGrid()->applyTransform(tgfY*rY*tgiY);
}

void GLWidget::showGrid(bool show)
{
    this->showGridOpt = show;
}

void GLWidget::selectGridPoint(QVector3D selectedPoint)
{
    cout << "selected point: " << selectedPoint[0] << " " << selectedPoint[1] << " " << selectedPoint[2] << "\n";
    this->model->getGrid()->setSelectedPoint(selectedPoint);
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
        float deltaX = (event->pos().x() - lastMouseRot[0]) / this->width() ;
        angles[0] += deltaX;
        //cout << "deltaX: " << deltaX << " e angles: " << angles[0] << "\n";
//        if (angles[0] >= 360) {
//            angles[0] -= 360;
//        }

        tnw::TransformMatrix rX = tnw::rotacaoX(angles[0]);
        tnw::TransformMatrix tmiX = tnw::translacao(-model->getModel()->getPontoMedio());
        tnw::TransformMatrix tmfX = tnw::translacao(model->getModel()->getPontoMedio());
        tnw::TransformMatrix tgiX = tnw::translacao(-model->getGrid()->getMedianPoint());
        tnw::TransformMatrix tgfX = tnw::translacao(model->getGrid()->getMedianPoint());

        model->getModel()->aplicarTransformacao(tmfX*rX*tmiX);
        model->getGrid()->applyTransform(tgfX*rX*tgiX);
        lastMouseRot[0] = event->pos().x();

        float deltaY = (lastMouseRot[1] - event->pos().y()) / this->height();
        angles[1] += deltaY;
        //cout << "deltaY: " << deltaY << " e angles: " << angles[1] << "\n";
//        if (angles[1] >= 360){
//            angles[1] -= 360;
//        }
        tnw::TransformMatrix rY = tnw::rotacaoY(angles[1]);
        tnw::TransformMatrix tmiY = tnw::translacao(-model->getModel()->getPontoMedio());
        tnw::TransformMatrix tmfY = tnw::translacao(model->getModel()->getPontoMedio());
        tnw::TransformMatrix tgiY = tnw::translacao(-model->getGrid()->getMedianPoint());
        tnw::TransformMatrix tgfY = tnw::translacao(model->getGrid()->getMedianPoint());

        model->getModel()->aplicarTransformacao(tmfY*rY*tmiY);
        model->getGrid()->applyTransform(tgfY*rY*tgiY);
        lastMouseRot[1] = event->pos().y();
    }
    if (lbpress){
//        cout << "left button pressed!\n";
//        flush(cout);
//        //Aqui vai o código de mover o ponto
//        float deltaDir = (lastMouseMov[0] - event->pos().x()) / this->width();
//        cout << "deltaDir: " << deltaDir << "\n";
//        flush(cout);
//        lastMouseMov[0] = event->pos().x();
//        QVector3D selPoint = this->model->getGrid()->getSelectedPoint();
//        //Se o ponto selecionado do grid é válido
//        if (selPoint.x() >= 0 && selPoint.y() >= 0 && selPoint.z() >= 0 ){
//            QVector4D pointBefore = this->model->getGrid()->getPoints().get(selPoint.x(), selPoint.y(), selPoint.z());
//            if (moveSelectedPointInDir[0]) {
//                pointBefore.setX(pointBefore.x() + deltaDir);
//            } else if (moveSelectedPointInDir[1]) {
//                pointBefore.setY(pointBefore.y() + deltaDir);
//            } else if (moveSelectedPointInDir[2]) {
//                pointBefore.setZ(pointBefore.z() + deltaDir);
//            }

//            this->model->getGrid()->getPoints().set(selPoint.x(), selPoint.y(), selPoint.z(), pointBefore);
//            this->model->deformModel();
//        }
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
//    cout << "key press: " << event->key() << "\n";
//    flush(cout);
    if (event->key() ==  Qt::Key_X) {
        //cout << "x key pressed!\n";
        moveSelectedPointInDir[0] = true;
        moveSelectedPointInDir[1] = false;
        moveSelectedPointInDir[2] = false;
    } else if (event->key() == Qt::Key_Y) {
        //cout << "y key pressed\n";
        moveSelectedPointInDir[0] = false;
        moveSelectedPointInDir[1] = true;
        moveSelectedPointInDir[2] = false;
    } else if (event->key() == Qt::Key_Z){
        //cout << "z key pressed\n";
        moveSelectedPointInDir[0] = false;
        moveSelectedPointInDir[1] = false;
        moveSelectedPointInDir[2] = true;
    } else if (event->key() == Qt::Key_Left) {
        QVector3D selPoint = this->model->getGrid()->getSelectedPoint();
        //Se o ponto selecionado do grid é válido
        if (selPoint.x() >= 0 && selPoint.y() >= 0 && selPoint.z() >= 0 ){
            QVector4D pointBefore = this->model->getGrid()->getPoints().get(selPoint.x(), selPoint.y(), selPoint.z());
            pointBefore.setX(pointBefore.x() - 0.1);
            this->model->moveGridPoint(selPoint.x(), selPoint.y(), selPoint.z(), pointBefore);
            cout << "deformed model!\n";
            flush(cout);
        }
    } else if (event->key() == Qt::Key_Right) {
        QVector3D selPoint = this->model->getGrid()->getSelectedPoint();
        if (selPoint.x() >= 0 && selPoint.y() >= 0 && selPoint.z() >= 0 ){
             QVector4D pointBefore = this->model->getGrid()->getPoints().get(selPoint.x(), selPoint.y(), selPoint.z());
             pointBefore.setX(pointBefore.x() + 0.1);
             this->model->moveGridPoint(selPoint.x(), selPoint.y(), selPoint.z(), pointBefore);
        }
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
