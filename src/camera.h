#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>


enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 8.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 80.0f;
const float MULTIPLIER = 1.0f;


class Camera 
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    //Eular Angles;
    float Yaw;
    float Pitch;

    //Camera Options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float Multiplier_;



    // constructors with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), Multiplier_(MULTIPLIER)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // constructor with the scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), Multiplier_(MULTIPLIER)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    //returns the view matrix calculated using eular angles and the lookat matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    //processes input received from any keyboard-like input system. accepts input parameneter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, bool isShift_)
    {
        float velocity = MovementSpeed * deltaTime;
        float multiplier = 1.0f;
        if (isShift_)
        {
            multiplier = 2.0f;
        }
        if (direction == FORWARD)
        {
            Position += Front * velocity * multiplier * Multiplier_;
        }
        if (direction == BACKWARD)
        {
            Position -= Front * velocity * multiplier * Multiplier_;
        }
        if (direction == LEFT)
        {
            Position -= Right * velocity * multiplier * Multiplier_;
        }
        if (direction == RIGHT)
        {
            Position += Right * velocity * multiplier * Multiplier_;
        }
        if (direction == UP)
        {
            Position += WorldUp * velocity * multiplier * Multiplier_;
        }
        if (direction == DOWN)
        {
            Position -= WorldUp * velocity * multiplier * Multiplier_;
        }
    }

    // processes input received from a mouse input system, expects the offset value in both the x and y direction
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        /*
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 120.0f)
            Zoom = 120.0f;
        */
        Multiplier_ += (float)yoffset / 10.0f;
        if (Multiplier_ < 0.01f)
            Multiplier_ = 0.01f;
        if (Multiplier_ > 120.0f)
            Multiplier_ = 120.0f;
        
    }
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif