#ifndef VBO_H
#define VBO_H

#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <obj.h>
#include <QDebug>

class VBO
{
public:
    VBO();

    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
    QOpenGLBuffer indexLineBuffer;
    QOpenGLTexture *texture;

    void loadVBO(Obj *mdl);
};

#endif // VBO_H
