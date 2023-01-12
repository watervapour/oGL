#include <glad\glad.h>
#include <GLFW\glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>
#include <cmath>
#include <string>

float vertices[] = {
    // positions            // normals          // texture coords
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
    0.5f, 0.5f, -0.5f,      0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
    0.5f, 0.5f, -0.5f,      0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f,      -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,     -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,     -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,      -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

    0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    0.5f, 0.5f, -0.5f,      1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,      1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,     0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
    0.5f, -0.5f, 0.5f,      0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,      0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,     0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,  0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    0.5f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f};

glm::vec3 cubePositions[] ={
    glm::vec3( 0.0f, 0.0f, 0.0f),
    glm::vec3( 2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
};

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

Camera cam;

// frame timing variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float frameSum = 0.0f;
int fpsCount = 0;
int fpsHistoric = 0;

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    bool sprint = false;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        sprint = true;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.processKeyboard(FORWARD, deltaTime, sprint);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.processKeyboard(BACKWARD, deltaTime, sprint);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.processKeyboard(LEFT, deltaTime, sprint);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.processKeyboard(RIGHT, deltaTime, sprint);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.processKeyboard(UP, deltaTime, sprint);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.processKeyboard(DOWN, deltaTime, sprint);
    
}

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    cam.processMouseMovement( ((float)xpos - lastX), (lastY - (float)ypos), true);
    lastX = xpos;
    lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    cam.ProcessMouseScroll(yoffset);
}

int main(int argc, char **argv){

    GLFWwindow *window;

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to init GLAD.\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Max # of vertex attribs: " << nrAttributes << "\n";

    cam.position = cam.position - glm::vec3(0.0f, 0.0f, -6.0f);

    shader shaderCrate("./vertex.vs", "./fragment_phong.fs");
    // buffers and vertex attributes
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // texture_diffuse
    unsigned int crateDiffuse;
    glGenTextures(1, &crateDiffuse);
    glBindTexture(GL_TEXTURE_2D, crateDiffuse);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int texWidth, texHeight, texChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../res/container2.png", &texWidth, &texHeight, &texChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << "../res/container2.png" << std::endl;
    }
    stbi_image_free(data);
    // texture_specular
    unsigned int crateSpecular;
    glGenTextures(1, &crateSpecular);
    glBindTexture(GL_TEXTURE_2D, crateSpecular);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../res/container2_specular.png", &texWidth, &texHeight, &texChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << "../res/container2_specular.png" << std::endl;
    }
    stbi_image_free(data);


    shader lightingShader("./vertex.vs", "./fragment_light.fs");
    lightingShader.use();
    // lighting 
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        fpsCount++;
        frameSum += deltaTime;
        if (frameSum >= 1.0f){
            fpsHistoric = fpsCount;
            fpsCount = 0;
            frameSum -= 1.0f;
        }
        std::string windowTitle = "oGL";
        windowTitle.append(" | FOV: ");
        windowTitle.append(std::to_string((int)cam.fov));
        windowTitle.append(" | FPS = ");
        windowTitle.append(std::to_string(fpsHistoric));
        windowTitle.append(" | frameTime = ");
        windowTitle.append(std::to_string(deltaTime*1000.0f));
        windowTitle.append("ms");
        glfwSetWindowTitle(window, windowTitle.c_str());

        processInput(window);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // object
        shaderCrate.use();
        shaderCrate.setInt("material.diffuse", 0);
        shaderCrate.setInt("material.specular", 1);
        shaderCrate.setFloat("material.shininess", 64.0f);

        glm::vec3 lightColor = glm::vec3(1.0f);
        glm::vec3 lightDiffuse = lightColor * glm::vec3(0.9f);
        glm::vec3 lightAmbient = lightDiffuse * glm::vec3(0.5f);
        shaderCrate.setVec3("light.ambient", lightAmbient);
        shaderCrate.setVec3("light.diffuse", lightDiffuse);
        shaderCrate.setVec3("light.position", cam.position);
        shaderCrate.setVec3("light.direction", cam.front);
        shaderCrate.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
        shaderCrate.setFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));
        shaderCrate.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        shaderCrate.setFloat("light.constant", 1.0f);
        shaderCrate.setFloat("light.linear", 0.59f);
        shaderCrate.setFloat("light.quadratic", 0.032f);
        shaderCrate.setVec3("viewPos", cam.position);
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cam.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), 800.0f / 600.0f, 0.1f, 100.0f);
            
        for (int i = 0; i < 10; i++){    
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) + (20.0f * i), glm::vec3(0.5f, 1.0f, 0.0f));
            shaderCrate.setMat4("model", model);
            shaderCrate.setMat4("view", view);
            shaderCrate.setMat4("projection", projection);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, crateDiffuse);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, crateSpecular);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // light
        // lightingShader.use();
        // model = glm::mat4(1.0f);
        // model = glm::translate(model, lightPos);
        // model = glm::scale(model, glm::vec3(0.2f));
        // lightingShader.setMat4("view", view);
        // lightingShader.setMat4("projection", projection);
        // lightingShader.setMat4("model", model);
        // lightingShader.setVec3("color", lightColor);
        // glBindVertexArray(lightVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}