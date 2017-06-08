#ifndef WIDGET_H
#define WIDGET_H

#include "camera.h"
#include <QGLWidget>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <obj.h>
#include <QtMath>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class Widget : public QGLWidget
{
    Q_OBJECT

public:
    Widget(QGLWidget *parent = 0);
    ~Widget();

    void fitToView(bool simple);
    void resetScene();
    void loadVBO();

    Obj *model;
    QOpenGLShaderProgram *shader = nullptr;
    //QOpenGLFunctions *glFunctions = nullptr;

    void loadVBO2();
    void trace();
protected:
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;



private:
    Camera camera;

    QPointF pos;

    GLuint vboId1;
    GLuint vboId2;
    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
    QOpenGLBuffer indexTextureBuffer;
    QOpenGLBuffer indexLineBuffer;    
    QOpenGLTexture *texture;
    GLfloat *vertices;
    GLuint *indices;
    GLfloat *texcoords;


    float zoom;
    QMatrix4x4 zoomMatrix;
    QMatrix4x4 rotate;
    QMatrix4x4 translate;
};

#endif // WIDGET_H
