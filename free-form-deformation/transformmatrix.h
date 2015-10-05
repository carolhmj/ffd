#ifndef TRANSFORMMATRIX_H
#define TRANSFORMMATRIX_H

#include <vector>
#include <initializer_list>
#include <QVector4D>
#include <QVector3D>

namespace tnw {

    class TransformMatrix
    {
    private:
        std::vector<std::vector<double>> matrix;
    public:
        // Métodos
        void mostrar();

        // Operadores
        TransformMatrix operator*(const TransformMatrix& direita);  // Multiplicação de Matriz
        TransformMatrix& operator= (const TransformMatrix& direita);// Atribuição
        double& operator() (unsigned i, unsigned j); // Acesso para modificação
        double operator() (unsigned i, unsigned j) const; // Acesso para visualização

        // Construtores
        TransformMatrix(); // Identidade
        TransformMatrix(std::initializer_list<std::initializer_list<double>> l); // Dado em tempo de compilação
        TransformMatrix(double a[4][4]); // Por um vetor
        TransformMatrix(const TransformMatrix &obj); //Copiador

        // Desconstrutor
        ~TransformMatrix();

        //Get and Setter
        void setMatrix(double a[4][4]);
    };

    //Operador Matrix * QVector4D
    QVector4D operator*(const TransformMatrix& esquerda, const QVector4D& direita);

    //Funções que retornam as matrizes de transformação
    TransformMatrix translacao(double x, double y, double z);
    TransformMatrix translacao(QVector3D v);
    TransformMatrix translacao(QVector4D v);
    TransformMatrix rotacaoX(double angGraus);
    TransformMatrix rotacaoY(double angGraus);
    TransformMatrix rotacaoZ(double angGraus);
    TransformMatrix rotacaoVetor(double angGraus, QVector3D v);
    TransformMatrix escala(double x, double y, double z);

    //Função pra transformar graus em radianos
    double radianos(double graus);

    //Funções que retornam as matrizes de projeção
    TransformMatrix ortho(double left, double right, double bottom, double top, double near, double far);
    TransformMatrix frustum(double left, double right, double bottom, double top, double near, double far);
    TransformMatrix perspective(double fovy, double aspect, double near, double far);
    TransformMatrix isometric(double scale,double near, double far,bool positive_hor, bool positive_ver);

}
#endif // TRANSFORMMATRIX_H
