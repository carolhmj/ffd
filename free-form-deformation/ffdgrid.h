#ifndef FFDGRID_H
#define FFDGRID_H

#include <QVector3D>
#include <QVector4D>
#include <QString>
#include "gridpoints.h"
#include "transformmatrix.h"
#include "model.h"

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
    QVector4D p;
    QVector4D S, T, U;
    int ns, nt, nu;
    GridPoints points;
    QVector3D selectedPoint;
    enum DrawMode {POINTS_ONLY, WIRE};
public:
    FFDGrid(QVector4D p, QVector4D S, QVector4D T, QVector4D U, int ns, int nt, int nu);
    ~FFDGrid();

    QVector4D getMedianPoint();
    void draw(int mode);
    void applyTransform(tnw::TransformMatrix m);
    void reset();
    int getNs() const;
    int getNt() const;
    int getNu() const;
    GridPoints getPoints() const;
    QVector4D getP() const;
    QVector4D getS() const;
    QVector4D getT() const;
    QVector4D getU() const;
    QVector3D getSelectedPoint() const;
    void setSelectedPoint(const QVector3D &value);
    void moveGridPoint(int i, int j, int k, QVector4D p);
    QString printPointsInString();
};

#endif // FFDGRID_H
