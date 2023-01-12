#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV         = 45.0f;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float fov;

    Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
            float _yaw = YAW,
            float _pitch = PITCH):
            front(glm::vec3(0.0f, 0.0f, -1.0f)), 
            movementSpeed(SPEED), 
            mouseSensitivity(SENSITIVITY), 
            fov(FOV) {
        position = _position;
        worldUp = _up;
        yaw = _yaw;
        pitch = _pitch;
        updateCameraVectors();
    }

    Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch):
            front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV) {
        position = glm::vec3(_posX, _posY, _posZ);
        worldUp = glm::vec3(_upX, _upY, _upZ);
        yaw = _yaw;
        pitch = _pitch;
        updateCameraVectors();    
    }

    glm::mat4 GetViewMatrix(){
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(Camera_Movement direction, float deltaTime, bool fast){
        float velocity = movementSpeed * deltaTime;
        if (fast)
            velocity *= 4;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        if (direction == UP)
            position += up * velocity;
        if (direction == DOWN)
            position -= up * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true){
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch){
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }
    
    void ProcessMouseScroll(double yoffset){
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 120.0f)
            fov = 120.0f;
    }

    private:
        void updateCameraVectors(){
            glm::vec3 newFront;
            newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            newFront.y = sin(glm::radians(pitch));
            newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(newFront);

            right = glm::normalize(glm::cross(front, worldUp));
            up = glm::normalize(glm::cross(right, front));
        }
};

#endif