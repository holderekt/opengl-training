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

class TPCamera{
public:

    TPCamera(glm::vec3, glm::vec3, glm::vec3, float);
    void move(directions);
    void changeRotation(float);
    glm::mat4 getView();
    void setSpeed(float speed){ camera_speed = speed; };

private:

    glm::vec3 cameraPos;
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;

    float camX;
    float camZ;

    float camera_speed;
    float radius;

};


TPCamera::TPCamera(glm::vec3 cameraPos, glm::vec3 cameraUp, glm::vec3 cameraFront, float radius){
    this->cameraPos   = cameraPos;
    this->cameraUp    = cameraUp;
    this->cameraFront = cameraFront;
    this->radius = radius;
    this->camera_speed = 0;
};


void TPCamera::move(directions dir){
    switch (dir){
        case D_UP:
            cameraPos -= camera_speed * cameraFront;
            break;
        case D_DOWN:
            cameraPos += camera_speed * cameraFront;
            break;
        case D_RIGHT:
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;
            break;
        case D_LEFT:
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;
            break;
    }
}

void TPCamera::changeRotation(float yaw){
    camX = sin(yaw) * radius;
    camZ = cos(yaw) * radius;
    cameraFront.x = camX;
    cameraFront.z = camZ;
    cameraFront = glm::normalize(cameraFront);
}


glm::mat4 TPCamera::getView(){
    return glm::lookAt(glm::vec3(cameraPos.x + camX, cameraPos.y, cameraPos.z +camZ), cameraPos + cameraFront, cameraUp);
}