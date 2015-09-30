#include "gridpoints.h"

GridPoints::GridPoints(int i, int j, int k)
{
    this->_data.reserve(i*j*k);
    this->ni = i;
    this->nj = j;
    this->nk = k;
}

void GridPoints::set(int i, int j, int k, vec p)
{
    this->_data.at(i * j*this->ni + k*this->ni*this->nj) = p;
}

vec GridPoints::get(int i, int j, int k)
{
    return this->_data.at(i * j*this->ni + k*this->ni*this->nj);
}


