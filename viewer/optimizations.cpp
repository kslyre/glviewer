#include "optimizations.h"
#include <QtTest>

Optimizations::Optimizations()
{

}

MatrixXf Optimizations::jacobian(Functor functor)
{
    int pointsLength = functor.pointsCount();
    int pvLength = functor.pvLength();
    int elems = functor.elems();

    MatrixXf j(elems*pointsLength, pvLength);

    for(int index=0; index < pointsLength; index++){
        for(int indElem = 0; indElem < elems; indElem++){
            ProblemVector pv = functor.grad(index, indElem);
            for(int indParam = 0; indParam < pvLength; indParam++){
                j(elems*index + indElem, indParam) = pv.params[indParam];
            }
        }
    }
    return j;
}

VectorXf Optimizations::innerFunc(Functor functor)
{
    int pointsLength = functor.pointsCount();
    int elems = functor.elems();

    VectorXf f(elems*pointsLength);
    for(int index=0; index < pointsLength; index++){
        QVector<double> funv = functor.func(index);
        for(int indElem = 0; indElem < elems; indElem++){
            f(elems*index + indElem) = funv[indElem];
        }
    }
    return f;
}

ProblemVector Optimizations::gaussNewton(Functor functor)
{
    bool go = functor.probVector.goNext;

    MatrixXf j = jacobian(functor);
    VectorXf f = innerFunc(functor);

    MatrixXf b = j.transpose()*f;
    MatrixXf a = j.transpose()*j;

    MatrixXf x = a.llt().solve(-b);

    QVector<double> vector = { x(0), x(1), x(2), x(3), x(4), x(5), x(6) };
    ProblemVector add = ProblemVector(vector);
    ProblemVector nextIter = functor.probVector + add;
    qDebug() << ProblemVector(vector).params;
    double err = (functor.func(nextIter) - functor.func(functor.probVector)).length();
    qDebug() << functor.func(nextIter) << err << "|" << add.Length();
    go = (err > 1e-6) & (qAbs(nextIter.Length() - functor.probVector.Length()) > 1e-5);
    //qDebug() << (nextIter - functor.probVector).Length();
    //min = err < 1e-5;

    if(err != err) {
        go = false;
    }
    //if(nextIter.params[0] > 1e+6)
    //    go = false;
    //if(nextIter.Length() < functor.probVector.Length())
    //    go = false;
    if(go)
        functor.probVector = nextIter;

    functor.probVector.goNext = go;
    return functor.probVector;
}
