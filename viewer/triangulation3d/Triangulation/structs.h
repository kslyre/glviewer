#ifndef STRUCTS_H
#define STRUCTS_H

#endif // STRUCTS_H

#include <QList>

struct PolyStruct
{
    int vertex;
    int texture;
    int normal;

    PolyStruct(int v, int vt=0, int n=0){
        vertex = v;
        texture = vt;
        normal = n;
    }

    inline bool operator== (const PolyStruct &p) const
    {
        return ( (vertex == p.vertex) &
                 (texture == p.texture) &
                 (normal == p.normal) );
    }
};

struct PolygonStruct
{
    QList<PolyStruct> polygon;

    inline bool operator== (const PolygonStruct &p) const
    {
        return ( polygon == p.polygon );
    }
};
