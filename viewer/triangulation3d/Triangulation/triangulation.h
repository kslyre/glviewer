#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <obj.h>

class Triangulation
{
public:
    Triangulation();

    void calculate(QList<PolygonStruct> input, QList<PolygonStruct> &output);
private:

};

#endif // TRIANGULATION_H
