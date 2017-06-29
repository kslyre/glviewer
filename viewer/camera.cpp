#include "camera.h"

Camera::Camera()
{

}

void Camera::setProjection(float fov, float ratio, float nearField, float farField)
{
    this->fov = fov;
    aspectRatio = ratio;
    this->nearField = nearField;
    this->farField = farField;

    projection.setToIdentity();
    projection.perspective(fov, ratio, nearField, farField);
}
