#include "derivable.h"

#include <QVector>

Derivable::Derivable()
{

}

Derivable Derivable::operator+(const Derivable &f2) const
{
    return Derivable(val + f2.val, deriv + f2.deriv);
}

Derivable Derivable::operator-(const Derivable &f2) const
{
    return Derivable(val - f2.val, deriv - f2.deriv);
}

Derivable Derivable::operator*(const Derivable &f2) const
{
    return Derivable(val * f2.val, deriv * f2.val + val * f2.deriv);
}

QVector<Derivable> Derivable::operator*(const QVector<Derivable> &f2) const
{
    QVector<Derivable> res;
    foreach (Derivable d, f2) {
        res.append(Derivable(val * d.val, deriv * d.val + val * d.deriv));
    }
    return res;
}

Derivable Derivable::operator*(const float &f2) const
{
    return Derivable(val * f2, deriv * f2);
}

Derivable Derivable::operator/(const Derivable &f2) const
{
    return Derivable(val / f2.val, (deriv * f2.val - val * f2.deriv) / f2.val / f2.val);
}

Derivable Derivable::cos(Derivable f)
{
    return Derivable(qCos(f.val), -qSin(f.val)*f.deriv);
}

Derivable Derivable::sin(Derivable f)
{
    return Derivable(qSin(f.val), qCos(f.val)*f.deriv);
}

Derivable Derivable::pow(Derivable f, double val)
{
    return Derivable(qPow(f.val, val), val*qPow(f.val, val-1.f)*f.deriv);
}

Derivable Derivable::sqrt(Derivable f)
{
    return Derivable(qSqrt(f.val), 1.f/(2.f*qSqrt(f.val))*f.deriv);
}
