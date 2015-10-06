#include "gridpoints.h"


std::vector<QVector4D> GridPoints::getData() const
{
    return _data;
}
GridPoints::GridPoints(int i, int j, int k)
{
    this->_data.reserve(i*j*k);
    this->ni = i;
    this->nj = j;
    this->nk = k;
    //Ocupando o objeto
    this->_data.insert(this->_data.begin(), i*j*k, QVector4D(0.0,0.0,0.0,0.0));
}

GridPoints::~GridPoints()
{
}

void GridPoints::set(int i, int j, int k, QVector4D p)
{
    this->_data.at(i * j*this->ni + k*this->ni*this->nj) = p;
}

QVector4D GridPoints::get(int i, int j, int k)
{
    return this->_data.at(i * j*this->ni + k*this->ni*this->nj);
}

QVector4D GridPoints::operator()(int i, int j, int k) const
{
    return this->_data.at(i * j*this->ni + k*this->ni*this->nj);
}

int GridPoints::dataSize() const
{
    return this->_data.size();
}

QVector4D &GridPoints::operator()(int i, int j, int k)
{
    return this->_data.at(i + j*this->ni + k*this->ni*this->nj);
}


