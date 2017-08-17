#include "kdtree.h"

KDTstruct::KDTstruct(int level)
{
    this->level = level;
}

KDTree::KDTree()
{


}

void KDTree::buildKDTree(QVector<QVector3D> list)
{
    qDebug() << "KDtree building";
    root = new KDTstruct(0);

    this->list = list;

    proceedLeaf(root);

    qDebug() << "# done / kd";
}

void KDTree::proceedLeaf(KDTstruct *leaf)
{
    leaf->left = new KDTstruct(leaf->level+1);
    leaf->right = new KDTstruct(leaf->level+1);

    proceedLeaf(leaf->left);
    proceedLeaf(leaf->right);
}
