#ifndef FFDGRID_H
#define FFDGRID_H

#include "KDtree.h"
#include "Vec.h"
#include "gridpoints.h"

using namespace trimesh;
/*
 * Classe que representa o grid de pontos usados na FFD.
 * Guarda os vetores (S,T,U) que definem o FFD, a quantidade de pontos
 * em cada direção dos vetores, e uma estrutura guardando as coordenadas
 * de cada ponto.
*/
class FFDGrid
{
private:
    vec S, T, U;
    int ns, nt, nu;
    GridPoints points;
public:
    FFDGrid(vec S, vec T, vec U, int ns, int nt, int nu);
    ~FFDGrid();

};

#endif // FFDGRID_H
