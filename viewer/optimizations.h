#ifndef OPTIMIZATIONS_H
#define OPTIMIZATIONS_H

#include "functors.h"

class Optimizations
{
public:
    Optimizations();
    ProblemVector gaussNewton(Functor functor);
};

#endif // OPTIMIZATIONS_H
