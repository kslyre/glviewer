#include "obj.h"

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

