#include "model.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <cstdio>
#include <iostream>
#include <QStringList>
#include <QRegularExpression>
#include <QRegExp>
#include <GL/gl.h>

using std::get;

void tnw::Model::desenhar(float ambiente[3],QList<tnw::Light> luzes)
{
    glBegin(GL_TRIANGLES);
    foreach (tnw::Face f, this->faces) {
        QVector4D v1 = *get<0>(f), v2 = *get<1>(f), v3 = *get<2>(f);
        QVector3D a(v2-v1),b(v3-v1),point(v1*1/3 + v2*1/3 + v3*1/3);

        QVector3D n(QVector3D::normal(a,b));

        // Passando os vértices pro OpenGL
        if(get<6>(f) != nullptr)
            glColor3fv(get<6>(f)->toColor(point,n,ambiente,luzes));
        glVertex3d(v1.x(),v1.y(),v1.z());
        glVertex3d(v2.x(),v2.y(),v2.z());
        glVertex3d(v3.x(),v3.y(),v3.z());

    }
    glEnd();
}

void tnw::Model::desenhar(tnw::TransformMatrix m,float ambiente[3],QList<tnw::Light> luzes)
{

    foreach (tnw::Face f, this->faces) {
        QVector4D v1 = m*(*get<0>(f)), v2 = m*(*get<1>(f)), v3 = m*(*get<2>(f));
        QVector3D b(v2-v1),a(v3-v1),point(v1*1/3 + v2*1/3 + v3*1/3);

        QVector3D n(QVector3D::normal(a,b));
        if(get<6>(f) != nullptr)
            glColor3fv(get<6>(f)->toColor(point,n,ambiente,luzes));
        glBegin(GL_TRIANGLES);
        glVertex3d(v1.x(),v1.y(),v1.z());
        glVertex3d(v2.x(),v2.y(),v2.z());
        glVertex3d(v3.x(),v3.y(),v3.z());
        glEnd();
        /*glBegin(GL_POINTS);
            glVertex3d(point.x(),point.y(),point.z());
        glEnd();*/
    }

}

void tnw::Model::aplicarTransformacao(TransformMatrix m)
{
    QMutableListIterator<tnw::Vertice> iter(vertices);
    while (iter.hasNext()) {
        tnw::Vertice v = iter.next();
        iter.setValue(m*v);
    }
}

void tnw::Model::aplicarTransformacao(tnw::TransformMatrix m, int i)
{
    if(i < grupos.length()) {
        QMutableListIterator<tnw::Vertice*> iter(grupos[i]);
        while (iter.hasNext()) {
            tnw::Vertice* v = iter.next();
            *v = (m*(*v));
        }
    }
    else
        throw std::out_of_range("Grupo não existente");
}

QList<tnw::Vertice> tnw::Model::getVertices()
{
    return this->vertices;
}

QList<tnw::Face> tnw::Model::getFaces()
{
    return this->faces;
}

QHash<QString, tnw::ColorMatrix> tnw::Model::getPaleta()
{
    return paleta;
}

tnw::Vertice tnw::Model::getPontoMedio()
{
    double medio[4] = {0,0,0,1};
    int n = vertices.length();

    foreach (Vertice v, vertices) {
        for (int i = 0; i < 3; ++i) {
            medio[i] += v[i]/n;
        }
    }

    pontoMedio[0] = medio[0];
    pontoMedio[1] = medio[1];
    pontoMedio[2] = medio[2];
    pontoMedio[3] = medio[3];

    return pontoMedio;
}

tnw::Vertice tnw::Model::getPontoMedio(int i)
{
    if(i < grupos.length()) {
        float medio[4] = {0,0,0,1};
        int n = 0;
        tnw::Vertice pontoMedioG;

        foreach (Vertice* v, grupos[i])
            if((*v)[3]==1) n++;

        foreach (Vertice* v, grupos[i])
            if((*v)[3]==1)
                for (int j = 0; j < 3; ++j)
                    medio[j] += (*v)[j]/n;

        pontoMedioG[0] = medio[0];
        pontoMedioG[1] = medio[1];
        pontoMedioG[2] = medio[2];
        pontoMedioG[3] = medio[3];

        return pontoMedioG;
    }
    else
        throw std::out_of_range("Grupo não existente");
}

QHash<QString,tnw::ColorMatrix> lerMateriais(QString path) {
    QFile arquivo(path);
    if(!arquivo.open(QIODevice::ReadOnly)) {
        printf("E: %s\n",arquivo.errorString().toStdString().c_str());
        throw -1;
    }

    QHash<QString,tnw::ColorMatrix> paletaDeCores;
    QString chave;
    tnw::ColorMatrix cores;

    QString linha;
    QStringList parseada;

    bool lendoMaterial = false;

    QRegExp regularNome("([\\S]*)([\\s]*)([\\S]*)([\\s]*)");
    QRegExp regularKvalue("([\\S]*)([\\s]*)([\\S]*[\\s]*)([\\S]*[\\s]*)([\\S]*[\\s]*)");

    while(!arquivo.atEnd()) {
        linha = arquivo.readLine();
        switch (linha[0].toLatin1()) {
            case '#': // É um comentário
                break;
            case 'n': // Nome de um material
                if(lendoMaterial){ paletaDeCores.insert(chave,cores);printf("Adicionado %s!\n",chave.toStdString().c_str());}
                lendoMaterial = true;
                regularNome.indexIn(linha);
                parseada = regularNome.capturedTexts();
                chave = parseada[3];
                break;
            case 'K': // É um valor de cor
                regularKvalue.indexIn(linha);
                parseada = regularKvalue.capturedTexts();
                if(parseada[1] == "Ka")
                    cores.setKa(parseada[3].toFloat(),parseada[4].toFloat(),parseada[5].toFloat());
                if(parseada[1] == "Kd")
                    cores.setKd(parseada[3].toFloat(),parseada[4].toFloat(),parseada[5].toFloat());
                if(parseada[1] == "Ks")
                    cores.setKs(parseada[3].toFloat(),parseada[4].toFloat(),parseada[5].toFloat());
                break;
        }
    }
    if(lendoMaterial){ paletaDeCores.insert(chave,cores);printf("Adicionado %s!\n",chave.toStdString().c_str());}

    return paletaDeCores;
}


tnw::Model::Model(QString pathname)
{
    if(pathname.isEmpty())
        throw -1;
    // Se tudo estiver certo..
    QFile arquivo(pathname);
    // Vamos tentar abrir o arquivo
    if(!arquivo.open(QIODevice::ReadOnly)) {
        std::cerr << arquivo.errorString().toStdString() << std::endl;
        throw -1;
    }
    std::cout << "Modelo carregado: " << pathname.toStdString() << std::endl;

    QFileInfo info(arquivo);
    QString linha;
    QStringList parseada;
    QStringList verticesDaFace;

    int* vtx = new int[3];
    //int* txt = new int[3];
    int* nrm = new int[3];
    tnw::ColorMatrix* corAtual= nullptr;

    QRegExp regularExpression("([\\S]*)([\\s]*)([\\S]*[\\s]*)([\\S]*[\\s]*)([\\S]*[\\s]*)");

    while(!arquivo.atEnd())
    {
        linha = arquivo.readLine();
        switch (linha[0].toLatin1()) {
        case '#':
            // É um comentário
            break;
        case 'm': // Se é o link de um material
            regularExpression.indexIn(linha);
            parseada = regularExpression.capturedTexts();
            if(parseada[1] == "mtllib")
                paleta = lerMateriais((info.dir().absolutePath() + "/" + parseada[3]).trimmed());
            break;
        case 'g': // Se for um grupo.
            grupos << tnw::Grupo(); // Adiciona um novo grupo
            break;
        case 'v': // Se for um vértice ou uma normal ou uma textura...
            regularExpression.indexIn(linha);
            parseada = regularExpression.capturedTexts();
            if(parseada[1] == "v") {
                vertices << tnw::Vertice(parseada[3].toFloat(),parseada[4].toFloat(),parseada[5].toFloat(),1);
                if(grupos.length() > 0)
                    grupos.last() << &vertices.last();
            }
            if(parseada[1] == "vn") {
                normais << tnw::Vertice(parseada[3].toFloat(),parseada[4].toFloat(),parseada[5].toFloat(),0);
                if(grupos.length() > 0)
                    grupos.last() << &normais.last();
            }
            break;
        case 'u': // Se é uma instrução para usar uma cor
            regularExpression.indexIn(linha);
            parseada = regularExpression.capturedTexts();
            if(parseada[1] == "usemtl") {
                std::cout << "Usando " << parseada[3].trimmed().toStdString() << std::endl;
                corAtual = &paleta[parseada[3].trimmed()];
            }
            break;
        case 'f': // Se for uma face...
            regularExpression.indexIn(linha);
            parseada = regularExpression.capturedTexts();
            for (int i = 3; i < 6; ++i) {
                verticesDaFace = parseada[i].split("/",QString::KeepEmptyParts);
                if(!verticesDaFace[0].isEmpty())
                    vtx[i-3] = verticesDaFace[0].toInt()-1;
                if(verticesDaFace.size() > 2 && !verticesDaFace[2].isEmpty())
                    nrm[i-3] = verticesDaFace[2].toInt()-1;
                else
                    nrm[i-3] = -1;
            }

            faces << tnw::Face(&(vertices[vtx[0]]),
                               &(vertices[vtx[1]]),
                               &(vertices[vtx[2]]),
                               &(normais[nrm[0]]),
                               &(normais[nrm[1]]),
                               &(normais[nrm[2]]),
                                 corAtual);
            break;
        default:
            break;
        }
    }
    delete[] vtx;
    //delete[] txt;
    delete[] nrm;

    std::cout << "Quantidade de grupos: " << grupos.length() << std::endl;

    float max[4]={0,0,0,0};
    foreach (tnw::Vertice v, vertices) {
        for (int i = 0; i < 3; ++i) {
            if(std::abs(v[i]) > max[i])
                max[i] = std::abs(v[i]);
        }
    }
    for (int i = 0; i < 3; ++i) {
        if(max[i] > max[3])
            max[3] = max[i];
    }

    QMutableListIterator<tnw::Vertice> iter(vertices);
    while (iter.hasNext()) {
        tnw::Vertice v = iter.next();
        iter.setValue(tnw::Vertice(v.x()/max[3],v.y()/max[3],v.z()/max[3],1));
    }

    double medio[3] = {0,0,0};

    foreach (Vertice v, vertices) {
        for (int i = 0; i < 3; ++i) {
            medio[i] += v[i];
        }
    }

    for (int i = 0; i < 3; ++i) {
        medio[i] /= vertices.length();
    }

    pontoMedio.setX(medio[0]);
    pontoMedio.setY(medio[1]);
    pontoMedio.setZ(medio[2]);
    pontoMedio.setW(1);

    std::cout << "Ponto Médio: " << pontoMedio[0] << " , " << pontoMedio[1] << " , " << pontoMedio[2] << std::endl;
}

tnw::Model::Model(QList<tnw::Vertice> vertices, QList<tnw::Face> faces)
{
    this->faces = faces;
    this->vertices = vertices;
}
