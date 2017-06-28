#ifndef WIDGETTESTS_H
#define WIDGETTESTS_H

#include "widget.h"
#include <QTest>

class WidgetTests : public QObject
{
    Q_OBJECT
public:
    explicit WidgetTests(QObject *parent = 0);
private slots:
    void testNormalRay1();

    void testNormalRay2();
    void testZeroTriangle();
    void testZeroRayTriangle();
    void testNonZeroRay();
    void testParallelRay();
    void testTraceBoxTrue();
    void testTraceBoxFalse();
signals:

public slots:
};

#endif // WIDGETTESTS_H
