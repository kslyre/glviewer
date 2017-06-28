#include "bvh.h"

BVHstruct::BVHstruct()
{

}

BVHstruct::BVHstruct(int lvl)
{
    level = lvl;
}

BVH::BVH()
{

}

void BVH::buildBVH(Obj *model)
{
    qInfo() << "BVH building...";
    rootBVH = new BVHstruct(0);
    bvhDepth = 12;

    // root indices filling
    for(int polygonIndex=0; polygonIndex < model->polygons.length(); polygonIndex++){
        rootBVH->polyIndexes.append(polygonIndex);
    }
    rootBVH->bound = model->size;

    proceedBVHleaf(rootBVH, model);
    qInfo() << "# done";
}

void BVH::proceedBVHleaf(BVHstruct *leaf, Obj *model)
{
    // if leaf is reached
    if(leaf->level > bvhDepth-1)
        return;

    leaf->left = new BVHstruct(leaf->level+1);
    leaf->right = new BVHstruct(leaf->level+1);
    Divider divine = getDivider(leaf->bound);   // getting median divider

    for(int leafIndex=0; leafIndex < leaf->polyIndexes.length(); leafIndex++){
        int polygonIndex = leaf->polyIndexes[leafIndex];

        // get polygon side relatively to divider
        int res = getPolySide(divine, leaf->bound,
                              QList<QVector3D>()
                              <<  model->vertexes[model->polygons[polygonIndex].list[0].vertex]
                              <<  model->vertexes[model->polygons[polygonIndex].list[1].vertex]
                              <<  model->vertexes[model->polygons[polygonIndex].list[2].vertex]
                );
        PolygonStruct polygon = model->polygons[polygonIndex];

        // if res == 0 - to left side
        // if res == 3 - to right side
        // otherwise   - to both sides
        if(res == 0 || res != 3)
        {
            leaf->left->polyIndexes.append(polygonIndex);
            proceedBound(model->vertexes[polygon.list[0].vertex], leaf->left->bound);
            proceedBound(model->vertexes[polygon.list[1].vertex], leaf->left->bound);
            proceedBound(model->vertexes[polygon.list[2].vertex], leaf->left->bound);
        }
        if(res == 3 || res != 0)
        {
            leaf->right->polyIndexes.append(polygonIndex);
            proceedBound(model->vertexes[polygon.list[0].vertex], leaf->right->bound);
            proceedBound(model->vertexes[polygon.list[1].vertex], leaf->right->bound);
            proceedBound(model->vertexes[polygon.list[2].vertex], leaf->right->bound);
        }
    }

    proceedBVHleaf(leaf->left, model);
    proceedBVHleaf(leaf->right, model);
}

int BVH::getPolySide(Divider divine, Box bound, QList<QVector3D> vertexes)
{
    int sum = 0;
    foreach (QVector3D v, vertexes){
        switch (divine.axis) {
        case X:
            if(v.x() > bound.min.x()+divine.value)
                sum++;
            break;
        case Y:
            if(v.y() > bound.min.y()+divine.value)
                sum++;
            break;
        case Z:
            if(v.z() > bound.min.z()+divine.value)
                sum++;
            break;
        default:
            break;
        }
    }
    return sum;
}

Divider BVH::getDivider(Box &box)
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
void BVH::proceedBound(QVector3D vertex, Box &box)
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
