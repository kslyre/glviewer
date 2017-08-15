#include "window.h"

Window::Window()
{
    setWindowTitle(tr("Model Viewer"));
    resize(1200, 900);

    glwidget = new Widget();

    qInfo() << QDir::currentPath();

    QPushButton *openButton = createButton(tr("Open"));
    QPushButton *saveButton = createButton(tr("Save"));
    QPushButton *clearButton = createButton(tr("Clear"));
    QPushButton *resetButton = createButton(tr("Reset"));
    QPushButton *fitButton = createButton(tr("Perfect Fit"));
    QPushButton *projButton = createButton(tr("Projection"));
    QPushButton *icpButton = createButton(tr("ICP"));
    modelList = new QListWidget();

    connect(openButton,  &QPushButton::clicked, this, &Window::openFileButton);
    connect(saveButton,  &QPushButton::clicked, this, &Window::saveFileButton);
    connect(clearButton, &QPushButton::clicked, this, &Window::clearViewport);
    connect(resetButton, &QPushButton::clicked, glwidget, &Widget::resetView);
    connect(fitButton,   &QPushButton::clicked, this, &Window::perfectFit);
    connect(projButton,  &QPushButton::clicked, this, &Window::projection);
    connect(icpButton,  &QPushButton::clicked, this, &Window::gaussNewton);
    connect(modelList,   &QListWidget::itemClicked, this, &Window::listClick);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(openButton, 0,0, 1,3);
    layout->addWidget(saveButton, 0,3, 1,3);
    layout->addWidget(clearButton, 0,6, 1,3);
    layout->addWidget(resetButton, 0,9, 1,3);
    layout->addWidget(fitButton, 0,12, 1,3);
    layout->addWidget(projButton, 0,16, 1,3);
    layout->addWidget(icpButton, 0,19, 1,3);
    layout->addWidget(glwidget,   1,0, 20,20);
    layout->addWidget(modelList,  1,20, 8,2);

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

ERRORS Window::openFile(Model *model, QString filepath)
{
    QString filename;
    if(filepath.isNull())
        filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open model file"),
                                                    QDir::currentPath(),
                                                    tr("Model Files (*.obj)"));
    else
        filename = filepath;

    qInfo() << "file opening...";

    Obj *inputObj = new Obj();
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return ERRORS::CANT_OPEN_FILE;
    QTextStream textStream(&file);

    if(!inputObj->readObj(textStream)){
        file.close();
        return ERRORS::BAD_OBJ_FILE;
    }
    file.close();

    Triangulation triang;
    triang.calculate(inputObj->polygons, model->obj->polygons);

    model->obj->vertexes = inputObj->vertexes;
    model->obj->textures = inputObj->textures;
    model->obj->normals = inputObj->normals;
    model->obj->size = inputObj->size;
    model->randomColor();

    model->obj->getBounds(model->obj->vertexes, model->obj->polygons);
    model->name = QFileInfo(file).fileName();

    qInfo() << "# done";

    return ERRORS::NO_ERRORS;
}


ERRORS Window::writeFile(Obj *model){
    QString filename;
    filename = QFileDialog::getSaveFileName(this,
                                            tr("Save model file"),
                                            QDir::currentPath(),
                                            tr("Model Files (*.obj)"));
    if(filename.length() < 1)
        return ERRORS::NO_ERRORS;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
        return ERRORS::CANT_OPEN_FILE;
    QTextStream textStream(&file);
    model->writeObj(textStream);
    file.close();
    qInfo() << "saved!";

    return ERRORS::NO_ERRORS;
}

bool Window::showErrorMessage(ERRORS error)
{
    QString errtext;
    switch (error) {
    case ERRORS::CANT_OPEN_FILE:
        errtext = tr("Can't open file!");
        break;
    case ERRORS::BAD_OBJ_FILE:
        errtext = tr("Obj file is bad!");
        break;
    case ERRORS::MODELS_COUNT:
        errtext = tr("You should open exactly 2 models!");
        break;
    default:
        return false;
        break;
    }
    QMessageBox msg;
    msg.setText(errtext);
    msg.exec();
    return true;
}

void Window::openFileButton()
{
    ModelFactory f;
    Model* model = f.createModel();
    ERRORS res;
    if((res = openFile(model)) == ERRORS::NO_ERRORS){
        glwidget->models.append(model);
        model->vbo.loadVBO(model->obj);
        model->bvh.buildBVH(model->obj);

        QListWidgetItem* item = new QListWidgetItem(model->name, modelList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        glwidget->fitToView(true);
    }
    else {
        showErrorMessage(res);
    }
}

void Window::saveFileButton()
{
    if(modelList->count() < 1)
        return;
    ERRORS res;
    if((res = writeFile(glwidget->models[modelList->currentRow()]->obj)) != ERRORS::NO_ERRORS);
        showErrorMessage(res);
}

void Window::clearViewport()
{
    glwidget->clearScene();
    modelList->clear();
}

void Window::perfectFit()
{
    glwidget->fitToView(false);
}

void Window::projection()
{
    if(glwidget->models.count() == 2){
        glwidget->modelProjection();

        QListWidgetItem* item = new QListWidgetItem("proj", modelList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }
    else {
        showErrorMessage(ERRORS::MODELS_COUNT);
    }
}

void Window::listClick(QListWidgetItem* item)
{
    QListWidget *temp = new QListWidget();
    temp = item->listWidget();
    if(temp->row(item) >= 0){
        bool checked = item->checkState() == Qt::Checked;
        glwidget->models[temp->row(item)]->visible = checked;
        glwidget->update();
    }
}

void Window::gaussNewton()
{
    if(glwidget->models.count() == 2){
        glwidget->gaussNewton();

        QListWidgetItem* item = new QListWidgetItem("icp", modelList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }
    else {
        showErrorMessage(ERRORS::MODELS_COUNT);
    }
}
