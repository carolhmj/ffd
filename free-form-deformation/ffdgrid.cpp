#include "ffdgrid.h"
#include <GL/gl.h>
#include <iostream>

/*Construtor do FFDGrid. Coloca os pontos em suas posições
 * iniciais de acordo com a fórmula
 */
using namespace std;

int FFDGrid::getNs() const
{
    return ns;
}

int FFDGrid::getNt() const
{
    return nt;
}

int FFDGrid::getNu() const
{
    return nu;
}

GridPoints FFDGrid::getPoints() const
{
    return points;
}

QVector4D FFDGrid::getP() const
{
    return p;
}

QVector4D FFDGrid::getS() const
{
    return S;
}

QVector4D FFDGrid::getT() const
{
    return T;
}

QVector4D FFDGrid::getU() const
{
    return U;
}

QVector3D FFDGrid::getSelectedPoint() const
{
    return selectedPoint;
}

void FFDGrid::setSelectedPoint(const QVector3D &value)
{
    selectedPoint = value;
}

void FFDGrid::moveGridPoint(int i, int j, int k, QVector4D p)
{
    this->points(i,j,k) = p;
}

QString FFDGrid::printPointsInString()
{
    QString ret;

    ret.push_back("num of points: ");
    ret.setNum((this->ns+1)*(this->nt+1)*(this->nu+1));
    ret.push_back("\n");
    for (int i = 0; i <= this->ns; i++){
        for (int j = 0; j <= this->nt; j++){
            for (int k = 0; k < this->nu; k++){
                ret.setNum(this->points(i,j,k).x());
                ret.push_back(" ");
                ret.setNum(this->points(i,j,k).y());
                ret.push_back(" ");
                ret.setNum(this->points(i,j,k).z());
                ret.push_back("\n");
            }
        }
    }
    return ret;
}
FFDGrid::FFDGrid(QVector4D p, QVector4D S, QVector4D T, QVector4D U, int ns, int nt, int nu) :
    /* Como nós desejamos na verdade ter os pontos de 0 a ns, a nt e a nu,
     * o número de pontos armazenados é 1 a mais.
     */
    points(ns+1, nt+1, nu+1)
{
    this->p = p;
    this->S = S;
    this->T = T;
    this->U = U;

    this->ns = ns;
    this->nt = nt;
    this->nu = nu;

    this->selectedPoint.setX(-1);
    this->selectedPoint.setY(-1);
    this->selectedPoint.setZ(-1);

    cout << "======pontos do grid=======\n";
    //Inicialização dos pontos
    for (int i = 0; i <= ns; i++){
        for (int j = 0; j <= nt; j++){
            for (int k = 0; k <= nu; k++){
                QVector4D v = p + float(i)/float(ns) * S + float(j)/float(nt) * T + float(k)/float(nu) * U;
                cout << v.x() << " " << v.y() << " " << v.z() << "\n";
                this->points(i,j,k) = v;
            }
        }
    }
}

FFDGrid::~FFDGrid()
{

}

//Ponto médio do grid é o ponto na metade de todos os vetores
QVector4D FFDGrid::getMedianPoint()
{
    return p + 0.5 * S + 0.5 * T + 0.5 * U;
}

void FFDGrid::draw(int mode)
{
    switch (mode) {
    case WIRE:
        //cout << "Drawing lines\n";
        //flush(cout);
        glTranslatef(-0.5,-0.5,-0.5);
        glColor3f(1,0,0);
        glBegin(GL_LINES);
            for (int i=0; i <= ns; i++){
                for (int j=0; j <= nt; j++){
                    for (int k=0; k <= nu; k++){
                        //Ponto
                        QVector4D v1 = this->points(i,j,k);
                        //Vizinho da frente - Não existe quando k é max
                        if (k < nu){
                            QVector4D v2 = this->points(i,j,k+1);
                            glVertex3d(v1[0],v1[1],v1[2]);
                            glVertex3d(v2[0],v2[1],v2[2]);
                        }

                        //Vizinho de cima - Não existe quando j é max
                        if (j < nt){
                           QVector4D v3 = this->points(i,j+1,k);
                           glVertex3d(v1[0],v1[1],v1[2]);
                           glVertex3d(v3[0],v3[1],v3[2]);
                        }

                        //Vizinho da esquerda - Não existe quando i é max
                        if (i < ns){
                            QVector4D v4 = this->points(i+1,j,k);
                            glVertex3d(v1[0],v1[1],v1[2]);
                            glVertex3d(v4[0],v4[1],v4[2]);
                        }
                    }
                }
            }
        glEnd();
        glTranslatef(0.5,0.5,0.5);
    case POINTS_ONLY:
        //std::cout << "Drawing points\n";
        //flush(std::cout);
        glTranslatef(-0.5,-0.5,-0.5);
        glColor3f(1,1,1);
        glEnable(GL_POINT_SMOOTH);
        glPointSize(4);
        glBegin(GL_POINTS);
            for (int i=0; i <= ns; i++){
                for (int j=0; j <= nt; j++){
                    for (int k=0; k <= nu; k++){
                        QVector4D v = this->points(i,j,k);
                        if (QVector3D(i,j,k) == this->selectedPoint){
                            glColor3f(0,1,0);
                            glPointSize(8);
                            glVertex3d(v[0],v[1],v[2]);
                            glPointSize(4);
                            glColor3f(1,1,1);
                        } else {
                            glVertex3d(v[0],v[1],v[2]);
                        }
                    }
                }
            }
        glEnd();
        glTranslatef(0.5,0.5,0.5);
        break;
    default:
        break;
    }
}

//Aplica uma matriz de transformação nos pontos
void FFDGrid::applyTransform(tnw::TransformMatrix m)
{
    for (int i = 0; i <= ns; i++){
        for (int j = 0; j <= nt; j++){
            for (int k = 0; k <= nu; k++){
                this->points(i,j,k) = m * this->points(i,j,k);
            }
        }
    }
}

void FFDGrid::reset()
{
    //Reaplica a inicialização dos pontos
    for (int i = 0; i <= ns; i++){
        for (int j = 0; j <= nt; j++){
            for (int k = 0; k <= nu; k++){
                QVector4D v = this->p + float(i)/float(this->ns) * this->S + float(j)/float(this->nt) * this->T + float(k)/float(this->nu) * this->U;
                this->points(i,j,k) = v;
            }
        }
    }
}

