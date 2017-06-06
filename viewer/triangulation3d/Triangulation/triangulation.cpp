#include "triangulation.h"

Triangulation::Triangulation()
{

}

void Triangulation::calculate(QList<PolygonStruct> input, QList<PolygonStruct> &output)
{
    foreach (PolygonStruct f, input) {
        Q_ASSERT(f.polygon.length() >= 3);
        if (f.polygon.length() == 3) {
            output.append(f);
            continue;
        }
        for(int i=1; i < f.polygon.length() - 1; i++){
            PolygonStruct poly;
            poly.polygon.append(f.polygon[0]);
            poly.polygon.append(f.polygon[i]);
            poly.polygon.append(f.polygon[i+1]);
            output.append(poly);
        }
    }
}
