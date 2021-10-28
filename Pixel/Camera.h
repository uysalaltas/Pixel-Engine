#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

# define M_PI           3.14159265358979323846  /* pi */

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

float sensitivity = 0.1f;
float mouseUpDown = 0;
float mouseLeftRight = 0;

class Camera
{
public:

    Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector, float width, float height)
        : m_eye(std::move(eye))
        , m_lookAt(std::move(lookat))
        , m_upVector(std::move(upVector))
        , m_width(width)
        , m_height(height)
    {
        UpdateViewMatrix();
    }

    glm::mat4x4 GetViewMatrix() const { return m_viewMatrix; }
    glm::vec3 GetEye() const { return m_eye; }
    glm::vec3 GetUpVector() const { return m_upVector; }
    glm::vec3 GetLookAt() const { return m_lookAt; }
    glm::vec3 GetViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
    glm::vec3 GetRightVector() const { return glm::transpose(m_viewMatrix)[0]; }
    float GetFOV() const { return m_fov; }

    void UpdateViewMatrix()
    {
        m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
    }

    void SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up)
    {
        m_eye = std::move(eye);
        m_lookAt = std::move(lookat);
        m_upVector = std::move(up);
        UpdateViewMatrix();
    }

    void ArcBallCamera(float deltaX, float deltaY) 
    {
        glm::vec4 position(GetEye().x, GetEye().y, GetEye().z, 1);
        glm::vec4 pivot(GetLookAt().x, GetLookAt().y, GetLookAt().z, 1);
        float deltaAngleX = (2 * M_PI / m_width);
        float deltaAngleY = (M_PI / m_height);
        float xAngle = deltaX * deltaAngleX;
        float yAngle = deltaY * deltaAngleY;

        float cosAngle = glm::dot(GetViewDir(), m_upVector);
        if (cosAngle * glm::sign(yAngle) > 0.99f)
            yAngle = 0;

        glm::mat4x4 rotationMatrixX(1.0f);
        rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_upVector);
        position = (rotationMatrixX * (position - pivot)) + pivot;

        glm::mat4x4 rotationMatrixY(1.0f);
        rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightVector());
        glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

        SetCameraView(finalPosition, GetLookAt(), m_upVector);
    }

    void ProcessMouseScroll(float yoffset)
    {
        m_fov -= (float)yoffset;
        if (m_fov < 1.0f)
            m_fov = 1.0f;
        if (m_fov > 45.0f)
            m_fov = 45.0f;
    }

    //// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    //void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    //{
    //    float velocity = MovementSpeed * deltaTime;
    //    if (direction == FORWARD)
    //        Position += Front * velocity;
    //    if (direction == BACKWARD)
    //        Position -= Front * velocity;
    //    if (direction == LEFT)
    //        Position -= Right * velocity;
    //    if (direction == RIGHT)
    //        Position += Right * velocity;
    //}
    //void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    //{
    //    xoffset *= sensitivity;
    //    yoffset *= sensitivity;
    //    mouseLeftRight += xoffset;
    //    mouseUpDown += yoffset;
    //    m_eye.y = r * cos(glm::radians(mouseUpDown));
    //    m_eye.z = r * sin(glm::radians(mouseUpDown));
    //    UpdateViewMatrix();
    //    std::cout << glm::to_string(m_eye) << std::endl;
    //}

private:
    glm::mat4x4 m_viewMatrix;
    glm::vec3 m_eye;                // Camera position in 3D
    glm::vec3 m_lookAt;             // Point that the camera is looking at
    glm::vec3 m_upVector;           // Orientation of the camera
    float m_width;
    float m_height;
    float m_fov = 45;
};
#endif
