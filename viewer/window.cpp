#include "window.h"
#include <QGridLayout>

Window::Window()
{

    setWindowTitle(tr("Model Viewer"));
    resize(1000, 900);

    glwidget = new Widget();
    openFile();
    QPushButton *openButton = createButton(tr("Open"));
    QPushButton *resetButton = createButton(tr("Reset"));
    QPushButton *fitButton = createButton(tr("Perfect Fit"));
    connect(openButton, &QPushButton::clicked, [=](){ openFile(); });
    connect(resetButton, &QPushButton::clicked, [=](){ glwidget->resetScene(); });
    connect(fitButton, &QPushButton::clicked, [=](){ glwidget->fitToView(false); });

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(openButton, 0,0, 1,3);
    layout->addWidget(resetButton, 0,3, 1,3);
    layout->addWidget(fitButton, 0,6, 1,3);
    layout->addWidget(glwidget,   1,0, 20,20);

    layout->setMargin(0);
    //layout->setColumnStretch(0, 70);
    //layout->setColumnStretch(1, 1);
    setLayout(layout);
}

QPushButton *Window::createButton(QString text)
{
    QPushButton* b = new QPushButton;
    b->setText(text);
    b->setMinimumSize(80,40);

    return b;
}

bool Window::openFile()
{

    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open model file"),
                                                    QDir::currentPath(),
                                                    tr("Model Files (*.obj)"));

    Obj *inputObj = new Obj();
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QTextStream textStream(&file);

    inputObj->readObj(textStream);
    file.close();

    glwidget->model = new Obj();
    Triangulation triang;
    triang.calculate(inputObj->polygons, glwidget->model->polygons);

    glwidget->model->vertexes = inputObj->vertexes;
    glwidget->model->textures = inputObj->textures;
    glwidget->loadVBO();
    glwidget->fitToView(true);

    return true;
}
