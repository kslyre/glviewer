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
    Obj(QList<QVector3D> vertexes, QList<QVector3D> textures, QList<PolygonStruct> polygons);
    ~Obj();

    QList<QVector3D> vertexes;
    QList<QVector3D> textures;
    QList<QVector4D> normals;
    QList<PolygonStruct> polygons;

    Box size;

    //QList<Box> polyBounds;

    bool readObj(QTextStream &textStream);
    void writeObj(QTextStream &textStream) const;
    void getNormals();
    void getBounds(QList<QVector3D> vertexes, QList<PolygonStruct> polygons);

    void proceedBound(QVector3D vertex, Box &box);
private:
    void checkSize(QVector3D vertex);
};

#endif // FILEOBJ_H
