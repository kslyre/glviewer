#include "raytrace.h"

RayTrace::RayTrace()
{

}

void RayTrace::trace(Obj *low, Obj *high, Obj *output, BVHstruct *bvh, int bvhDepth)
{
    qInfo() << "tracing...";
    output->polygons = low->polygons;
    output->normals = low->normals;
    output->textures = low->textures;
    this->bvhDepth = bvhDepth;

    for(int vertex=0; vertex < low->vertexes.length(); vertex++){

        leafs.clear();
        // getting BVH's leafs
        boxPolyIndices(low->vertexes[vertex],
                       low->normals[vertex].toVector3D(),
                       bvh);

        QVector<QVector3D> tracedVertexes;

        foreach (BVHstruct* leaf, leafs) {
            foreach (int polygonIndex, leaf->polyIndexes){
                PolygonStruct poly = high->polygons[polygonIndex];
                QVector3D a = high->vertexes[poly.list[0].vertex];
                QVector3D b = high->vertexes[poly.list[1].vertex];
                QVector3D c = high->vertexes[poly.list[2].vertex];

                // distance filter
                if(low->vertexes[vertex].distanceToPoint(a) > 6)
                    continue;

                QVector3D ray = interPoint(a,b,c,low->vertexes[vertex],low->normals[vertex].toVector3D());

                double delta = triangleDelta(a,b,c,ray);
                if(qAbs(delta) < 10e-9){
                    tracedVertexes.append(ray);
                    break;
                }
            }
        }

        QVector3D resRay;
        double mindist = 999;
        foreach (QVector3D v, tracedVertexes) {
            double dist = low->vertexes[vertex].distanceToPoint(v);
            if(mindist > dist){
                mindist = dist;
                resRay = v;
            }          
        }
        if(tracedVertexes.length() == 0)
            resRay = low->vertexes[vertex];
        //if(vertex % 100 == 0)
        //    qInfo() << vertex;
        output->vertexes.append(resRay);
    }


    qInfo() << "vertexes: " << output->vertexes.length();
    output->getNormals();
    qInfo() << "trace ended";
}

// intersection point
QVector3D RayTrace::interPoint(QVector3D a, QVector3D b, QVector3D c, QVector3D lowp, QVector3D lowv)
{
    QVector3D v1 = c - a;
    QVector3D v2 = b - a;
    QVector3D plane = QVector3D::normal(v1, v2);
    if(qAbs(QVector3D::dotProduct(lowv, plane)) < 10e-12 && plane.length() > 0)
        return a;
    double denumenator = QVector3D::dotProduct(lowv, plane);
    double t = qAbs(denumenator) > 10e-12 ? QVector3D::dotProduct(a - lowp, plane)/denumenator : 0;
    return lowp + lowv*t;
}

// not using
double RayTrace::pointCheck(QVector3D a, QVector3D b, QVector3D c, QVector3D p)
{
    a = a - a;
    b = b - a;
    c = c - a;
    return 0;
}

double RayTrace::triangleDelta(QVector3D a, QVector3D b, QVector3D c, QVector3D p)
{
    QVector3D ab = b-a;
    QVector3D ac = c-a;
    QVector3D bc = c-b;

    QVector3D ap = p-a;
    QVector3D bp = p-b;

    return QVector3D::crossProduct(ab,ap).length()/2 +
           QVector3D::crossProduct(ac,ap).length()/2 +
           QVector3D::crossProduct(bc,bp).length()/2 -
           QVector3D::crossProduct(ac,ab).length()/2;
}

void RayTrace::boxPolyIndices(QVector3D rayPos, QVector3D rayDir, BVHstruct *bvh)
{
    BVHstruct *bt = bvh;
    getBvhTraceBox(rayPos, rayDir, bt);
}

// getting traced BVH's leafs
void RayTrace::getBvhTraceBox(QVector3D rayPos, QVector3D rayDir, BVHstruct *bvh)
{
    if(bvh->level == bvhDepth){
        leafs.append(bvh);
        return;
    }

    if(traceBox(rayPos, rayDir, bvh->left->bound) || traceBox(rayPos, -rayDir, bvh->left->bound))
        getBvhTraceBox(rayPos, rayDir, bvh->left);
    if(traceBox(rayPos, rayDir, bvh->right->bound) || traceBox(rayPos, -rayDir, bvh->right->bound))
        getBvhTraceBox(rayPos, rayDir, bvh->right);
}

// ray tracing throw box
bool RayTrace::traceBox(QVector3D vertex, QVector3D normal, Box box)
{
    if((vertex.x() >= box.min.x() && vertex.x() <= box.max.x()) &&
       (vertex.y() >= box.min.y() && vertex.y() <= box.max.y()) &&
       (vertex.z() >= box.min.z() && vertex.z() <= box.max.z()))
        return true;

    float tmin = 0.f;
    float tmax = 0.f;
    float lo = 1.0f/normal.x()*(box.min.x() - vertex.x());
    float hi = 1.0f/normal.x()*(box.max.x() - vertex.x());
    tmin = qMin(lo, hi);
    tmax = qMax(lo, hi);

    float lo1 = 1.0f/normal.y()*(box.min.y() - vertex.y());
    float hi1 = 1.0f/normal.y()*(box.max.y() - vertex.y());
    tmin  = qMax(tmin, qMin(lo1, hi1));
    tmax = qMin(tmax, qMax(lo1, hi1));

    float lo2 = 1.0f/normal.z()*(box.min.z() - vertex.z());
    float hi2 = 1.0f/normal.z()*(box.max.z() - vertex.z());
    tmin  = qMax(tmin, qMin(lo2, hi2));
    tmax = qMin(tmax, qMax(lo2, hi2));

    return (tmin <= tmax) && (tmax > 10e-14);
}
