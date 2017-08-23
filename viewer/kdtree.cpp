#include "kdtree.h"

#include <structs.h>

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
    kdtDepth = 12;
    //list.clear();   //

    root->vertexes = list;
    foreach (QVector3D vertex, list) {
        proceedBound(vertex, root->bound);
    }

    proceedLeaf(root);

    kdtleafs.clear();
    getLeafStructs(root);

    qDebug() << "# done / kd";
}

void KDTree::proceedLeaf(KDTstruct *leaf)
{
    if(leaf->level > kdtDepth-1)
        return;

    leaf->left = new KDTstruct(leaf->level+1);
    leaf->right = new KDTstruct(leaf->level+1);

    Divider divine = getDivider(leaf->bound);
    leaf->divine = divine;

    foreach (QVector3D vertex, leaf->vertexes) {
        int vertSide = getVertexSide(divine, leaf->bound, vertex);

        if(vertSide==0){
            leaf->left->vertexes.append(vertex);
            proceedBound(vertex, leaf->left->bound);
        } else {
            leaf->right->vertexes.append(vertex);
            proceedBound(vertex, leaf->right->bound);
        }
    }

    proceedLeaf(leaf->left);
    proceedLeaf(leaf->right);
}

int KDTree::getVertexSide(Divider divine, Box bound, QVector3D vertex)
{
    int sum = 0;

    switch (divine.axis) {
    case X:
        if(vertex.x() > bound.min.x()+divine.value)
            sum++;
        break;
    case Y:
        if(vertex.y() > bound.min.y()+divine.value)
            sum++;
        break;
    case Z:
        if(vertex.z() > bound.min.z()+divine.value)
            sum++;
        break;
    default:
        break;
    }

    return sum;
}

Divider KDTree::getDivider(Box &box)
{
    QVector3D size = box.max - box.min;

    if(size.x() > size.y() && size.x() > size.z())
        return Divider(X, size.x()/2.0);
    else
        if(size.y() > size.x() && size.y() > size.z())
            return Divider(Y, size.y()/2.0);
        else
            return Divider(Z, size.z()/2.0);
}

// box resizing by vertex adding
void KDTree::proceedBound(QVector3D vertex, Box &box)
{
    if (vertex.x() < box.min.x())
        box.min.setX(vertex.x());
    if (vertex.y() < box.min.y())
        box.min.setY(vertex.y());
    if (vertex.z() < box.min.z())
        box.min.setZ(vertex.z());

    if (vertex.x() > box.max.x())
        box.max.setX(vertex.x());
    if (vertex.y() > box.max.y())
        box.max.setY(vertex.y());
    if (vertex.z() > box.max.z())
        box.max.setZ(vertex.z());
}

QVector3D KDTree::getClosestPoint(QVector3D initial)
{
    int nearStructIndex = 0;
    double nearDist = 9999;



    for(int i=0; i<kdtleafs.length(); i++){
        Box box = kdtleafs[i]->bound;
        QVector3D half = (box.max - box.min)/2;
        QVector3D kdtCenter = box.min + half;
        double distance = initial.distanceToPoint(kdtCenter);
        if(distance < nearDist){
            nearDist = distance;
            nearStructIndex = i;
        }
    }

    QVector3D nearest = QVector3D(999,999,999);
    foreach (QVector3D vertex, kdtleafs[nearStructIndex]->vertexes) {
        if((vertex-initial).length() < (nearest-initial).length())
            nearest = vertex;
    }
    return nearest;
}


void KDTree::getLeafStructs(KDTstruct *node)
{
    if(node->level == kdtDepth || (node->left->vertexes.length()  == 0 ||
                                   node->right->vertexes.length() == 0 )){
        kdtleafs.append(node);
        return;
    }

    getLeafStructs(node->left);
    getLeafStructs(node->right);
}

KDTstruct* KDTree::getLeaf(KDTstruct *node, QVector3D vertex)
{
    if(node->level == kdtDepth || (node->left->vertexes.length()  == 0 ||
                                   node->right->vertexes.length() == 0 )){
        return node;
    }

    return node;
}
