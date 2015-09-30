#include "gridpoints.h"

GridPoints::GridPoints(int i, int j, int k)
{
    this->_data.reserve(i*j*k);
    this->ni = i;
    this->nj = j;
    this->nk = k;
    this->_data.insert(this->_data.begin(), i*j*k, 0);
}

GridPoints::~GridPoints()
{
}

void GridPoints::set(int i, int j, int k, vec p)
{
    this->_data.at(i * j*this->ni + k*this->ni*this->nj) = p;
}

vec GridPoints::get(int i, int j, int k)
{
    return this->_data.at(i * j*this->ni + k*this->ni*this->nj);
}

vec GridPoints::operator()(int i, int j, int k) const
{
    return this->_data.at(i * j*this->ni + k*this->ni*this->nj);
}

int GridPoints::dataSize() const
{
    return this->_data.size();
}

vec &GridPoints::operator()(int i, int j, int k)
{
    return this->_data.at(i + j*this->ni + k*this->ni*this->nj);
}


