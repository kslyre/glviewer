#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "model.h"
#include <obj.h>

class ModelFactory
{
public:
    ModelFactory();

    Obj *createObj();
    Model *createEmptyModel();
    Model *createModel();
};

#endif // MODELFACTORY_H
