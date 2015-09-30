#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

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
}

void GLWidget::resizeGL(int w, int h)
{

}

void GLWidget::paintGL()
{
    qglClearColor(Qt::black);
}

