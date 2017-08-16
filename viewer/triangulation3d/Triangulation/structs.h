#ifndef STRUCTS_H
#define STRUCTS_H

#include <QList>
#include <QtMath>

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


class ProblemVector
{
public:
    QVector<double> params;
    bool goNext = true;

    ProblemVector()
    { }

    ProblemVector(QVector<double> vector)
    {
        params = vector;
    }

    ProblemVector(double value, int length)
    {
        for(int i=0; i<length; i++){
            params.append(value);
        }
    }

    inline ProblemVector operator +(const ProblemVector &v) const
    {
        QVector<double> res = {
            params[0] + v.params[0],
                            params[1] + v.params[1],
                            params[2] + v.params[2],
                            params[3] + v.params[3],
                            params[4] + v.params[4],
                            params[5] + v.params[5],
                            params[6] + v.params[6]
        };
        return ProblemVector(res);
    }

    inline ProblemVector operator -(const ProblemVector &v) const
    {
        QVector<double> res = {
            params[0] - v.params[0],
                            params[1] - v.params[1],
                            params[2] - v.params[2],
                            params[3] - v.params[3],
                            params[4] - v.params[4],
                            params[5] - v.params[5],
                            params[6] - v.params[6]
        };
        return ProblemVector(res);
    }

    double Length(){
        double res = 0;
        foreach (double d, params) {
            res += d*d;
        }
        return qSqrt(res);
    }
};

#endif // STRUCTS_H
