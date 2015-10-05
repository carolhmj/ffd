#include "gridmodel.h"
#include <cfloat>
#include <cmath>

/* Construtor da classe. Recebe o nome do modelo, e
 * a quantidade de pontos em cada direção. Nesse caso,
 * ele automaticamente irá definir os vetores S,T,U como
 * partindo do menor para o maior ponto do modelo
 */
GridModel::GridModel(QString modelName, int ns, int nt, int nu)
{
    this->model = new tnw::Model(modelName);
    //Precisamos descobrir os menores e maiores pontos do modelo
    float maxcoords[3] = {FLT_MIN, FLT_MIN, FLT_MIN}, mincoords[3] = {FLT_MAX, FLT_MAX, FLT_MAX};
    for (auto it = model->getVertices().begin(); it < model->getVertices().end(); it++){
        float x = (*it).x();
        float y = (*it).y();
        float z = (*it).z();
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
    QVector4D S(maxcoords[0],mincoords[1],mincoords[2],0);
    QVector4D T(mincoords[0],maxcoords[1],mincoords[2],0);
    QVector4D U(mincoords[0],mincoords[1],maxcoords[2],0);

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
    QVector3D pn = p.toVector3D();
    QVector3D Sn = S.toVector3D();
    QVector3D Tn = T.toVector3D();
    QVector3D Un = U.toVector3D();
    auto vertices = this->model->getVertices();
    for (unsigned int i = 0; i < vertices.size(); i++){
        QVector4D coord;
        coord.setX( formulaHelper(Tn,Un,Sn,vertices[i],pn) );
        coord.setY( formulaHelper(Un,Sn,Tn,vertices[i],pn) );
        coord.setZ( formulaHelper(Sn,Tn,Un,vertices[i],pn) );
        coord.setW( 1 );
        this->modelVerticesLocal[i] = coord;
    }
}

/* Função para calcular um termo da fórmula do livro computer animation,
 * p.150, Eq. 4.3, 4.4 e 4.5
 */
float GridModel::formulaHelper(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D p, QVector3D p0)
{
    //QVector3D::dotProduct( QVector3D::crossProduct(v1,v2), (p - p0) / QVector3D::dotProduct(QVector3D::crossProduct(v1,v2),v3));
    QVector3D::dotProduct(QVector3D::crossProduct(v1,v2), p - p0) / QVector3D::dotProduct(QVector3D::crossProduct(v1,v2), v3);
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
        QVector4D localCoords = modelVerticesLocal[i];
        modelVertices[i] = bezierHelper(localCoords.x(), localCoords.y(), localCoords.z());
    }
}

QVector4D GridModel::bezierHelper(float s, float t, float u)
{
    QVector4D acum;
    for (int i = 0; i <= grid->getNs(); i++){
        for (int j = 0; j <= grid->getNt(); j++){
            for (int k = 0; k <= grid->getNu(); k++){
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

