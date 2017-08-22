#ifndef BVH_H
#define BVH_H

#include <obj.h>
#include <structs.h>
#include <QDebug>

class BVHstruct
{
public:
    BVHstruct();
    BVHstruct(int lvl);

    int level;      // tree level

    BVHstruct *left;
    BVHstruct *right;

    Box bound;


    QVector<int> polyIndexes;
};

class BVH
{
public:
    BVH();

    BVHstruct *rootBVH;
    int bvhDepth;   // tree depth

    void buildBVH(Obj *model);
private:
    void proceedBVHleaf(BVHstruct *leaf, Obj *model);
    Divider getDivider(Box &box);
    void proceedBound(QVector3D vertex, Box &box);
    int getPolySide(Divider divine, Box bound, QVector<QVector3D> vertexes);
};

#endif // BVH_H
