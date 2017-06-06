#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include "widget.h"
#include <obj.h>
#include <triangulation.h>

class Window : public QWidget
{
    Q_OBJECT
public:
    Window();

signals:

public slots:

private:
    Widget *glwidget;
    //Obj *model;

    QPushButton *createButton(QString text);
    bool openFile();
};

#endif // WINDOW_H
