#include "modelfactory.h"

ModelFactory::ModelFactory()
{

}

Obj *ModelFactory::createObj()
{
    return new Obj();
}

Model *ModelFactory::createEmptyModel()
{
    return new Model();
}

Model *ModelFactory::createModel()
{
    return new Model(createObj());
}
