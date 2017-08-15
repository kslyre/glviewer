#include "model.h"
#include <qmath.h>
#include <Eigen/Eigen>

using namespace Eigen;

Model::Model()
{

}

Model::Model(Obj *file)
{
    obj = file;
}

void Model::randomColor()
{
    color = QVector4D(my_rand(2),my_rand(2),my_rand(2),1);
}

Obj Model::modifyObj(ProblemVector res)
{
    Obj* objm = new Obj(obj->vertexes, obj->textures, obj->polygons);

    double w = res.params[0];
    double x = res.params[1];
    double y = res.params[2];
    double z = res.params[3];

    Matrix<double, 3,3> qm;
    qm << 1- 2*y*y- 2*z*z,
            2*x*y - 2*z*w,
            2*x*z + 2*y*w,

          2*x*y + 2*z*w,
            1- 2*x*x- 2*z*z,
            2*y*z - 2*x*w,

          2*x*z - 2*y*w,
            2*y*z + 2*x*w,
            1- 2*x*x- 2*y*y;

    Matrix<double, 3, 1> tv;
    tv << res.params[4], res.params[5], res.params[6];

    for(int i=0; i<obj->vertexes.length(); i++){
        Matrix<double, 3, 1> point2v;
        point2v << obj->vertexes[i].x(),
                obj->vertexes[i].y(),
                obj->vertexes[i].z();

        Matrix<double, 3, 1> resv = qm * point2v + tv;

        obj->vertexes[i].setX(resv(0));
        obj->vertexes[i].setY(resv(1));
        obj->vertexes[i].setZ(resv(2));
    }

    objm->getNormals();
}

double Model::my_rand(int accuracy)
{
    double a = 0;
    a = (qrand() % int (qPow(10, accuracy) + 1))/qPow(10, accuracy);
    return a;
}
