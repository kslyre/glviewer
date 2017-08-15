#include "functors.h"

Functor::Functor()
{

}

Functor::Functor(QVector<QVector3D> point1, QVector<QVector3D> point2, ProblemVector pv)
{
    this->points1 = point1;
    this->points2 = point2;
    this->probVector = pv;
    center();
}

QVector3D Functor::func(ProblemVector pv)
{
    QVector<Derivable> resDeriv = f(pv, -1, -1);
    QVector3D res = { resDeriv[0].getValue(), resDeriv[1].getValue(), resDeriv[2].getValue() };
    return res;
}

QVector<double> Functor::func(int index)
{
    QVector<Derivable> resDeriv = f(probVector, index, -1);
    QVector<double> res;
    res.append(resDeriv[0].getValue());
    res.append(resDeriv[1].getValue());
    res.append(resDeriv[2].getValue());
    return res;
}

ProblemVector Functor::grad(int index, int indElem)
{
    QVector<double> v = {0,0,0,0,0,0,0};
    ProblemVector res = ProblemVector(v);
    for(int i=0; i<pvLength(); i++) {
        res.params[i] = f(probVector, index, i)[indElem].getDerivative();
    }
    return res;
}

QVector3D Functor::funcDeriv(int index)
{
    QVector<Derivable> resDeriv = f(probVector, index, -1);
    QVector3D res = { resDeriv[0].getDerivative(), resDeriv[1].getDerivative(), resDeriv[2].getDerivative() };
    return res;
}

QVector<Derivable> Functor::f(ProblemVector pv, int index, int indexParam)
{
    QVector<Derivable> pvD;

    double norm1 = qSqrt(qPow(pv.params[0],2) +
            qPow(pv.params[1],2) +
            qPow(pv.params[2],2) +
            qPow(pv.params[3],2));
    //if(norm1*norm1 < 1.f)
    //    norm1 = 1.f;

    for(int i=0; i<pvLength(); i++){
        Derivable xD;
        double value = pv.params[i];
        if(i < 4)
            value /= norm1;

        if(i == indexParam)
            xD = Derivable::IndependendVariable(value);
        else
            xD = Derivable(value);

        pvD.append(xD);
    }

    Derivable w = pvD[0];

    Derivable x = pvD[1];
    Derivable y = pvD[2];
    Derivable z = pvD[3];

    /*Derivable norm = w.sqrt(w.pow(w, 2) + w.pow(x,2) + w.pow(y,2) + w.pow(z, 2));

    w = w / norm;
    x = x / norm;
    y = y / norm;
    z = z / norm;*/

    Matrix<Derivable, 3,3> qm;
    qm << Derivable(1)- Derivable(2)*y*y- Derivable(2)*z*z,
            Derivable(2)*x*y - Derivable(2)*z*w,
            Derivable(2)*x*z + Derivable(2)*y*w,

          Derivable(2)*x*y + Derivable(2)*z*w,
            Derivable(1)- Derivable(2)*x*x- Derivable(2)*z*z,
            Derivable(2)*y*z - Derivable(2)*x*w,

          Derivable(2)*x*z - Derivable(2)*y*w,
            Derivable(2)*y*z + Derivable(2)*x*w,
            Derivable(1)- Derivable(2)*x*x- Derivable(2)*y*y;

    Matrix<Derivable, 3, 1> tv;
    tv << pvD[4], pvD[5], pvD[6];

    Matrix<Derivable, 3, 1> resv;


    if(index != -1){
        Matrix<Derivable, 3, 1> point2v;
        point2v << Derivable(points2[index].x()),
                Derivable(points2[index].y()),
                Derivable(points2[index].z());
        Matrix<Derivable, 3, 1> point1v;
        point1v << Derivable(points1[index].x()),
                Derivable(points1[index].y()),
                Derivable(points1[index].z());

        resv = qm * point2v + tv - point1v;
    } else {
        for(int i=0; i<points2.length(); i++){
            Matrix<Derivable, 3, 1> point2v;
            point2v << Derivable(points2[i].x()),
                    Derivable(points2[i].y()),
                    Derivable(points2[i].z());
            Matrix<Derivable, 3, 1> point1v;
            point1v << Derivable(points1[i].x()),
                    Derivable(points1[i].y()),
                    Derivable(points1[i].z());

            resv = resv + qm * point2v + tv - point1v;
        }
    }

    QVector<Derivable> res;
    res.append(resv[0]);
    res.append(resv[1]);
    res.append(resv[2]);
    return res;
}


void Functor::center()
{
    QVector3D res = QVector3D(0,0,0);
    foreach (QVector3D p, points2) {
        res += p;
    }
    center2 = res / points2.length();
}

