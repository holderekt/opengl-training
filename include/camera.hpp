#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <png.h>
#include <cstdint>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


typedef enum {D_UP, D_DOWN, D_RIGHT, D_LEFT} directions;

class Camera{
public:

    Camera(glm::vec3, glm::vec3, glm::vec3, float);
    void move(directions);
    void changeRotation(float);
    void changeRotation(float, float);
    glm::mat4 getView();
    glm::vec3 getPosition(){ return cameraPos; }
    void setSpeed(float speed){ camera_speed = speed; }

private:

    glm::vec3 cameraPos;
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;

    float camX;
    float camZ;

    float camera_speed;
    float radius;

};


Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraUp, glm::vec3 cameraFront, float radius){
    this->cameraPos   = cameraPos;
    this->cameraUp    = cameraUp;
    this->cameraFront = cameraFront;
    this->radius = radius;
    this->camera_speed = 0;
};


void Camera::move(directions dir){
    switch (dir){
        case D_UP:
            cameraPos += camera_speed * cameraFront;
            break;
        case D_DOWN:
            cameraPos -= camera_speed * cameraFront;
            break;
        case D_RIGHT:
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;
            break;
        case D_LEFT:
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;
            break;
    }

    std::cout << " Pos " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
    std::cout << " Front " << cameraFront.x << cameraFront.y << cameraFront.z << std::endl;

}

void Camera::changeRotation(float yaw){
    camX = sin(yaw) * radius;
    camZ = cos(yaw) * radius;
    cameraFront.x = camX;
    cameraFront.z = camZ;
    cameraFront = glm::normalize(cameraFront);
}

void Camera::changeRotation(float yaw, float pitch){
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

// todO ADD CAMZ / X
glm::mat4 Camera::getView(){
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}