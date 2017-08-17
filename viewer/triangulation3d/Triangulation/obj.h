#ifndef FILEOBJ_H
#define FILEOBJ_H

#include <file.h>
#include <QVector3D>
#include <QVector4D>
#include <structs.h>
#include <QTextStream>

class Obj : public File
{
public:
    Obj();
    Obj(QVector<QVector3D> vertexes, QVector<QVector3D> textures, QVector<PolygonStruct> polygons);
    ~Obj();

    QVector<QVector3D> vertexes;
    QVector<QVector3D> textures;
    QVector<QVector4D> normals;
    QVector<PolygonStruct> polygons;

    Box size;

    //QList<Box> polyBounds;

    bool readObj(QTextStream &textStream);
    void writeObj(QTextStream &textStream) const;
    void getNormals();
    void getBounds(QVector<QVector3D> vertexes, QVector<PolygonStruct> polygons);

    void proceedBound(QVector3D vertex, Box &box);
private:
    void checkSize(QVector3D vertex);
};

#endif // FILEOBJ_H
