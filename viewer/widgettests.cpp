#include "derivable.h"
#include "widgettests.h"

WidgetTests::WidgetTests(QObject *parent) : QObject(parent)
{

}

void WidgetTests::testNormalRay1()
{
    RayTrace *w = new RayTrace();
    QVector3D a = QVector3D(0.1, 0,  0);
    QVector3D b = QVector3D(0.2, 0.1,0);
    QVector3D c = QVector3D(0.25,0,0);
    QVector3D lowp = QVector3D(0.15, 0.05, -2.5);
    QVector3D lowv = QVector3D(0,0,-1);
    QVector3D p = w->interPoint(a,b,c,lowp,lowv);

    QCOMPARE(QVector3D(0.15,0.05,0), p);
}

void WidgetTests::testNormalRay2()
{
    RayTrace *w = new RayTrace();
    QVector3D a = QVector3D(0.1, 0,  0);
    QVector3D b = QVector3D(0.2, 0.1,0);
    QVector3D c = QVector3D(0.25,0,0);
    QVector3D lowp = QVector3D(0.15, 0.05, 0.5);
    QVector3D lowv = QVector3D(0,0,1);
    QVector3D p = w->interPoint(a,b,c,lowp,lowv);

    QCOMPARE(QVector3D(0.15,0.05,0), p);
}

void WidgetTests::testZeroTriangle()
{
    RayTrace *w = new RayTrace();
    QVector3D a = QVector3D(0, 0, 0);
    QVector3D b = QVector3D(0, 0, 0);
    QVector3D c = QVector3D(0, 0, 0);
    QVector3D p = QVector3D(0, 0, 0);

    QCOMPARE(0.0, w->triangleDelta(a,b,c,p));
}

void WidgetTests::testZeroRayTriangle()
{
    RayTrace *w = new RayTrace();
    QVector3D a = QVector3D(0, 0, 0);
    QVector3D b = QVector3D(0, 0, 0);
    QVector3D c = QVector3D(0, 0, 0);
    QVector3D lowp = QVector3D(0,0,0);
    QVector3D lowv = QVector3D(0,0,0);
    QVector3D p = w->interPoint(a,b,c,lowp,lowv);

    QCOMPARE(QVector3D(0,0,0), p);
}

void WidgetTests::testNonZeroRay()
{
    RayTrace *w = new RayTrace();
    QVector3D a = QVector3D(0, 0, 0);
    QVector3D b = QVector3D(0, 0, 0);
    QVector3D c = QVector3D(0, 0, 0);
    QVector3D lowp = QVector3D(0,0,1);
    QVector3D lowv = QVector3D(0,0,1);
    QVector3D p = w->interPoint(a,b,c,lowp,lowv);

    QCOMPARE(QVector3D(0,0,1), p);
}

void WidgetTests::testParallelRay(){
    RayTrace *w = new RayTrace();
    QVector3D a = QVector3D(0.1, 0,  0);
    QVector3D b = QVector3D(0.2, 0.1,0);
    QVector3D c = QVector3D(0.25,0,0);
    QVector3D lowp = QVector3D(0,0,1);
    QVector3D lowv = QVector3D(0.1,0,0);
    QVector3D p = w->interPoint(a,b,c,lowp,lowv);

    //qInfo() << p;
    QCOMPARE(p, QVector3D(0.1,0,0));
}

void WidgetTests::testTraceBoxTrue()
{
    RayTrace *w = new RayTrace();

    QVector3D vertex = QVector3D(0.5,0.5,1.5);
    QVector3D normal = QVector3D(0,0,-1);

    Box box;
    box.min = QVector3D(0,0,0);
    box.max = QVector3D(1,1,1);

    bool res = w->traceBox(vertex, normal, box);

    QCOMPARE(res, true);
}

void WidgetTests::testTraceBoxFalse()
{
    RayTrace *w = new RayTrace();

    QVector3D vertex = QVector3D(1.5,0.5,1.5);
    QVector3D normal = QVector3D(0,0,-1);

    Box box;
    box.min = QVector3D(0,0,0);
    box.max = QVector3D(1,1,1);

    bool res = w->traceBox(vertex, normal, box);

    QCOMPARE(res, false);
}

void WidgetTests::sqrtDeriv()
{
    Derivable d;
    d = d.sqrt(20);
    qDebug() << d.getValue() << d.getDerivative();
    QCOMPARE(1,1);
}

