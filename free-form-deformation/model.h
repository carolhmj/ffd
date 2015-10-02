#ifndef MODEL_H
#define MODEL_H

#include "TriMesh.h"
#include "Vec.h"
#include "ffdgrid.h"
#include <string>

using namespace trimesh;
/* Classe dedicada a armazenar o modelo a ser deformado.
 * Cada classe é associada a um grid(?)
 */
class Model
{
private:
    TriMesh* model;
    FFDGrid* grid;
public:
    Model(std::string modelName, int ns, int nt, int nu);
    void draw();
    void deformGrid();
};

#endif // MODEL_H
