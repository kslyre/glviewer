#ifndef WIDGET_H
#define WIDGET_H


#include "camera.h"
#include "modelfactory.h"
#include "raytrace.h"
#include <QGLWidget>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <obj.h>
#include <bvh.h>
#include <QtMath>
#include <QOpenGLShaderProgram>
#include <QGLBuffer>
#include <QOpenGLFunctions>
#include <QApplication>
#include <QThread>

class Widget : public QGLWidget
{
    Q_OBJECT

public:
    Widget(QGLWidget *parent = 0);
    ~Widget();

    QList<Model*> models;

    QOpenGLShaderProgram *shader = nullptr;
    Model *addModel();

    void modelProjection();
    void fitToView(bool simple);
    void resetView();
    void clearScene();
    void gaussNewton();
    void icp();
protected:
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    Camera camera;

    QPointF pos;

    RayTrace rayTrace;
    ModelFactory factory;

    float zoom;
    QMatrix4x4 zoomMatrix;
    QMatrix4x4 rotate;
    QMatrix4x4 translate;
};

#endif // WIDGET_H
