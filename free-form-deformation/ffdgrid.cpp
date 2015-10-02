#include "ffdgrid.h"
#include <GL/gl.h>
#include <iostream>

/*Construtor do FFDGrid. Coloca os pontos em suas posições
 * iniciais de acordo com a fórmula
 */
using namespace std;
FFDGrid::FFDGrid(trimesh::vec p, trimesh::vec S, trimesh::vec T, trimesh::vec U, int ns, int nt, int nu) :
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

    //Inicialização dos pontos
    for (int i = 0; i <= ns; i++){
        for (int j = 0; j <= nt; j++){
            for (int k = 0; k <= nu; k++){
                vec v = p + float(i)/float(ns) * S + float(j)/float(nt) * T + float(k)/float(nu) * U;
                //std::cout << "p" << i << ", " << j << ", " << k << ": " << v;
                //flush(std::cout);
                this->points(i,j,k) = v;
            }
        }
    }
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
                        vec v1 = this->points(i,j,k);
                        //Vizinho da frente - Não existe quando k é max
                        if (k < nu){
                            vec v2 = this->points(i,j,k+1);
                            glVertex3d(v1[0],v1[1],v1[2]);
                            glVertex3d(v2[0],v2[1],v2[2]);
                        }

                        //Vizinho de cima - Não existe quando j é max
                        if (j < nt){
                           vec v3 = this->points(i,j+1,k);
                           glVertex3d(v1[0],v1[1],v1[2]);
                           glVertex3d(v3[0],v3[1],v3[2]);
                        }

                        //Vizinho da esquerda - Não existe quando i é max
                        if (i < ns){
                            vec v4 = this->points(i+1,j,k);
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
                        vec v = this->points(i,j,k);
                        glVertex3d(v[0],v[1],v[2]);
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

void FFDGrid::reset()
{
    //Reaplica a inicialização dos pontos
    for (int i = 0; i <= ns; i++){
        for (int j = 0; j <= nt; j++){
            for (int k = 0; k <= nu; k++){
                vec v = this->p + float(i)/float(this->ns) * this->S + float(j)/float(this->nt) * this->T + float(k)/float(this->nu) * this->U;
                this->points(i,j,k) = v;
            }
        }
    }
}

