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
    void test01();

signals:

public slots:
};

#endif // WIDGETTESTS_H