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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main(){

    srand(time(NULL));

    float vertices1[] = {
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // up
        -0.1f, -0.5f, 0.0f,0.0f, 1.0f, 0.0f, // bottom left 
        -0.5f, -0.5f, 0.0f,0.0f, 0.0f, 1.0f // Bottom righ 
    };

    float vertices2[] = {
        -0.45f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// up
        -0.05f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f, // bottom left 
        -0.05f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f// Bottom right
    };

    // float verts[2][3];
    // verts[0] = vertices1;
    // verts[1] = vertices2; 


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    GLFWwindow* window = glfwCreateWindow(800, 600, "Day 3", NULL,NULL);

    if(window == NULL){
        std::cout << "Cant do shit" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // Tell OpenGL the rendering window
    //glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "Cant initialize GLAD" << std::endl;
    return -1;
    }

    Shader basicShader("./src/shaders/shader1.vert", "./src/shaders/frag1.frag");

    // Vertex Buffer Object
    unsigned int VBO[2];
    unsigned int VAO[2];
    glGenBuffers(2, VBO);
    glGenVertexArrays(2, VAO);

    // for(int i = 0; i<2; i++){
    //      glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(&verts[i]), &verts[i], GL_STATIC_DRAW);
    //     glBindVertexArray(VAO[i]);
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //     glEnableVertexAttribArray(0);
    // }

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glBindVertexArray(VAO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBindVertexArray(VAO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // // Element Buffer Object
    // unsigned int EBO;
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    

    // Main Loop
    while(!glfwWindowShouldClose(window)){

        processInput(window);

        /*  Code  */
        glClearColor(0.6f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        basicShader();
        

        // float timeValue = glfwGetTime();
        // float redValue = sin(timeValue) / 2.0f + 0.5f;
        // //std::cout << redValue << std::endl;
        // int uniformLocation =  glGetUniformLocation(shaderProgram, "inputColor");
        // glUniform4f(uniformLocation,redValue, 0.0f, 0.0f, 1.0f);
        
        
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

         
       
        // glUniform4f(uniformLocation,redValue, 0.4f, 0.0f, 1.0f);
        

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);


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
