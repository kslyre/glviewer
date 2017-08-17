#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <obj.h>

class Triangulation
{
public:
    Triangulation();

    void calculate(QVector<PolygonStruct> input, QVector<PolygonStruct> &output);
private:

};

#endif // TRIANGULATION_H
