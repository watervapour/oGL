#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "shader.h"

#include <iostream>
#include <cmath>

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,// bottom right
    -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// top left
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f // top right
};

float vertices_second[] = {
    0.0f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.7f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.7f, 0.0f, 0.0f, 0.0f, 1.0f
};

unsigned int indices[] = {
    0, 2, 1,
    2, 3, 1
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char** argv){

    GLFWwindow* window;

    if (!glfwInit()){
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(800, 600, "oGL", NULL, NULL);
    if (window == NULL){
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to init GLAD.\n";
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Max # of vertex attribs: "<< nrAttributes << "\n";    

    shader shader1("./vertex.vs", "./fragment_1.fs");
    shader shader2("./vertex.vs", "./fragment_2.fs");

    // buffers and vertex attributes
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // second set of VAO, VBO
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_second), vertices_second, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        
        glClearColor(0.1f, 0.45f, 0.35f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader1.use();
        glBindVertexArray(VAO);        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        float timeValue = glfwGetTime();
        float sinValue = (sin(timeValue) / 3.0f) + 0.5f;
        shader2.use();
        shader2.setFloat("cyclic", sinValue);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}