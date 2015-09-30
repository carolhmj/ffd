#ifndef GRIDPOINTS_H
#define GRIDPOINTS_H

#include <vector>
#include <Vec.h>

using namespace trimesh;
/*
 * Classe que armazena os pontos da grade em uma representação de matriz 3D
 * Os pontos são armazenados em um único vetor contínuo e acessados em
 * row-major order
*/
class GridPoints
{
private:
    std::vector<vec> _data;
    int ni,nj,nk;
public:
    GridPoints(int i, int j, int k);
    void set(int i, int j, int k, vec p);
    vec get(int i, int j, int k);
    vec& operator[](int i, int j, int k);
};

#endif // GRIDPOINTS_H
