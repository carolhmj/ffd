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
        cout << "Drawing lines\n";
        flush(cout);
        glColor3f(1,0,0);
        glBegin(GL_LINES);
            for (int i=0; i <= ns; i++){
                for (int j=0; j <= nt; j++){
                    for (int k=0; k < nu; k++){
                        vec v1 = this->points(i,j,k);
                        vec v2 = this->points(i,j,k+1);
                        glVertex3d(v1[0],v1[0],v1[0]);
                        glVertex3d(v2[0],v2[0],v2[0]);
                    }
                }
            }
        glEnd();
    case POINTS_ONLY:
        //std::cout << "Drawing points\n";
        //flush(std::cout);
        glColor3f(1,1,1);
        glEnable(GL_POINT_SMOOTH);
        glPointSize(2);
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
        break;
    default:
        break;
    }
}

