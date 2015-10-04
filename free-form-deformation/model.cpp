#include "model.h"
#include <cmath>
#include <iostream>

FFDGrid *Model::getGrid() const
{
    return grid;
}

void Model::setGrid(FFDGrid *value)
{
    grid = value;
}
Model::Model(const char *modelName, int ns, int nt, int nu)
{
    //Abre o modelo usando trimesh
    this->model = TriMesh::read(modelName);
    if (!model){
        std::cout << "Problemas para abrir o modelo!\n";
        exit(EXIT_FAILURE);
    }

    /*this->model->need_normals();
    this->model->need_tstrips();
    this->model->need_bbox();
    this->model->need_bsphere();

    //Descobre o maior x, maior y e maior z
    float max_x = 0, max_y = 0, max_z = 0;
    for (unsigned int i = 0; i < this->model->vertices.size(); i++){
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
    vec center = this->model->bbox.center();
    vec S(max_x,0,0);
    vec T(0,max_y,0);
    vec U(0,0,max_z);

    this->grid = new FFDGrid(center, S, T, U, ns, nt, nu);*/
}

void Model::drawModel()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(model->vertices[0]), &model->vertices[0][0]);

    if (!model->vertices.empty()){
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(model->normals[0]), &model->normals[0][0]);
    } else {
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    if (model->colors.empty()){
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, sizeof(model->colors[0]), &model->colors[0][0]);
    } else {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    if (model->tstrips.empty()){
        glDrawArrays(GL_POINTS, 0, model->vertices.size());
        return;
    } else {
        const int *t = &model->tstrips[0];
        const int *end = t + model->tstrips.size();
        while (t < end) {
            int striplen = *t++;
            glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_INT, t);
        }
    }


}

void Model::deformGrid()
{

}
