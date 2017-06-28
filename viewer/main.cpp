#include <QApplication>
#include "widgettests.h"
#include "window.h"

void runTests()
{
    WidgetTests tests;
    QTest::qExec(&tests);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    runTests();

    Window w;
    w.show();

    return a.exec();
}
