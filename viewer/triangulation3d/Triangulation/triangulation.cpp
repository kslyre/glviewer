#include "triangulation.h"

Triangulation::Triangulation()
{

}

void Triangulation::calculate(QVector<PolygonStruct> input, QVector<PolygonStruct> &output)
{
    foreach (PolygonStruct f, input) {
        Q_ASSERT(f.list.length() >= 3);
        if (f.list.length() == 3) {
            output.append(f);
            continue;
        }
        for(int i=1; i < f.list.length() - 1; i++){
            PolygonStruct poly;
            poly.list.append(f.list[0]);
            poly.list.append(f.list[i]);
            poly.list.append(f.list[i+1]);
            output.append(poly);
        }
    }
}
