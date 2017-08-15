#ifndef DERIVABLE_H
#define DERIVABLE_H

#include <QtMath>

class Derivable
{
    double val;
    double deriv;

public:
    Derivable();

    Derivable(double _val, double _deriv) : val(_val), deriv(_deriv) {}
    Derivable(double c): val(c), deriv(0) {}
    Derivable(int c): val(c), deriv(c) {}
    static Derivable IndependendVariable(double x) { return Derivable(x,1); }
    static Derivable NoVariable(double x) { return Derivable(x,0); }

    double getValue() const {return val;}
    double getDerivative() const {return deriv;}

    //inline ProblemVector operator +(const ProblemVector &v) const
    Derivable operator+(const Derivable& f2) const;
    Derivable operator-(const Derivable& f2) const;
    Derivable operator*(const Derivable& f2) const;
    Derivable operator/(const Derivable& f2) const;
    Derivable cos(Derivable f);
    Derivable sin(Derivable f);
    Derivable pow(Derivable f, double val);

    Derivable operator *(const float &f2) const;
    QVector<Derivable> operator *(const QVector<Derivable> &f2) const;
};

#endif // DERIVABLE_H
