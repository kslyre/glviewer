#ifndef KDTREE_H
#define KDTREE_H

#include <QVector3D>
#include <QtTest>
#include <structs.h>

class KDTstruct
{
public:
    KDTstruct();
    KDTstruct(int level);

    int level;

    KDTstruct *left;
    KDTstruct *right;

    Divider divine;
    Box bound;

    QVector<QVector3D> vertexes;
};


class KDTree
{
public:
    KDTree();

    KDTstruct *root;
    QVector<QVector3D> list;
    int kdtDepth;

    QVector<KDTstruct*> kdtleafs;

    void buildKDTree(QVector<QVector3D> list);
    void proceedLeaf(KDTstruct *leaf);
    QVector3D getClosestPoint(QVector3D initial);
    KDTstruct *getLeaf(KDTstruct *node, QVector3D vertex);
private:
    void getLeafStructs(KDTstruct *node);
    void proceedBound(QVector3D vertex, Box &box);
    Divider getDivider(Box &box);
    int getVertexSide(Divider divine, Box bound, QVector3D vertex);
};

#endif // KDTREE_H
