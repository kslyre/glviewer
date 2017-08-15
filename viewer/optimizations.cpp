#include "optimizations.h"

Optimizations::Optimizations()
{

}

ProblemVector Optimizations::gaussNewton(Functor functor)
{
    bool min = functor.probVector.goNext;

    MatrixXf j = jacobian(functor);
    VectorXf f = innerFunc(functor);

    MatrixXf b = j.transpose()*f;
    MatrixXf a = j.transpose()*j;

    MatrixXf x = a.llt().solve(-b);

    QVector<double> vector = { x(0), x(1), x(2), x(3), x(4), x(5), x(6)};
    ProblemVector nextIter = functor.probVector + ProblemVector(vector);
    double err = qAbs(functor.func(nextIter).distanceToPoint(functor.func(functor.probVector)));
    min = err < 1e-5;

    if(err != err)
    {
        min = true;
    }
    functor.probVector = nextIter;
    if(ops >= 100)
        min = true;

    functor.probVector.goNext = min;
}
