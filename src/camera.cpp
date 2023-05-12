#include "camera.h"

void Camera::init()
{
    pos = glm::vec3(0.0f, 0.0f, 3.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    fov = 90.0f;
}

void Camera::move_forward(float amount)
{
    pos += glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * amount;
}

void Camera::move_right(float amount)
{
    glm::vec3 temp = glm::normalize(glm::cross(front, up)) * amount;
    pos += glm::vec3(temp.x, 0.0f, temp.z);
}

void Camera::move_up(float amount)
{
    pos += glm::vec3(0.0f, amount, 0.0f);
}