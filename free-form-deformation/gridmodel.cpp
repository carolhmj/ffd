#include "gridmodel.h"
#include <cfloat>
#include <cmath>
#include <iostream>

using namespace std;
/* Construtor da classe. Recebe o nome do modelo, e
 * a quantidade de pontos em cada direção. Nesse caso,
 * ele automaticamente irá definir os vetores S,T,U como
 * partindo do menor para o maior ponto do modelo
 */

tnw::Model *GridModel::getModel() const
{
    return model;
}

FFDGrid *GridModel::getGrid() const
{
    return grid;
}
GridModel::GridModel(QString modelName, int ns, int nt, int nu)
{
    this->model = new tnw::Model(modelName);
    //Precisamos descobrir os menores e maiores pontos do modelo
    float maxcoords[3] = {FLT_MIN, FLT_MIN, FLT_MIN}, mincoords[3] = {FLT_MAX, FLT_MAX, FLT_MAX};
    for (int i = 0; i < model->getVertices().size(); i++){
        float x = model->getVertices()[i].x();
        float y = model->getVertices()[i].y();
        float z = model->getVertices()[i].z();
        if (x > maxcoords[0]){
            maxcoords[0] = x;
        } else if (x < mincoords[0]){
            mincoords[0] = x;
        }
        if (y > maxcoords[1]){
            maxcoords[1] = y;
        } else if (y < mincoords[1]){
            mincoords[1] = y;
        }
        if (z > maxcoords[2]){
            maxcoords[2] = z;
        } else if (z < mincoords[2]){
            mincoords[2] = z;
        }
    }

    QVector4D p(mincoords[0],mincoords[1],mincoords[2],1);
    //QVector4D p(0,0,0,1);
    QVector4D S(maxcoords[0]-mincoords[0],0,0,0);
    QVector4D T(0,maxcoords[1]-mincoords[1],0,0);
    QVector4D U(0,0,maxcoords[2]-mincoords[2],0);
    //cout << "p: " << p << "s: " << S << "t: " << T << "u: " << U;
    cout << "p: " << p[0] << " " << p[1] << " " << p[2] << "\n";
    cout << "S: " << S[0] << " " << S[1] << " " << S[2] << "\n";
    cout << "T: " << T[0] << " " << T[1] << " " << T[2] << "\n";
    cout << "U: " << U[0] << " " << U[1] << " " << U[2] << "\n";
    this->grid = new FFDGrid(p,S,T,U,ns,nt,nu);
    calcLocalModelVertices();

}

GridModel::GridModel(QString modelName, QVector4D p, QVector4D S, QVector4D T, QVector4D U, int ns, int nt, int nu)
{
    this->model = new tnw::Model(modelName);
    this->grid = new FFDGrid(p,S,T,U,ns,nt,nu);
    calcLocalModelVertices();
}

void GridModel::calcLocalModelVertices()
{
    if (grid == NULL){
        return;
    }

    /* Agora vamos descobrir as coordenadas locais dos vértices do grid,
     * segundo a fórmula do livro Computer Animation, p. 150. A ordem
     * das coordenadas locais na lista será a mesma ordem dos vértices
     * na lista.
     */
    QVector3D pn = this->grid->getP().toVector3D();
    QVector3D Sn = this->grid->getS().toVector3D();
    QVector3D Tn = this->grid->getT().toVector3D();
    QVector3D Un = this->grid->getU().toVector3D();
    auto vertices = this->model->getVertices();
    for (int i = 0; i < vertices.size(); i++){
        QVector4D coord;
        coord.setX( formulaHelper(Tn,Un,Sn,vertices[i].toVector3D(),pn) );
        coord.setY( formulaHelper(Un,Sn,Tn,vertices[i].toVector3D(),pn) );
        coord.setZ( formulaHelper(Sn,Tn,Un,vertices[i].toVector3D(),pn) );
        coord.setW( 1 );
        this->modelVerticesLocal.push_back(coord);
    }
}

/* Função para calcular um termo da fórmula do livro computer animation,
 * p.150, Eq. 4.3, 4.4 e 4.5
 */
float GridModel::formulaHelper(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D p, QVector3D p0)
{
    //QVector3D::dotProduct( QVector3D::crossProduct(v1,v2), (p - p0) / QVector3D::dotProduct(QVector3D::crossProduct(v1,v2),v3));
    return QVector3D::dotProduct(QVector3D::crossProduct(v1,v2), p - p0) / QVector3D::dotProduct(QVector3D::crossProduct(v1,v2), v3);
}

//Substitui o grid por outro, recalcula as coordenadas locais
void GridModel::substituteGrid(FFDGrid *grid)
{
    delete this->grid;
    this->grid = grid;
    calcLocalModelVertices();
}

/* Recebe um modelo e deforma seus vértices de acordo com a fórmula
 * de deformação do grid, no caso, a interpolação de Bézier mostrada
 * no livro Computer Animation, p. 151
 */
void GridModel::deformModel()
{
    if (model == NULL || grid == NULL) {
        return;
    }
    auto modelVertices = model->getVertices();
    for (int m = 0; m < modelVertices.size(); m++){
        QVector4D localCoords = modelVerticesLocal[m];
        modelVertices[m] = bezierHelper(localCoords.x(), localCoords.y(), localCoords.z());
    }
}

QVector4D GridModel::bezierHelper(float s, float t, float u)
{
    QVector4D acum;
    int l = grid->getNs(), m = grid->getNt(), n = grid->getNu();
    for (int i = 0; i <= l; i++){
        for (int j = 0; j <= m; j++){
            for (int k = 0; k <= n; k++){
                acum += combiHelper(l,i) * combiHelper(m,j) * combiHelper(n,k) * std::pow(1-s, l-i) * std::pow(s,i) * std::pow(1-t, m-j) * std::pow(t, j) * std::pow(1-u, n-k) * std::pow(u,k) * grid->getPoints().get(i,j,k);
            }
        }
    }
    //Considerando que os pontos do grid são multiplicados várias vezes, acho que é necessário
    acum.setW(1);
}

/* Código que computa coeficientes binomiais. Encontrei em:
 * http://stackoverflow.com/questions/24294192/computing-the-binomial-coefficient-in-c
 */
long long GridModel::combiHelper(int n, int k)
{
    long long ans=1;
    k=k>n-k?n-k:k;
    int j=1;
    for(;j<=k;j++,n--)
    {
        if(n%j==0)
        {
            ans*=n/j;
        }else
        if(ans%j==0)
        {
            ans=ans/j*n;
        }else
        {
            ans=(ans*n)/j;
        }
    }
    return ans;
}

