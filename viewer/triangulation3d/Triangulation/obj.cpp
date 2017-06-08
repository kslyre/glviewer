#include "obj.h"
#include <QDebug>

Obj::Obj(){

}

Obj::Obj(QList<QVector3D> vertexes, QList<QVector3D> textures, QList<PolygonStruct> polygons)
{
    this->vertexes = vertexes;
    this->textures = textures;
    this->polygons = polygons;
}

Obj::~Obj(){

}

void Obj::readObj(QTextStream &textStream)
{
    vertexes.clear();
    polygons.clear();

    while (!textStream.atEnd()){
        QString line = textStream.readLine();
        QStringList parsedLine = line.split(' ', QString::SkipEmptyParts);

        if(parsedLine.empty())
            continue;

        // vertexes reading
        bool isVertex = QString::compare(parsedLine.at(0), "v", Qt::CaseInsensitive) == 0;
        if(isVertex){
            //if(parsedLine.at(1).isEmpty())
            //    parsedLine.removeAt(1);
            vertexes.append(QVector3D(parsedLine.at(1).toFloat(),
                                      parsedLine.at(2).toFloat(),
                                      parsedLine.at(3).toFloat()));
        }

        // textures reading
        bool isTexture = QString::compare(parsedLine.at(0), "vt", Qt::CaseInsensitive) == 0;
        if(isTexture){
            textures.append(QVector3D(parsedLine.at(1).toFloat(),
                                      parsedLine.at(2).toFloat(),
                                      parsedLine.at(3).toFloat()));
        }

        // normals reading
        bool isNormal = QString::compare(parsedLine.at(0), "vn", Qt::CaseInsensitive) == 0;
        if(isNormal){
            normals.append(QVector3D(parsedLine.at(1).toFloat(),
                                     parsedLine.at(2).toFloat(),
                                     parsedLine.at(3).toFloat()).toVector4D());
        }

        // polygons reading
        bool isPolygon = QString::compare(parsedLine.at(0), "f", Qt::CaseInsensitive) == 0;
        if(isPolygon){
            PolygonStruct polygon;
            for(int i=1; i < parsedLine.length(); i++){
                line = parsedLine.at(i);
                QStringList polyLine = line.split('/');
                int vertex  = polyLine.at(0).toInt()-1;
                int texture = polyLine.length()==2 ? polyLine.at(1).toInt()-1 : 0;
                int normal  = polyLine.length()==3 ? polyLine.at(2).toInt()-1 : 0;
                polygon.polygon.append(PolyStruct(vertex, texture, normal));
            }
            polygons.append(polygon);
        } 
    }

    // normals calculate
    normals.clear();
    foreach (QVector3D v, vertexes) {
        normals.append(QVector3D(0,0,0).toVector4D());
    }

    foreach (PolygonStruct f, polygons) {
        int count = f.polygon.length();
        for (int i=0; i<count; i++){
            QVector3D a = i == 0 ?
                        vertexes[f.polygon[count-1].vertex] - vertexes[f.polygon[i].vertex] :
                    vertexes[f.polygon[i-1].vertex] - vertexes[f.polygon[i].vertex] ;
            QVector3D b = i == count-1 ?
                        vertexes[f.polygon[0].vertex] - vertexes[f.polygon[i].vertex] :
                    vertexes[f.polygon[i+1].vertex] - vertexes[f.polygon[i].vertex] ;
            normals[f.polygon[i].vertex] += QVector3D::normal(a, b).toVector4D();
            normals[f.polygon[i].vertex].setW(normals[f.polygon[i].vertex].w()+1);
        }
    }
    for(int i=0; i<normals.length(); i++){
        normals[i] /= normals[i].w();
    }
//    foreach (QVector4D n, normals) {
//        n = n / n.w();
//    }
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
        foreach (PolyStruct p, f.polygon) {
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

