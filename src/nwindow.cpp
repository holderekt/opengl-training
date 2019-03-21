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
#include <light.hpp>
#include <mesh.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *glWindowPos3dv);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

/* Global data modified by the mouse callback */
glm::vec3 playerFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 playerPos = glm::vec3(0.0f, -4.5f,  3.0f);
glm::vec3 playerUp = glm::vec3(0.0f, 1.0f,  0.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f,  0.0f),glm::vec3(0.0f, 0.0f, -3.0f), 0.0f);
glm::vec3 lightPos =  glm::vec3(1.0,-2.0,1.0);
glm::vec3 lightPos2 = glm::vec3(-2.0,-2.0,1.0);

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

    /* Window Code */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Day 19", NULL,NULL);

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

    Shader lightShader("./src/shaders/shader1.vert", "./src/shaders/frag1.frag");
    Shader lampShader("./src/shaders/shader2.vert", "./src/shaders/frag2.frag");
    Model<float> cube1("./models/cube.csv");

    Texture crateTex("./textures/crate.png", TEXTURE_DIFFUSE);
    Texture crate_spec_mapTex("./textures/crate_specular_map.png", TEXTURE_SPECULAR);

    std::vector<Texture> vtex;
    vtex.push_back(crateTex);
    vtex.push_back(crate_spec_mapTex);
    std::vector<Texture> vvvv;

    std::vector<unsigned int> mario;
    Mesh msh(cube1.getcose(), mario , vtex);
    Mesh msh2(cube1.getcose(), mario, vvvv);
 
    /* Cursor settings */

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetCursorPosCallback(window, mouse_callback);  




    glEnable(GL_DEPTH_TEST);  


    DirectionalLight light1(
        glm::vec3(0.2, 0.2, 0.2),
        glm::vec3( 0.5, 0.5, 0.5),
        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(-0.8f, -1.0f, -0.7));

    PointLight lightp1(
        glm::vec3(0.2, 0.2, 0.2),
        glm::vec3( 0.5, 0.5, 0.5),
        glm::vec3(1.0, 1.0, 1.0),
        lightPos, 1.0f, 0.032f, 0.09f
    );

    PointLight lightp2(
        glm::vec3(0.2, 0.2, 0.2),
        glm::vec3( 0.5, 0.5, 0.5),
        glm::vec3(1.0, 1.0, 1.0),
        lightPos2, 1.0f, 0.032f, 0.09f
    );


    // Main Loop
    while(!glfwWindowShouldClose(window)){

        
        processInput(window);
         float current_frame = glfwGetTime();
        //lightPos.x = sin(current_frame) * 5.0f;
        lightPos.z = cos(current_frame) * 3;
        lightPos2.z = sin(current_frame) * 10.0f;

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(800/ 600), 0.1f, 100.0f);
        glm::mat4 view = camera.getView();
        glm::mat4 model = glm::mat4(1);
        glm::mat4 transformation;


        // Background color
    
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lampShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model,lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        lampShader.setValue("model", model);
        lampShader.setValue("projection", projection);
        lampShader.setValue("view", view);
        msh2.draw(lampShader);

        lampShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model,lightPos2);
        model = glm::scale(model, glm::vec3(0.1f));
        lampShader.setValue("model", model);
        lampShader.setValue("projection", projection);
        lampShader.setValue("view", view);
        msh2.draw(lampShader);


        lightShader();

        lightShader.setValue("objectColor", glm::vec3(0.60, 1.00, 0.90));
        lightShader.setValue("lightColor", glm::vec3(1.0,1.0,1.0));
        lightShader.setValue("cameraPos", camera.getPosition());
        lightShader.setValue("material1.shinnes", 64.0f);

        //light1.use(lightShader, "dLight");

        lightp1.setPosition(lightPos);
        lightp1.use(lightShader, "pLight", 0);

        lightp2.setPosition(lightPos2);
        lightp2.use(lightShader, "pLight", 1);
 
        model = glm::mat4(1);
         model = glm::translate(model,glm::vec3(0.0,-2.0,0.0));
        lightShader.setValue("projection", projection);
        lightShader.setValue("view", view);
        lightShader.setValue("model", model);
        lightShader.setValue("objectColor", glm::vec3(1.0,1.0,1.0));


        msh.draw(lightShader);

        
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

    float xoffset = (xpos - lastX);
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    camX = sin(yaw) * 100;
    camZ = cos(yaw) * 100;

    camera.changeRotation(yaw, pitch);
}

