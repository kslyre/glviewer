#ifndef TRIANGULATIONTESTS_H
#define TRIANGULATIONTESTS_H

#include "triangulation.h"
#include <QTest>

class TriangulationTests : public QObject
{
    Q_OBJECT
public:
    explicit TriangulationTests(QObject *parent = 0);
private slots:
    void testTriangle01();
    void testTriangle02();
};

#endif // TRIANGULATIONTESTS_H
