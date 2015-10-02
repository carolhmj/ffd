#ifndef FFDGRID_H
#define FFDGRID_H

//#include "KDtree.h"
#include "Vec.h"
#include "gridpoints.h"

using namespace trimesh;
/*
 * Classe que representa o grid de pontos usados na FFD.
 * Guarda o ponto P0, os vetores (S,T,U) que definem o FFD,
 * a quantidade de pontos em cada direção dos vetores, uma
 * estrutura guardando as coordenadas de cada ponto, e constantes
 * dizendo os modos de desenho.
*/
class FFDGrid
{
private:
    vec p;
    vec S, T, U;
    int ns, nt, nu;
    GridPoints points;
    enum DrawMode {POINTS_ONLY, WIRE};
public:
    FFDGrid(trimesh::vec p, trimesh::vec S, trimesh::vec T, trimesh::vec U, int ns, int nt, int nu);
    ~FFDGrid();

    void draw(int mode);
    void reset();
};

#endif // FFDGRID_H
