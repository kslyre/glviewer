#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>



class Camera
{
public:
    Camera();

    QMatrix4x4 projection;

    float fov;
    float aspectRatio;
    float nearField;
    float farField;

    void setProjection(float fov, float ratio, float nearField, float farField);
};

#endif // CAMERA_H
