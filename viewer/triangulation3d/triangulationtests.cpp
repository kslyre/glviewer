#include "triangulationtests.h"

TriangulationTests::TriangulationTests(QObject *parent) : QObject(parent)
{

}

void TriangulationTests::testTriangle01()
{
    //QCOMPARE(2+2,4);
    //QVERIFY(2+2==4);

    QList<Polygon> input;
    Polygon p;
    p.polygon.append(Poly(1));
    p.polygon.append(Poly(2));
    p.polygon.append(Poly(3));
    p.polygon.append(Poly(4));
    input.append(p);

    QList<Polygon> expected;
    Polygon p1;
    p1.polygon.append(Poly(1));
    p1.polygon.append(Poly(2));
    p1.polygon.append(Poly(3));
    expected.append(p1);
    Polygon p2;
    p2.polygon.append(Poly(1));
    p2.polygon.append(Poly(3));
    p2.polygon.append(Poly(4));
    expected.append(p2);

    QList<Polygon> output;
    Triangulation triang;
    triang.calculate(input, output);

    QCOMPARE(output, expected);
}

void TriangulationTests::testTriangle02()
{
    QList<Polygon> input;
    Polygon p;
    p.polygon.append(Poly(1));
    p.polygon.append(Poly(2));
    p.polygon.append(Poly(3));
    input.append(p);

    QList<Polygon> expected;
    Polygon p1;
    p1.polygon.append(Poly(1));
    p1.polygon.append(Poly(2));
    p1.polygon.append(Poly(3));
    expected.append(p1);

    QList<Polygon> output;
    Triangulation triang;
    triang.calculate(input, output);

    QCOMPARE(output, expected);
}

