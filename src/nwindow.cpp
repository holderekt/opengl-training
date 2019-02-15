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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


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

    // float vertices[] = {
    //     -0.8f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // up
    //     -0.8f, -0.8f, 0.0f,0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom left 
    //     -0.2f, -0.8f, 0.0f,0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // Bottom righ 
    //     -0.2f, -0.5f, 0.0f,0.5f, 0.5f, 1.0f,   0.0f, 1.0f    // Bottom righ 
    // };

    float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

    // float vertices2[] = {
    //     0.2f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // up
    //     0.2f, 0.5f, 0.0f,0.0f, 1.0f, 0.0f,  1.0f, 0.0f,// bottom left 
    //     0.8f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f,  0.0f, 0.0f,// Bottom righ 
    //     0.8f, 0.8f, 0.0f,0.5f, 0.5f, 1.0f,   0.0f, 1.0f  // Bottom righ 
    // };

    unsigned int elements[] = {
        0,1,3,
        1,2,3
    };



    

    /* Window Code */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    GLFWwindow* window = glfwCreateWindow(800, 600, "Day 6", NULL,NULL);

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

    unsigned int VAO[2], VBO[2], EBO;
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(2, VAO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);



    // glBindVertexArray(VAO[1]);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    // glEnableVertexAttribArray(2);
    
    movement_function movOffset(0.0f, 1.4f);

    // Textures

    Texture a;
    a.load_image("./textures/brick.png");
   


    // Main Loop
    while(!glfwWindowShouldClose(window)){

        processInput(window);

        /*  Code    */

        glClearColor(0.6f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue =0; // glfwGetTime();

        basicShader();

        
        basicShader.setValue("yOffset",  movOffset(timeValue));
        basicShader.setValue("xOffset",  0.0f);
        glBindVertexArray(VAO[0]);
        a();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glBindTexture(GL_TEXTURE_2D, texture);
        // basicShader.setValue("yOffset", movOffset(timeValue) * (-1));
        // basicShader.setValue("xOffset",  0.0f);
        // glBindVertexArray(VAO[1]);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /*          */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

// Function that handles the resize at runtime
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width, height);    
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}


