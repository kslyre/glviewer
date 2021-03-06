#ifndef MODEL_H
#define MODEL_H

#include "bvh.h"
#include "kdtree.h"
#include "vbo.h"
#include <obj.h>

class Model
{
public:
    Model();
    Model(Obj *file);

    QString name = "";      // filename
    bool visible = true;    // visible in viewport
    QVector4D color;

    BVH bvh;
    VBO vbo;
    Obj *obj;
    KDTree kdt;

    void randomColor();
    //QVector<QVector3D> modifyVertexes(ProblemVector res);
private:
    double my_rand(int accuracy);
};

#endif // MODEL_H
