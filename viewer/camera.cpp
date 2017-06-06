#include "camera.h"

Camera::Camera()
{

}

void Camera::setProjection(float fov, float ratio, float nearField, float farField)
{
    projection.setToIdentity();
    projection.perspective(fov, ratio, nearField, farField);
}
