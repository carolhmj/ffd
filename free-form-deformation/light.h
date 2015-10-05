#ifndef LUZES_H
#define LUZES_H
#include <QVector3D>
#include <QList>

namespace tnw {
    class Light
    {
    private:
        float** componentes;
        float* pos;
    public:
        void operator= (const Light& direita);// Atribuição
        float& operator[](unsigned i);
        float operator[](unsigned i) const;

        float& getKd(unsigned i);
        float  getKd(unsigned i) const;
        void   setKd(float a, float b, float c);

        float& getKs(unsigned i);
        float  getKs(unsigned i) const;
        void   setKs(float a, float b, float c);

        QVector3D getPos();

        Light(float x, float y, float z);
        Light(const Light& l); // Copiador

        ~Light();
    };
}
#endif // LUZES_H
