#include "model.h"
#include <cmath>

Model::Model(std::string modelName, int ns, int nt, int nu)
{
    //Abre o modelo usando trimesh
    this->model = TriMesh::read(modelName);

    //Descobre o maior x, maior y e maior z
    float max_x = 0, max_y = 0, max_z = 0;
    for (int i = 0; i < this->model->vertices.size(); i++){
        vec v = this->model->vertices[i];
        if (fabs(v[0]) > max_x){
            max_x = fabs(v[0]);
        }
        if (fabs(v[1]) > max_y){
            max_x = fabs(v[0]);
        }
        if (fabs(v[2]) > max_z){
            max_x = fabs(v[0]);
        }
    }

    //Descobre o ponto central do mesh
    this->model->need_bbox();
    vec center = this->model->bbox.center();
    vec S(max_x,0,0);
    vec T(0,max_y,0);
    vec U(0,0,max_z);

    this->grid = new FFDGrid(center, S, T, U, ns, nt, nu);
}

void Model::draw()
{

}

void Model::deformGrid()
{

}
