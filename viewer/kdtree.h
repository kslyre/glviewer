#ifndef KDTREE_H
#define KDTREE_H

#include <QVector3D>
#include <QtTest>

class KDTstruct
{
public:
    KDTstruct();
    KDTstruct(int level);

    int level;

    KDTstruct *left;
    KDTstruct *right;

    QVector3D value;
};


class KDTree
{
public:
    KDTree();

    KDTstruct *root;
    QVector<QVector3D> list;

    void buildKDTree(QVector<QVector3D> list);
    void proceedLeaf(KDTstruct *leaf);
};

#endif // KDTREE_H
