#include "light.h"
#include <stdexcept>

void tnw::Light::operator=(const tnw::Light &direita)
{
    for (int i = 0; i < 3; ++i)
        pos[i] = direita[i];
    setKd(direita.getKd(0),direita.getKd(1),direita.getKd(2));
    setKs(direita.getKs(0),direita.getKs(1),direita.getKs(2));
}

float &tnw::Light::operator[](unsigned i)
{
    if(i<3)
        return pos[i];
    throw std::invalid_argument("Indice fora da matrix");
}

float tnw::Light::operator[](unsigned i) const
{
    if(i<3)
        return pos[i];
    throw std::invalid_argument("Indice fora da matrix");
}

float &tnw::Light::getKd(unsigned i)
{
    if(i<3)
        return componentes[0][i];
    throw std::invalid_argument("Indice fora da matrix");
}

float tnw::Light::getKd(unsigned i) const
{
    if(i<3)
        return componentes[0][i];
    throw std::invalid_argument("Indice fora da matrix");
}

void tnw::Light::setKd(float a, float b, float c)
{
    componentes[0][0] = a;
    componentes[0][1] = b;
    componentes[0][2] = c;
}

float &tnw::Light::getKs(unsigned i)
{
    if(i<3)
        return componentes[1][i];
    throw std::invalid_argument("Indice fora da matrix");
}

float tnw::Light::getKs(unsigned i) const
{
    if(i<3)
        return componentes[1][i];
    throw std::invalid_argument("Indice fora da matrix");
}

void tnw::Light::setKs(float a, float b, float c)
{
    componentes[1][0] = a;
    componentes[1][1] = b;
    componentes[1][2] = c;
}

QVector3D tnw::Light::getPos()
{
    return QVector3D(pos[0],pos[1],pos[2]);
}

tnw::Light::Light(float x, float y, float z)
{
    pos = new float[3];
    componentes = new float*[2];
    for (int i = 0; i < 2; ++i)
            componentes[i] = new float[3];

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            componentes[i][j] = 0;
}

tnw::Light::Light(const tnw::Light &direita)
{
    pos = new float[3];
    componentes = new float*[2];
    for (int i = 0; i < 2; ++i)
            componentes[i] = new float[3];

    for (int i = 0; i < 3; ++i)
        pos[i] = direita[i];
    setKd(direita.getKd(0),direita.getKd(1),direita.getKd(2));
    setKs(direita.getKs(0),direita.getKs(1),direita.getKs(2));
}

tnw::Light::~Light()
{
    delete pos;
    for (int i = 0; i < 2; ++i) {
        delete componentes[i];
    }
    delete componentes;
}
