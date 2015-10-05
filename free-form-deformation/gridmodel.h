#ifndef GRIDMODEL_H
#define GRIDMODEL_H

#include <QVector4D>
#include <QVector3D>
#include <QString>
#include <vector>
#include "model.h"
#include "ffdgrid.h"

/* Classe que associa um modelo a um grid de FFD.
 * Armazena o modelo, o grid, e as coordenadas do
 * modelo no grid
 */
class GridModel
{
private:
    tnw::Model *model;
    FFDGrid *grid;
    std::vector<QVector4D> modelVerticesLocal;
public:
    GridModel(QString modelName, int ns, int nt, int nu);
    void calcLocalModelVertices();
    float formulaHelper(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D p, QVector3D p0);
    void substituteGrid();
    void deformModel();
    QVector4D bezierHelper(float s, float t, float u);
    long long combiHelper(int n, int k);
};

#endif // GRIDMODEL_H
