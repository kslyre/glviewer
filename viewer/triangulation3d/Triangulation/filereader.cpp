#include "filereader.h"

FileReader::FileReader()
{

}

Obj *FileReader::createEmpty()
{
    QList<QVector3D> v;
    QList<QVector3D> vt;
    QList<PolygonStruct> p;
    return new Obj(v, vt, p);
}
