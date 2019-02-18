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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, bool *value);


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

    // unsigned int elements[] = {
    //     0,1,3,
    //     1,2,3
    // };

    glm::vec3 positions[] = {
        glm::vec3(-1.0f, -1.0f,0.0f),
        glm::vec3(1.0f, -1.0f,0.0f),
        glm::vec3(0.0f, 1.0f,0.0f)
    };

    Model<float> cube1("./models/cube.csv");
    

    /* Window Code */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    GLFWwindow* window = glfwCreateWindow(800, 600, "Day 7", NULL,NULL);

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
   // glGenBuffers(1, &EBO);
    glGenVertexArrays(2, VAO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, cube1.size(), cube1(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    //glEnableVertexAttribArray(2);



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
    Texture b;
    b.load_image("./textures/grass.png");
    Texture c;
    c.load_image("./textures/red.png");
   
    basicShader();
    basicShader.setValue("mTex1",0);

    // float timeValue;
    bool flag = true;
    float yOffset = 0.0;
    float xOffset = 0.0;

    glEnable(GL_DEPTH_TEST);  



    // Main Loop
    while(!glfwWindowShouldClose(window)){

        processInput(window, &flag);


        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        //note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.5f)); 
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)(800/ 600), 0.1f, 100.0f);


        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            if(xOffset <= 0.7)
                xOffset = xOffset + 0.03;
        }

        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            if(xOffset >= -0.7)
                xOffset = xOffset - 0.03;
        }

        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            if(yOffset <= 0.7)
                yOffset = yOffset + 0.03;
        }

        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            if(yOffset >= (-0.7))
                yOffset = yOffset - 0.03;
        }

        /*  Code    */

        glClearColor(0.6f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // if(flag)
        //     timeValue =  glfwGetTime();
      
        
        basicShader();
        basicShader.setValue("yOffset", yOffset);
        basicShader.setValue("xOffset", xOffset);

        unsigned int modelLoc = glGetUniformLocation(basicShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        unsigned int viewLoc = glGetUniformLocation(basicShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int proLoc = glGetUniformLocation(basicShader.ID, "projection");
        glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO[0]);
        glActiveTexture(GL_TEXTURE0);
        a();

        for(int i=0; i!=3; i++){
            model =  glm::translate(view, positions[i]);
            switch(i){
                case 0:
                    a();
                break;
                case 1:
                    b();
                break;
                case 2:
                    c();
                break;
            }
            model = glm::rotate(model, (float)glfwGetTime()*((i+1)*2), glm::vec3(1.0f, 1.0f, .0f));  
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);          
        }
        
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

void processInput(GLFWwindow *window, bool *value){


    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
        *value = !(*value);
    }
}


