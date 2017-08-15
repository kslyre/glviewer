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

QList<QVector3D> Model::modifyVertexes(ProblemVector res)
{
    //Obj* objm = new Obj(obj->vertexes, obj->textures, obj->polygons);

    QList<QVector3D> list;
    double norm = qSqrt(qPow(res.params[0],2) +
            qPow(res.params[1],2) +
            qPow(res.params[2],2) +
            qPow(res.params[3],2));

    //if(norm*norm < 1.f)
    //    norm = 1.f;

    double w = res.params[0]/norm;
    double x = res.params[1]/norm;
    double y = res.params[2]/norm;
    double z = res.params[3]/norm;

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

        list.append(QVector3D(resv(0), resv(1), resv(2)));
    }

    //objm->getNormals();
    return list;
}

double Model::my_rand(int accuracy)
{
    double a = 0;
    a = (qrand() % int (qPow(10, accuracy) + 1))/qPow(10, accuracy);
    return a;
}
