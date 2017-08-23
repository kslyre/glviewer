#include "optimizations.h"
#include "widget.h"

Widget::Widget(QGLWidget *parent)
    : QGLWidget(parent)
{
    zoom = 5.f;
    //glFunctions = new QOpenGLFunctions();
    //glFunctions->initializeOpenGLFunctions();
    makeCurrent();
    shader = new QOpenGLShaderProgram();
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex,":/shader.vert"))
        qFatal("Can\'t compile vertex shader");
    if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment,":/shader.frag"))
        qFatal("Can\'t compile fragment shader");
    if (!shader->link()) {
        qFatal("Can\'t link shader");
    }
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
   //qglClearColor(Qt::white); // заполняем экран белым цветом
   //glEnable(GL_DEPTH_TEST); // задаем глубину проверки пикселей
   //glShadeModel(GL_FLAT); // убираем режим сглаживания цветов
   //glEnable(GL_CULL_FACE); // говорим, что будем строить только внешние поверхности
   //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // фигуры будут закрашены с обеих сторон

    rotate.setToIdentity();
    translate.setToIdentity();
    zoomMatrix.setToIdentity();
}

void Widget::resizeGL(int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);

    float aspectRatio = nWidth / float(nHeight);

    camera.setProjection(80, aspectRatio, 1.f, 100.f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(camera.projection.constData());
}

void Widget::paintGL()
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    zoomMatrix.setToIdentity();
    zoomMatrix.translate(0,0,-zoom);
    QMatrix4x4 modelview = zoomMatrix * rotate * translate;

    // http://www.gamedev.ru/code/terms/Shading
    foreach (Model* model, models) {
        if(!model->visible)
            continue;

        shader->bind();
        //if(model->obj->textures.length() > 0)
        //    model->vbo.texture->bind();
        shader->setUniformValue("modelViewMatrix", modelview);
        shader->setUniformValue("projectionMatrix",camera.projection);
        //if(model->obj->textures.length() > 0)
        //    shader->setUniformValue("texture", model->vbo.texture);
        shader->setUniformValue("color", model->color);
        shader->setUniformValue("light", QVector3D(0,0,1));
        shader->enableAttributeArray("vertex");
        shader->enableAttributeArray("normal");
        shader->enableAttributeArray("v_texcoord");

        model->vbo.vertexBuffer.bind();

        // triangles
        model->vbo.indexBuffer.bind();
        int polygonsCount = model->obj->polygons.count()*3;
        shader->setAttributeBuffer("vertex",GL_FLOAT,0,3,sizeof(VertexStruct));
        shader->setAttributeBuffer("normal",GL_FLOAT,sizeof(QVector3D),3,sizeof(VertexStruct));
        shader->setAttributeBuffer("v_texcoord",GL_FLOAT,2*sizeof(QVector3D),2,sizeof(VertexStruct));
        shader->setAttributeValue( "view",  zoomMatrix.inverted()*QVector4D(0,0,0,1));
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);
        glDrawElements(GL_TRIANGLES, polygonsCount, GL_UNSIGNED_INT, 0);
        glDisable(GL_POLYGON_OFFSET_FILL);
        model->vbo.indexBuffer.release();

        // wireframe
        model->vbo.indexLineBuffer.bind();
        shader->setUniformValue("color",QVector4D(0.1,0.1,0.1,0.5));
        shader->setAttributeBuffer("vertex",GL_FLOAT,0,3,sizeof(VertexStruct));
        glDrawElements(GL_LINES, polygonsCount*2, GL_UNSIGNED_INT, 0);
        model->vbo.indexLineBuffer.release();

        model->vbo.vertexBuffer.release();

        shader->disableAttributeArray("v_texcoord");
        shader->disableAttributeArray("normal");
        shader->disableAttributeArray("vertex");
        //if(model->obj->textures.length() > 0)
        //    model->vbo.texture->release();
        shader->release();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->localPos().x() - pos.x();
    int dy = event->localPos().y() - pos.y();

    float angle = 1.f;

    if (event->buttons() & Qt::LeftButton){
        rotate.rotate(angle*dx, 0, rotate(1,1), 0);
        rotate.rotate(angle*dy, rotate(0,0), 0, rotate(0,2));
    }

    double delta = zoom/1000.f;
    if (event->buttons() & Qt::MiddleButton){
        translate.translate(dx*delta*rotate(0,0), 0, dx*delta*rotate(0,2));
        translate.translate(0, -dy*delta, 0);
    }

    pos = event->localPos();
    update();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
   pos = event->localPos();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    double delta = zoom/50.f;
    zoom += event->delta() < 0 ? delta : -delta;
    update();
}

void Widget::fitToView(bool simple)
{
    QVector3D vmin = QVector3D(999,999,999);
    QVector3D vmax = QVector3D(-999,-999,-999);

    foreach (Model* model, models) {
        if(!model->visible)
            continue;
        if(model->obj->size.min.x() < vmin.x())
            vmin.setX(model->obj->size.min.x());
        if(model->obj->size.min.y() < vmin.y())
            vmin.setY(model->obj->size.min.y());
        if(model->obj->size.min.z() < vmin.z())
            vmin.setZ(model->obj->size.min.z());
        if(model->obj->size.max.x() > vmax.x())
            vmax.setX(model->obj->size.max.x());
        if(model->obj->size.max.y() > vmax.y())
            vmax.setY(model->obj->size.max.y());
        if(model->obj->size.max.z() > vmax.z())
            vmax.setZ(model->obj->size.max.z());
    }
    QVector3D center = (vmax + vmin)/2;
    translate.setToIdentity();
    translate.translate(-center);
    float radius = center.distanceToPoint(vmax);


    if(simple){
        zoom = radius / qSin(40); //coef + nearf;
        update();
        return;
    }

    zoom = radius / qSin(40);
    float nearf = camera.nearField;
    QVector3D cameraPos = QVector3D(0,0,zoom);
    QVector3D cameraNFPos = QVector3D(0,0,zoom-nearf);
    QVector3D NFPosY = QVector3D(0, 1/qCos(qDegreesToRadians(camera.fov/2))*nearf, cameraNFPos.z());

    QVector3D fovLineVector = NFPosY - cameraPos;
    //QVector3D rv = tn - center;
    //float rd = QVector3D::crossProduct(rv, tv).length()/tv.length();

    float r01 = QVector3D::dotProduct((NFPosY - center), fovLineVector)/fovLineVector.length();
    QVector3D prevFovPoint = NFPosY - r01/fovLineVector.length()*fovLineVector;
    QVector3D radiusFovPoint = (prevFovPoint-center).normalized()*radius;
    float coef; // = radius / ; // else - revert and -res
    float distance = (radius - (radiusFovPoint-prevFovPoint).length());// / zoom;

    coef = distance / radius;
    zoom = coef + nearf;
    qInfo() << zoom << distance << radius << coef;
    update();
}

void Widget::resetView()
{
    rotate.setToIdentity();
    translate.setToIdentity();
    zoomMatrix.setToIdentity();
    zoom = 5.f;
    zoomMatrix.translate(0,0,-zoom);
    fitToView(true);
    update();
}

// add model to list
Model* Widget::addModel()
{
    models.append(factory.createModel());
    return models.last();
}

void Widget::clearScene()
{
    models.clear();
    resetView();
}

void Widget::modelProjection()
{
    addModel();
    rayTrace.trace(models[0]->obj, models[1]->obj, models[2]->obj,
            models[1]->bvh.rootBVH, models[1]->bvh.bvhDepth);

    models[2]->obj->textures = models[0]->obj->textures;
    models[2]->obj->polygons = models[0]->obj->polygons;
    models[2]->randomColor();
    models[2]->vbo.loadVBO(models[2]->obj);
    models[2]->bvh.buildBVH(models[2]->obj);
}

void Widget::gaussNewton()
{
    addModel();   
    models[2]->obj->textures = models[1]->obj->textures;
    models[2]->obj->polygons = models[1]->obj->polygons;
    models[2]->randomColor();
    Optimizations opt;

    QVector<double> vector = { 1,0,0,0,0,0,0 };
    ProblemVector pv = ProblemVector(vector);
    Functor sf = Functor(models[0]->obj->vertexes,
                         models[1]->obj->vertexes, pv);

    int num = 0;
    while(pv.goNext && num < 50){
        qDebug() << num++;

        sf.problemVector = pv;
        pv = opt.gaussNewton(sf);
        double norm = qSqrt(qPow(pv.params[0],2) +
                qPow(pv.params[1],2) +
                qPow(pv.params[2],2) +
                qPow(pv.params[3],2));

        pv.params[0] /= norm;
        pv.params[1] /= norm;
        pv.params[2] /= norm;
        pv.params[3] /= norm;

        // apply mod to points
        models[2]->obj->vertexes = opt.modifyVertexes(models[1]->obj->vertexes, pv);
        models[2]->obj->getNormals();
        models[2]->vbo.loadVBO(models[2]->obj);
        this->update();

        QApplication::processEvents();
        QThread::msleep(5);      
    }
    qDebug() << "res:  " << pv.params;


    models[2]->bvh.buildBVH(models[2]->obj);
    update();
}

void Widget::icp()
{
    addModel();
    models[2]->obj->vertexes = models[1]->obj->vertexes;
    models[2]->obj->textures = models[1]->obj->textures;
    models[2]->obj->polygons = models[1]->obj->polygons;
    models[2]->randomColor();
    Optimizations opt;
    QVector<QVector3D> fig2;

    QVector<double> vector = { 1,0,0,0,0,0,0 };
    ProblemVector problemVector = ProblemVector(vector);
    Functor functor = Functor(models[0]->obj->vertexes,
                         fig2, problemVector);

    int num = 0;
    while(problemVector.goNext && num < 50){
        qDebug() << " ";
        qDebug() << num++;

        fig2 = models[2]->obj->vertexes;

        QVector<QVector3D> fig3;
        models[2]->kdt.buildKDTree(fig2);
        // foreach point find nearest point
        foreach(QVector3D p1, models[0]->obj->vertexes){
            QVector3D nearest = models[2]->kdt.getClosestPoint(p1);
            fig3.append(nearest);
        }
        functor.points2 = fig3;

        for(int i=0; i<2; i++){
            functor.problemVector = problemVector;
            problemVector = opt.gaussNewton(functor);

            double norm = qSqrt(qPow(problemVector.params[0],2) +
                    qPow(problemVector.params[1],2) +
                    qPow(problemVector.params[2],2) +
                    qPow(problemVector.params[3],2));

            problemVector.params[0] /= norm;
            problemVector.params[1] /= norm;
            problemVector.params[2] /= norm;
            problemVector.params[3] /= norm;
            functor.problemVector = problemVector;
        }

        // apply mod to points
        models[2]->obj->vertexes = opt.modifyVertexes(fig2, problemVector);
        models[2]->obj->getNormals();
        models[2]->vbo.loadVBO(models[2]->obj);
        this->update();

        QApplication::processEvents();
        QThread::msleep(5);
    }
    qDebug() << "res:  " << problemVector.params;

    models[2]->bvh.buildBVH(models[2]->obj);
    update();
}
