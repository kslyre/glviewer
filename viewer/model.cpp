#include "model.h"
#include <qmath.h>

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

double Model::my_rand(int accuracy)
{
    double a = 0;
    a = (qrand() % int (qPow(10, accuracy) + 1))/qPow(10, accuracy);
    return a;
}
