#ifndef GRIDPOINTS_H
#define GRIDPOINTS_H

#include <vector>
#include <QVector4D>

/*
 * Classe que armazena os pontos da grade em uma representação de matriz 3D
 * Os pontos são armazenados em um único vetor contínuo e acessados em
 * row-major order
*/
class GridPoints
{
private:
    std::vector<QVector4D> _data;
    int ni,nj,nk;
public:
    GridPoints(int i, int j, int k);
    ~GridPoints();
    void set(int i, int j, int k, QVector4D p);
    QVector4D get(int i, int j, int k);
    QVector4D& operator()(int i, int j, int k);
    QVector4D operator()(int i, int j, int k) const;
    int dataSize() const;
    std::vector<QVector4D> getData() const;
};

#endif // GRIDPOINTS_H
