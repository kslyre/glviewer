#include "obj.h"
#include <QDebug>

Obj::Obj(){

}

Obj::Obj(QVector<QVector3D> vertexes, QVector<QVector3D> textures, QVector<PolygonStruct> polygons)
{
    this->vertexes = vertexes;
    this->textures = textures;
    this->polygons = polygons;
}

Obj::~Obj(){

}

bool Obj::readObj(QTextStream &textStream)
{
    vertexes.clear();
    polygons.clear();
    int maxVertexInPolygon = 0;
    int maxTextureInPolygon = 0;
    bool polyWithTexture = false;
    bool parseOk = true;

    while (!textStream.atEnd()){
        QString line = textStream.readLine();
        QStringList parsedLine = line.split(' ', QString::SkipEmptyParts);

        if(parsedLine.empty())
            continue;

        bool okx, oky, okz;
        // vertexes reading
        bool isVertex = QString::compare(parsedLine.at(0), "v", Qt::CaseInsensitive) == 0;
        if(isVertex){
            QVector3D vertex = QVector3D(parsedLine.at(1).toFloat(&okx),
                                         parsedLine.at(2).toFloat(&oky),
                                         parsedLine.at(3).toFloat(&okz));

            if(!(parseOk = okx && oky && okz))
                break;

            vertexes.append(vertex);
            proceedBound(vertex, size);
        }

        // textures reading
        bool isTexture = QString::compare(parsedLine.at(0), "vt", Qt::CaseInsensitive) == 0;
        if(isTexture){
            textures.append(QVector3D(parsedLine.at(1).toFloat(&okx),
                                      parsedLine.at(2).toFloat(&oky),
                                      parsedLine.length() > 3 ? parsedLine.at(3).toFloat(&okz) : 1));

            if(!(parseOk = okx && oky && okz))
                break;
        }

        // normals reading
        bool isNormal = QString::compare(parsedLine.at(0), "vn", Qt::CaseInsensitive) == 0;
        if(isNormal){
            normals.append(QVector3D(parsedLine.at(1).toFloat(&okx),
                                     parsedLine.at(2).toFloat(&oky),
                                     parsedLine.at(3).toFloat(&okz)).toVector4D());

            if(!(parseOk = okx && oky && okz))
                break;
        }

        // polygons reading
        bool isPolygon = QString::compare(parsedLine.at(0), "f", Qt::CaseInsensitive) == 0;
        if(isPolygon){
            PolygonStruct polygon;
            for(int i=1; i < parsedLine.length(); i++){
                QString polyLine = parsedLine.at(i);
                QStringList polyVertexLine = polyLine.split('/');
                int vertex  = polyVertexLine.at(0).toInt(&okx)-1;
                int texture = polyVertexLine.length()==2 ? polyVertexLine.at(1).toInt(&oky)-1 : 0;
                int normal  = polyVertexLine.length()==3 ? polyVertexLine.at(2).toInt(&okz)-1 : 0;

                if(!(parseOk = okx && oky && okz))
                    break;

                polygon.list.append(PolygonVertex(vertex, texture, normal));
                if(polyVertexLine.length()==2)
                    polyWithTexture = true;
                if(vertex > maxVertexInPolygon)
                    maxVertexInPolygon = vertex;
                if(texture > maxTextureInPolygon)
                    maxTextureInPolygon = texture;
            }
            polygons.append(polygon);          
        } 
    }
    if(vertexes.length() == 0 || polygons.length() == 0 ||
       //(textures.length() == 0 && polyWithTexture) ||
       !parseOk ||
       maxVertexInPolygon+1 != vertexes.length())// ||
       //maxTextureInPolygon+1 != textures.length())
            return false;


    getNormals();
    return true;
}

void Obj::writeObj(QTextStream &textStream) const
{
    // vertexes writing
    foreach (QVector3D v, vertexes) {
        textStream << "v  " << v.x()
                   << " "   << v.y()
                   << " "   << v.z() << endl;
    }
    textStream << "# " << vertexes.length() << " vertices" << endl << endl;

    // textures writing
    foreach (QVector3D vt, textures) {
        textStream << "vt " << vt.x()
                   << " "   << vt.y()
                   << " "   << vt.z() << endl;
    }
    textStream << "# " << textures.length() << " texture coords" << endl << endl;

    // polygons writing
    foreach (PolygonStruct f, polygons) {
        textStream << "f ";
        foreach (PolygonVertex p, f.list) {
            textStream << p.vertex+1;
            if(p.texture != 0)
                textStream << "/" << p.texture+1;
            if(p.normal  != 0)
                textStream << "/" << p.normal+1;
            textStream << " ";
        }
        textStream << endl;
    }
    textStream << "# " << polygons.length() << " polygons" << endl;
}

void Obj::getNormals()
{
    // normals calculate
    normals.clear();
    foreach (QVector3D v, vertexes) {
        normals.append(QVector3D(0,0,0).toVector4D());
    }

    foreach (PolygonStruct f, polygons) {
        int count = f.list.length();
        for (int i=0; i<count; i++){
            QVector3D a = i == 0 ?
                        vertexes[f.list[count-1].vertex] - vertexes[f.list[i].vertex] :
                    vertexes[f.list[i-1].vertex] - vertexes[f.list[i].vertex] ;
            QVector3D b = i == count-1 ?
                        vertexes[f.list[0].vertex] - vertexes[f.list[i].vertex] :
                    vertexes[f.list[i+1].vertex] - vertexes[f.list[i].vertex] ;
            normals[f.list[i].vertex] += QVector3D::normal(a, b).toVector4D();
            normals[f.list[i].vertex].setW(normals[f.list[i].vertex].w()+1);
        }
    }
    for(int i=0; i<normals.length(); i++){
        normals[i] /= normals[i].w();
    }
}

void Obj::getBounds(QVector<QVector3D> vertexes, QVector<PolygonStruct> polygons)
{
    for(int poly=0; poly<polygons.length(); poly++){
    //foreach (PolygonStruct f, polygons){
        Box bound;
        //bound.polyIndex = poly;
        foreach (PolygonVertex p, polygons[poly].list) {
            QVector3D v = vertexes[p.vertex];

            if (v.x() < bound.min.x())
                bound.min.setX(v.x());
            if (v.y() < bound.min.y())
                bound.min.setY(v.y());
            if (v.z() < bound.min.z())
                bound.min.setZ(v.z());
            if (v.x() > bound.max.x())
                bound.max.setX(v.x());
            if (v.y() > bound.max.y())
                bound.max.setY(v.y());
            if (v.z() > bound.max.z())
                bound.max.setZ(v.z());
        }
        //polyBounds.append(bound);
    }
}

void Obj::proceedBound(QVector3D vertex, Box &box)
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
