#ifndef STRUCTS_H
#define STRUCTS_H

#include <QList>

struct PolygonVertex
{
    int vertex;
    int texture;
    int normal;

    PolygonVertex(int v, int vt=0, int n=0){
        vertex = v;
        texture = vt;
        normal = n;
    }

    inline bool operator== (const PolygonVertex &p) const
    {
        return ( (vertex == p.vertex) &
                 (texture == p.texture) &
                 (normal == p.normal) );
    }
};

struct PolygonStruct
{
    QList<PolygonVertex> list;

    inline bool operator== (const PolygonStruct &p) const
    {
        return ( list == p.list );
    }
};

struct VertexStruct
{
    QVector3D vertex;
    QVector3D normal;
    QVector2D texture;

    VertexStruct(){

    }

    VertexStruct(QVector3D v, QVector3D n, QVector2D t){
        v = vertex;
        n = normal;
        t = texture;
    }
};

struct Box
{
    QVector3D min = QVector3D(999,999,999);
    QVector3D max = QVector3D(-999,-999,-999);
    int polyIndex = 0;
};

enum Axis
{
    X, Y, Z
};

struct Divider
{
    Axis axis;
    double value;   // median value

    Divider(Axis a, double v){
        axis = a;
        value = v;
    }
};

#endif // STRUCTS_H
