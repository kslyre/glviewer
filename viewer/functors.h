#ifndef FUNCTORS_H
#define FUNCTORS_H

#include <Eigen/Eigen>
#include "derivable.h"
#include <QVector2D>
#include <QVector3D>
#include <structs.h>

using namespace Eigen;

class Functor
{
public:
    QVector<QVector3D> points1;
    QVector<QVector3D> points2;
    QVector3D center2;

    Functor();
    Functor(QVector<QVector3D> points1, QVector<QVector3D> point2, ProblemVector pv);

    int elems() { return 3; }
    int pointsCount() { return points1.length(); }
    int pvLength() const { return problemVector.params.length(); }

    ProblemVector problemVector;

    double func();
    double innerFunc();
    void center();
    double grad();
    QVector<Derivable> f(ProblemVector pv, int index, int indexParam) const;
    QVector3D func(ProblemVector pv) const;
    ProblemVector grad(int index, int indElem);
    QVector3D funcDeriv(int index);

    QVector<double> func(int index);
};

#endif // FUNCTORS_H
