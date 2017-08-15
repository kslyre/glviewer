#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QListWidget>
#include <QErrorMessage>
#include <QGridLayout>
#include <QMessageBox>
#include "widget.h"
#include <obj.h>
#include <triangulation.h>

enum ERRORS
{
    NO_ERRORS,
    CANT_OPEN_FILE,
    BAD_OBJ_FILE,
    MODELS_COUNT
};


class Window : public QWidget
{
    Q_OBJECT
public:
    Window();

signals:

private slots:
    void openFileButton();
    void saveFileButton();
    void clearViewport();
    void perfectFit();
    void projection();
    void gaussNewton();
    void listClick(QListWidgetItem *item);
private:
    Widget *glwidget;
    QListWidget *modelList;

    QPushButton *createButton(QString text);
    ERRORS openFile(Model *model, QString filepath = nullptr);
    ERRORS writeFile(Obj *model);
    bool showErrorMessage(ERRORS error);
};

#endif // WINDOW_H
