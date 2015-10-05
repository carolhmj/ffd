#ifndef TNW_MODEL_H
#define TNW_MODEL_H
#include <QList>
#include <QHash>
#include <QVector3D>
#include <QVector4D>
#include <tuple>
#include "transformmatrix.h"
#include "colormatrix.h"

namespace tnw {

	/** Um vertice tem a mesma representação que um QVector3D. */
    typedef QVector4D Vertice;

    /** Uma face é simplesmente uma tripla de Vertices. */
    typedef std::tuple<Vertice*,Vertice*,Vertice*,
                       Vertice*,Vertice*,Vertice*,
                       tnw::ColorMatrix*
                      >Face;

    /* Um grupo de vértices é uma lista de vértices */
    typedef QList<Vertice*> Grupo;

	/**
	 *  Esta classe representa um modelo.
	 */
    class Model {
	private:
        QList<Vertice> vertices; // A lista de vertices normalzados do modelo.
        QList<Vertice> normais; // A lista de normais normalizados.
        QList<Face> faces;     // A lista de faces do modelo.
        QList<Grupo> grupos;  // A lista de grupos
        Vertice pontoMedio;  // Ponto médio do objeto, pode estar desatualizado
        QHash<QString,tnw::ColorMatrix> paleta; // Paleta de cores do modelo
	public:
        //Métodos
        void desenhar(float ambiente[3],QList<tnw::Light> luzes);
        void desenhar(TransformMatrix m,float ambiente[3],QList<tnw::Light> luzes);
        void aplicarTransformacao(TransformMatrix m); // Aplica a transformação no modelo inteiro
        void aplicarTransformacao(TransformMatrix m,int i); // Aplica a transformação no i-ésimo grupo do modelo.
        // Get and Setters
        QList<Vertice> getVertices();
        QList<Face> getFaces();
        QHash<QString,tnw::ColorMatrix> getPaleta();
        Vertice getPontoMedio(); // Retorna e atualiza o ponto médio
        Vertice getPontoMedio(int i); // Retorna o ponto médio do i-ésimo grupo
        // Construtores
        Model(QString pathname); // Construtor que recebe um arquivo OBJ.
        Model(QList<Vertice> vertices,QList<Face> faces); // Construtor que recebe uma lista de vertices e faces.
	};

}
#endif
