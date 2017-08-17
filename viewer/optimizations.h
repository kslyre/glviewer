#ifndef OPTIMIZATIONS_H
#define OPTIMIZATIONS_H

#include "functors.h"

class Optimizations
{
public:
    Optimizations();
    ProblemVector gaussNewton(Functor functor);
    MatrixXf jacobian(Functor functor);
    VectorXf innerFunc(Functor functor);
    QVector<QVector3D> modifyVertexes(QVector<QVector3D> vertexes, ProblemVector res);
};

#endif // OPTIMIZATIONS_H
