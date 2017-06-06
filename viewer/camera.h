#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>



class Camera
{
public:
    Camera();

    QMatrix4x4 projection;

    float distance;
    float aspectRatio;
    float nearField;
    float farField;

    void setProjection(float distance, float ratio, float nearField, float farField);
};

#endif // CAMERA_H
