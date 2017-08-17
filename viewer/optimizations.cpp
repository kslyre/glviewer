#include "optimizations.h"
#include <QtTest>

Optimizations::Optimizations()
{

}

QVector<QVector3D> Optimizations::modifyVertexes(QVector<QVector3D> vertexes, ProblemVector res)
{
    QVector<QVector3D> list;
    double norm = qSqrt(qPow(res.params[0],2) +
            qPow(res.params[1],2) +
            qPow(res.params[2],2) +
            qPow(res.params[3],2));

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

    for(int i=0; i<vertexes.length(); i++){
        Matrix<double, 3, 1> point2v;
        point2v << vertexes[i].x(),
                vertexes[i].y(),
                vertexes[i].z();

        Matrix<double, 3, 1> resv = qm * point2v + tv;

        list.append(QVector3D(resv(0), resv(1), resv(2)));
    }

    return list;
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

    MatrixXf x = a.colPivHouseholderQr().solve(-b);

    QVector<double> vector = { x(0), x(1), x(2), x(3), x(4), x(5), x(6) };
    ProblemVector add = ProblemVector(vector);
    ProblemVector nextIter = functor.probVector + add;
    qDebug() << ProblemVector(vector).params;
    double err = (functor.func(nextIter) - functor.func(functor.probVector)).length();
    qDebug() << functor.func(nextIter) << err << "|" << add.Length();
    go = (err > 1e-5) & (qAbs(nextIter.Length() - functor.probVector.Length()) > 1e-6);
    //qDebug() << (nextIter - functor.probVector).Length();
    //min = err < 1e-5;

    if(err != err) {
        go = false;
    }
    //if(nextIter.params[0] > 1e+6)
    //    go = false;
    //if(nextIter.Length() < functor.probVector.Length())
    //    go = false;
    functor.probVector = nextIter;

    functor.probVector.goNext = go;
    return functor.probVector;
}
