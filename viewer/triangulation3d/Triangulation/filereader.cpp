#include "filereader.h"

FileReader::FileReader()
{

}

Obj *FileReader::createEmpty()
{
    QVector<QVector3D> v;
    QVector<QVector3D> vt;
    QVector<PolygonStruct> p;
    return new Obj(v, vt, p);
}
