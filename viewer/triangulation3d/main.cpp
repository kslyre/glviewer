#include <QCoreApplication>

//#include <filereader.h>
#include <QFile>
#include <triangulation.h>
#include "triangulationtests.h"

void runTests()
{
    TriangulationTests tests;
    QTest::qExec(&tests);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    runTests();

    Obj *inputObj = new Obj();
    QFile file(qApp->applicationDirPath() + "/WrapHead.obj");
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QTextStream textStream(&file);

    inputObj->readObj(textStream);
    file.close();

    Obj *outputObj = new Obj();
    Triangulation triang;
    triang.calculate(inputObj->polygons, outputObj->polygons);

    outputObj->vertexes = inputObj->vertexes;
    outputObj->textures = inputObj->textures;

    QFile file1(qApp->applicationDirPath() + "/WrapHead_tr.obj");
    if(!file1.open(QIODevice::WriteOnly))
        return false;
    QTextStream textStream1(&file1);
    outputObj->writeObj(textStream1);
    file1.close();

    //return a.exec();
}
