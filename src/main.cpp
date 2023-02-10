#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "camera.h"
#include "mesh.h"

#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <string>

unsigned int loadTexture(char const * path, unsigned int wrap_mode);

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,   2.0f),
    glm::vec3( 2.3f, -3.3f,  -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f,  -3.0f)
};

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Max # of vertex attribs: " << nrAttributes << "\n";

    cam.position = cam.position - glm::vec3(0.0f, 0.0f, -6.0f);

   float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    float transparentVertices[] = {
        // positions 
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  1.0f
    };
    std::vector<glm::vec3> transparencyPositions = {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f),
        glm::vec3(-3.3f, 0.0f, -2.3f),
        glm::vec3(4.3f, 0.0f, 3.3f),
        glm::vec3(-1.3f, 0.0f, 0.3f),
        glm::vec3(0.3f, 0.0f, -0.1f),
    };

    stbi_set_flip_vertically_on_load(true);
    Shader TexShader("./vertex.vs", "./fragment_model.fs");
    
    // Floor mesh
    std::vector<unsigned int> floorInts = {
        0, 1, 2,
        3, 4, 5
    };
    std::vector<Vertex> verts;
    for(int i = 0; i < (sizeof(planeVertices) / sizeof(float)); i+=5){
        Vertex v;
        v.Position = { planeVertices[0 + i], planeVertices[1 + i], planeVertices[2 + i]};
        v.Normal = {0.0f, 0.0f, 1.0f};
        v.TexCoords = { planeVertices[3 + i], planeVertices[4 + i]};
        verts.push_back(v);
    }    
    unsigned int floorTexRef = loadTexture("../res/metal.png", GL_REPEAT);
    Texture floor_tex = {floorTexRef, "../res/metal.png", "texture_diffuse"};
    std::vector<Texture> texVec;
    texVec.push_back(floor_tex);
    Mesh M_floor = Mesh(verts, floorInts, texVec);

    // Cube mesh
    std::vector<unsigned int> cubeInts;
    for(int i = 0; i < 36; i++){
        cubeInts.push_back(i);
    }
    std::vector<Vertex> cubeVerts;
    for (int i = 0; i < (sizeof(cubeVertices) / sizeof(float)); i+=5){
        Vertex v;
        v.Position = { cubeVertices[i + 0], cubeVertices[i + 1], cubeVertices[i + 2]};
        v.Normal = {0.0f, 0.0f, 1.0f};
        v.TexCoords = { cubeVertices[i + 3], cubeVertices[i + 4] };
        cubeVerts.push_back(v);
    }
    unsigned int cubeTexture = loadTexture("../res/marble.jpg", GL_CLAMP_TO_EDGE);
    Texture cube_tex = {cubeTexture, "../res/marble.jpg", "texture_diffuse"};
    Mesh M_cube = Mesh(cubeVerts, cubeInts, std::vector<Texture>{cube_tex});


    // transparencies
    Shader trShader = Shader("./vertex.vs", "./fragment_transparency.fs");
    unsigned int trVAO, trVBO;
    glGenVertexArrays(1, &trVAO);
    glGenBuffers(1, &trVBO);
    glBindVertexArray(trVAO);
    glBindBuffer(GL_ARRAY_BUFFER, trVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    unsigned int transparency = loadTexture("../res/blending_transparent_window.png", GL_CLAMP_TO_EDGE);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cam.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), 800.0f / 600.0f, 0.1f, 100.0f);
        
        
        TexShader.use();
        TexShader.setMat4("model", model);
        TexShader.setMat4("view", view);
        TexShader.setMat4("projection", projection);
        M_floor.Draw(TexShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 2.2f));
        TexShader.setMat4("model", model);
        M_cube.Draw(TexShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.2f, 0.3f, 0.5f));
        TexShader.setMat4("model", model);
        M_cube.Draw(TexShader);


        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < transparencyPositions.size(); i++){
            float distance = glm::length(cam.position - transparencyPositions[i]);
            sorted[distance] = transparencyPositions[i];
        }
                
        glBindVertexArray(trVAO);
        glBindTexture(GL_TEXTURE_2D, transparency);
        trShader.use();
        trShader.setMat4("view", view);
        trShader.setMat4("projection", projection);
        trShader.setInt("Material.tex", 0);
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            trShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    glfwTerminate();
    return 0;
}

unsigned int loadTexture(char const * path, unsigned int wrap_mode){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if(data){
        GLenum format;
        if(nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}