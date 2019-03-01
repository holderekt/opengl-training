#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include <time.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <cstring>
#include <shader.hpp>
#include <texture.hpp>
#include <model.hpp>
#include <camera.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *glWindowPos3dv);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

/* Global data modified by the mouse callback */
glm::vec3 playerFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 playerPos = glm::vec3(0.0f, -4.5f,  3.0f);
glm::vec3 playerUp = glm::vec3(0.0f, 1.0f,  0.0f);
Camera camera(glm::vec3(0.0f, 2.0f,  3.0f), glm::vec3(0.0f, 1.0f,  0.0f),glm::vec3(0.0f, 0.0f, -1.0f), 20.0f);

/* Mouse callback utility data */
bool firstMouse = true;
float yaw  = -90.0f;	
float pitch = -20.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float camX = 0;
float camZ = 0;

/* Sinusoidal function between two values */
class movement_function{
  private:
    float min;
    float max;

  public:
    movement_function(float min, float max)
      : min(min), max(max) {}

    float range() {return (max-min); }
    float cycle() { return 2*range(); } 

    float operator()(float input){
        float value = fmod(input, cycle());

        if (value > range())
            value = cycle() - value;

        return value + min;
    }
};


int main(){

    srand(time(NULL));

    glm::vec3 positions[] = {
        glm::vec3(-1.0f, -1.0f,0.0f),
        glm::vec3(1.0f,-1.0f,0.0f),
        glm::vec3(0.0f, 1.0f,0.0f)
    };

    /* Window Code */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Day 9", NULL,NULL);

    if(window == NULL){
        std::cout << "Cant do shit" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "Cant initialize GLAD" << std::endl;
    return -1;
    }

    /* Elements Code */ 

    Shader basicShader("./src/shaders/shader1.vert", "./src/shaders/frag1.frag");
    Model<float> cube1("./models/cube.csv");
    Model<float> floor1("./models/floor.csv");

    /* VBO and VAO construction */

    unsigned int VAO[2], VBO[2];
    glGenBuffers(2, VBO);
    glGenVertexArrays(2, VAO);
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, cube1.size(), cube1(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, floor1.size(), floor1(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Cursor settings */

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetCursorPosCallback(window, mouse_callback);  


    /* Textures */

    Texture brickTex("./textures/brick.png");
    Texture grassTex("./textures/grass.png");
    Texture redTex("./textures/red.png");
    glEnable(GL_DEPTH_TEST);  

    float rotationAngle;

    // Main Loop
    while(!glfwWindowShouldClose(window)){

        
        processInput(window);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(800/ 600), 0.1f, 100.0f);
        glm::mat4 view = camera.getView();
        glm::mat4 model;
        glm::mat4 transformation;

        unsigned int modelLoc = glGetUniformLocation(basicShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(basicShader.ID, "view");
        unsigned int proLoc = glGetUniformLocation(basicShader.ID, "projection");

        // Background color
    
        glClearColor(0.6f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Three rotating cubes
          
        basicShader();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO[0]);
        glActiveTexture(GL_TEXTURE0);
        brickTex();

        float h[3];

        for(int i=0; i!=3; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            switch(i){
                case 0:
                    brickTex();
                    h[0]= 1.0f;
                    h[1]= 1.0f;
                    h[2]= 0.0f;
                break;
                case 1:
                    grassTex();
                    h[0]= 0.0f;
                    h[1]= 1.0f;
                    h[2]= 1.0f;
                break;
                case 2:
                    redTex();
                    h[0]= 1.0f;
                    h[1]= 0.0f;
                    h[2]= 1.0f;
                break;
            }
            model = glm::rotate(model, (float)glfwGetTime()*((i+1)*2), glm::vec3(h[0],h[1],h[2]));  
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);          
        }

        // Floor

        basicShader();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));
        transformation = glm::mat4(1.0f);  
        transformation = glm::scale(transformation, glm::vec3(40.0,40.0, 0.0));  
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,-5.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f,0.0f, 0.0f)); 
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model*transformation));
        glBindVertexArray(VAO[1]);
        glActiveTexture(GL_TEXTURE0);
        brickTex();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Playeri

        basicShader();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1.0f);
        rotationAngle = atan2(camZ,camX+3);
        model = glm::translate(model, playerPos); 
        model = glm::rotate(model,-rotationAngle ,glm::vec3(0.0,1.0,0.0)); 
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO[0]);
        glActiveTexture(GL_TEXTURE0);
        grassTex();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width, height);    
}


void processInput(GLFWwindow *window){

    static float delta_time = 0;
    static float last_frame = 0;


    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }else{
        
        /* Calculate camera speed based on delta time */ 

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        float camera_speed;

        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            camera_speed = delta_time * 10.0f;
        }else{
            camera_speed = delta_time * 4.0f;
        }

        camera.setSpeed(camera_speed);

        /* WASD Movement based on camera direction vector */

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            playerPos -= glm::normalize(glm::cross(playerFront, playerUp)) * camera_speed;
            camera.move(D_RIGHT);      
        }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            playerPos += glm::normalize(glm::cross(playerFront, playerUp)) * camera_speed;
            camera.move(D_LEFT);  
        }

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            playerPos -= camera_speed * playerFront;
            camera.move(D_UP);
        }

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            playerPos += camera_speed * playerFront;
            camera.move(D_DOWN);
        }
    }
}




void mouse_callback(GLFWwindow* window, double xpos, double ypos){

    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = -(xpos - lastX);
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    camX = sin(yaw) * 20.0f;
    camZ = cos(yaw) * 20.0f;  

    glm::vec3 front;
    front.x = camX;
    front.y = 0.0;
    front.z = camZ;

    playerFront = glm::normalize(front);
    camera.changeRotation(yaw);
}

