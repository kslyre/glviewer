#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "bvh.h"
#include <obj.h>
#include <QDebug>


class RayTrace
{
public:
    RayTrace();

    QVector<BVHstruct*> leafs;

    void trace(Obj *low, Obj *high, Obj *output, BVHstruct *bvh, int bvhDepth);

    void boxPolyIndices(QVector3D rayPos, QVector3D rayDir, BVHstruct *bvh);
    bool traceBox(QVector3D vertex, QVector3D normal, Box box);
    QVector3D interPoint(QVector3D a, QVector3D b, QVector3D c, QVector3D lowp, QVector3D lowv);
    double pointCheck(QVector3D a, QVector3D b, QVector3D c, QVector3D p);
    float triangleSquare(float a, float b, float c);
    double triangleDelta(QVector3D a, QVector3D b, QVector3D c, QVector3D p);
    bool traceBox2(QVector3D vertex, QVector3D normal, Box box);
private:
    int bvhDepth;       // tree depth
    void getBvhTraceBox(QVector3D rayPos, QVector3D rayDir, BVHstruct *bvh);
};

#endif // RAYTRACE_H
