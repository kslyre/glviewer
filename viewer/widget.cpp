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
    //loadVBO();
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
    //QMatrix4x4 projection;
    //projection.ortho(-1 * aspectRatio, 1 * aspectRatio, -1, 1, -20.f, 100.f);
    //projection.frustum(-1,1,-1,1,0.1,100);
    //projection.perspective(80, aspectRatio, 1.f, 100.f);

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

    if(model != NULL){
    shader->bind();
    shader->setUniformValue("modelViewMatrix", modelview);
    shader->setUniformValue("projectionMatrix",camera.projection);
    shader->setUniformValue("color", QVector4D(0.5,0.5,1,1));
    shader->setUniformValue("light", QVector3D(0,0,50));
    shader->enableAttributeArray("vertex");
    shader->enableAttributeArray("normal");

    vertexBuffer.bind();

    indexBuffer.bind(); 
    int polygonsCount =model->polygons.count()*3;
    shader->setAttributeBuffer("vertex",GL_FLOAT,0,3,6*sizeof(GLfloat));
    shader->setAttributeBuffer("normal",GL_FLOAT,3*sizeof(GLfloat),3,6*sizeof(GLfloat));
    shader->setAttributeValue( "view",  modelview.inverted()*QVector4D(0,0,0,1));
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);
    glDrawElements(GL_TRIANGLES, polygonsCount, GL_UNSIGNED_INT, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
    indexBuffer.release();

//    indexLineBuffer.bind();
//    shader->setUniformValue("color",QVector4D(0.9,0.9,0.9,1));
//    shader->setAttributeBuffer("vertex",GL_FLOAT,0,3,6*sizeof(GLuint));
//    glDrawElements(GL_LINES, polygonsCount*2, GL_UNSIGNED_INT, 0);
//    indexLineBuffer.release();

    vertexBuffer.release();

    shader->disableAttributeArray("normal");
    shader->disableAttributeArray("vertex");
    shader->release();
    }


    //glDrawElements(GL_TRIANGLES, model->polygons.count()*3, 0, indexBuffer);
    //glDrawArrays();


    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    zoomMatrix.setToIdentity();
    zoomMatrix.translate(0,0,-zoom);
    QMatrix4x4 modelview = zoomMatrix * rotate * translate;
    glLoadMatrixf(modelview.constData());
    //qInfo() << camera;

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистка экрана
    glColor4f(0,0,0,0.5);*/
    // grid
//    glBegin(GL_LINES);
//    int size = 1;
//    for(int i=-3; i<=3; i++)
//    {
//        glVertex3f(3,0,i*size);
//        glVertex3f(-3,0,i*size);
//        glVertex3f(i*size,0,3);
//        glVertex3f(i*size,0,-3);
//    }
//    glEnd();

    /*if(model != NULL){
        glColor4f(0.4, 0.4, 1, 1);
        glBegin(GL_TRIANGLES);
        foreach (PolygonStruct ps, model->polygons) {
            foreach (PolyStruct p, ps.polygon) {
                QVector3D point = model->vertexes[p.vertex-1];
                glVertex3f(point.x(), point.y(), point.z());
            }
        }
        glEnd();
    } else {
        // triangle
        glColor4f(0.4, 0.4, 1, 1);
        glBegin(GL_TRIANGLES);
         glVertex3f(0, 0, 1);
         glVertex3f(0, 1, 1);
         glVertex3f(1, 0, 1);
        glEnd();
    }*/
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

    if (event->buttons() & Qt::MiddleButton){
        translate.translate(dx*0.01f*rotate(0,0), 0, dx*0.01f*rotate(0,2));
        translate.translate(0, -dy*0.001f, 0);
    }

    pos = event->localPos();
    update();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
   pos = event->localPos();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{

}

void Widget::wheelEvent(QWheelEvent *event)
{
    zoom += event->delta() < 0 ? 0.25 : -0.25;
    update();
}

void Widget::fitToView(bool simple)
{
    QVector3D vmin = QVector3D(0,0,0);
    QVector3D vmax = QVector3D(0,0,0);

    foreach (QVector3D v, model->vertexes) {
        if (v.x() < vmin.x())
            vmin.setX(v.x());
        if (v.y() < vmin.y())
            vmin.setY(v.y());
        if (v.z() < vmin.z())
            vmin.setZ(v.z());
        if (v.x() > vmax.x())
            vmax.setX(v.x());
        if (v.y() > vmax.y())
            vmax.setY(v.y());
        if (v.z() > vmax.z())
            vmax.setZ(v.z());
    }
    QVector3D center = (vmax + vmin)/2;
    translate.translate(-center);
    float radius = center.distanceToPoint(vmax);


    if(simple){
        zoom = radius / qSin(40); //coef + nearf;
        update();
        return;
    }

    float nearf = 1.f;
    QVector3D cameraPos = QVector3D(0,0,zoom);
    QVector3D cameraNFPos = QVector3D(0,0,zoom-nearf);
    QVector3D NFPosY = QVector3D(0, 1/qCos(qDegreesToRadians(40.f))*nearf, cameraNFPos.z());

    QVector3D fovLineVector = NFPosY - cameraPos;
    //QVector3D rv = tn - center;
    //float rd = QVector3D::crossProduct(rv, tv).length()/tv.length();

    float r01 = QVector3D::dotProduct((NFPosY - center), fovLineVector)/fovLineVector.length();
    QVector3D prevFovPoint = NFPosY - r01/fovLineVector.length()*fovLineVector;
    QVector3D radiusFovPoint = (prevFovPoint-center).normalized()*radius;
    float coef; // = radius / ; // else - revert and -res
    float distance = (radius - (radiusFovPoint-prevFovPoint).length()) * zoom;
    //qInfo() << coef << radius / qSin(40);
    if(radius > distance)
        coef = radius / distance;
    else
        coef = distance / radius;
    zoom = coef + nearf;
    update();
}

void Widget::resetScene()
{
    model = NULL;
    rotate.setToIdentity();
    translate.setToIdentity();
    zoomMatrix.setToIdentity();
    zoom = 5.f;
    zoomMatrix.translate(0,0,-zoom);
    update();
}

void Widget::loadVBO()
{
    // vertices
    int countV = model->vertexes.count();
    vertices = new GLfloat[countV*3*2];
    for(int i=0; i < countV; i++){
        vertices[i*6+0] = model->vertexes[i].x();
        vertices[i*6+1] = model->vertexes[i].y();
        vertices[i*6+2] = model->vertexes[i].z();
        //qInfo() << model->normals[i] / model->normals[i].w();
        model->normals[i].toVector3D().normalize();
        //qInfo() << model->normals[i];
        vertices[i*6+3] = model->normals[i].x();//model->normals[i].w();
        vertices[i*6+4] = model->normals[i].y();//model->normals[i].w();
        vertices[i*6+5] = model->normals[i].z();//model->normals[i].w();
    }

    vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertices, (countV*3*2)*sizeof(GLfloat));
    vertexBuffer.release();
    //delete [] vertices;

    // http://www.gamedev.ru/code/articles/HLSL?page=4
    // http://steps3d.narod.ru/tutorials/lighting-tutorial.html
    // https://iandreev.wordpress.com/2010/06/27/%D0%BC%D0%BE%D0%B4%D0%B5%D0%BB%D1%8C-%D0%BE%D1%81%D0%B2%D0%B5%D1%89%D0%B5%D0%BD%D0%B8%D1%8F-%D0%B1%D0%BB%D0%B8%D0%BD%D0%BD%D0%B0-%D1%84%D0%BE%D0%BD%D0%B3%D0%B0/
    // https://stackoverflow.com/questions/21079623/how-to-calculate-the-normal-matrix

    // indices
    // polygons
    int countF = model->polygons.count();
    indices = new GLuint[countF*3+countF*3*2];
    for(int i=0; i < countF; i++){
        indices[i*3+0] = model->polygons[i].polygon[0].vertex;
        indices[i*3+1] = model->polygons[i].polygon[1].vertex;
        indices[i*3+2] = model->polygons[i].polygon[2].vertex;
    }
    // polygon edges
    int shift = countF*3;
    for(int i=0; i < countF; i++){
        indices[shift+i*6+0] = model->polygons[i].polygon[0].vertex;
        indices[shift+i*6+1] = model->polygons[i].polygon[1].vertex;
        indices[shift+i*6+2] = model->polygons[i].polygon[1].vertex;
        indices[shift+i*6+3] = model->polygons[i].polygon[2].vertex;
        indices[shift+i*6+4] = model->polygons[i].polygon[2].vertex;
        indices[shift+i*6+5] = model->polygons[i].polygon[0].vertex;
    }

    indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indices, (countF*3)*sizeof(GLuint));
    indexBuffer.release();

    indexLineBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexLineBuffer.create();
    indexLineBuffer.bind();
    indexLineBuffer.allocate((indices+shift), (countF*3*2)*sizeof(GLuint));
    indexLineBuffer.release();
    //delete [] indices;
    qInfo() << "VBO loaded";

//    glFunctions->glGenBuffers();
//    glGenBuffersARB(1, &vboId1);
//    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId1);
//    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), vertices, GL_STATIC_DRAW_ARB);
//    delete [] vertices;

//    glGenBuffersARB(2, &vboId2);
//    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vboId2);
//    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(indices), indices, GL_STATIC_DRAW_ARB);
//    delete [] indices;
}

