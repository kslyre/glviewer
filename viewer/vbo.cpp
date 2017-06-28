#include "vbo.h"

VBO::VBO()
{

}

void VBO::loadVBO(Obj *mdl)
{
    int vertexesSize = mdl->polygons.count()*3;
    int countPolygons = mdl->polygons.count();

    VertexStruct *vs = new VertexStruct[vertexesSize];
    GLuint *indices = new GLuint[countPolygons*3+countPolygons*3*2];

    // vertices and indices
    for(int polygonIndex=0; polygonIndex < countPolygons; polygonIndex++){
        for(int polyVertexIndex=0; polyVertexIndex < 3; polyVertexIndex++){
            PolygonVertex p = mdl->polygons[polygonIndex].list[polyVertexIndex];

            indices[polygonIndex*3+polyVertexIndex] = polygonIndex*3+polyVertexIndex;

            vs[polygonIndex*3+polyVertexIndex].vertex  = mdl->vertexes[p.vertex];
            vs[polygonIndex*3+polyVertexIndex].normal  = mdl->normals[p.vertex].toVector3D();
            if(mdl->textures.length() == 0)
                continue;
            vs[polygonIndex*3+polyVertexIndex].texture = mdl->textures[p.texture].toVector2D();
        }
    }

    vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vs, (vertexesSize)*sizeof(VertexStruct));
    vertexBuffer.release();
    delete [] vs;

    // polygon edges
    int shift = countPolygons*3;
    for(int i=0; i < countPolygons; i++){
        indices[shift+i*6+0] = i*3+0;
        indices[shift+i*6+1] = i*3+1;
        indices[shift+i*6+2] = i*3+1;
        indices[shift+i*6+3] = i*3+2;
        indices[shift+i*6+4] = i*3+2;
        indices[shift+i*6+5] = i*3+0;
    }

    indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indices, (countPolygons*3)*sizeof(GLuint));
    indexBuffer.release();

    indexLineBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexLineBuffer.create();
    indexLineBuffer.bind();
    indexLineBuffer.allocate((indices+shift), (countPolygons*3*2)*sizeof(GLuint));
    indexLineBuffer.release();
    delete [] indices;
    qInfo() << "VBO loaded";

    if(mdl->textures.length() == 0)
        return;

    texture = new QOpenGLTexture(QImage(":/checker2.png"));
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    qInfo() << "Texture loaded";
}
