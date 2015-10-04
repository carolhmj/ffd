#ifndef MODEL_H
#define MODEL_H

#include "TriMesh.h"
#include "Vec.h"
#include "ffdgrid.h"
#include <GL/gl.h>
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
    Model(const char *modelName, int ns, int nt, int nu);
    void drawModel();
    void deformGrid();
    FFDGrid *getGrid() const;
    void setGrid(FFDGrid *value);
};

#endif // MODEL_H
