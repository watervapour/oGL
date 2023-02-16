#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <string>

unsigned int window_width = 800;
unsigned int window_height = 600;

Camera cam;

// frame timing variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float frameSum = 0.0f;
int fpsCount = 0;
int fpsHistoric = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
}
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

float lastX = (float)window_width/2.0f;
float lastY = (float)window_height/2.0f;
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

unsigned int loadTexture(char const * path, unsigned int wrap_mode);
unsigned int loadCubeMap(std::vector<std::string> faces);

int main(int argc, char **argv){

    GLFWwindow *window;

    if (!glfwInit()){
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, "oGL", NULL, NULL);
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
    GLFWimage logo;
    logo.pixels = stbi_load("../res/oGL-logo.png", &logo.width, &logo.height, 0, 4);
    glfwSetWindowIcon(window, 1, &logo);
    stbi_image_free(logo.pixels);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to init GLAD.\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Max # of vertex attribs: " << nrAttributes << "\n";

    cam.position = cam.position - glm::vec3(0.0f, 0.0f, -6.0f);

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    unsigned int fbo_texture;
    glGenTextures(1, &fbo_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "Framebuffer creation error!" << std::endl;
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    float quadVertices[] ={
        -1.0f, 1.0f,    0.0f, 1.0f,
        -1.0f,-1.0f,    0.0f, 0.0f,
         1.0f,-1.0f,    1.0f, 0.0f,
        
        -1.0f, 1.0f,    0.0f, 1.0f,
         1.0f,-1.0f,    1.0f, 0.0f,
         1.0f, 1.0f,    1.0f, 1.0f
    };
    
    unsigned int fbqVAO, fbqVBO;
    glGenVertexArrays(1, &fbqVAO);
    glGenBuffers(1, &fbqVBO);
    glBindVertexArray(fbqVAO);
    glBindBuffer(GL_ARRAY_BUFFER, fbqVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    Shader fbq_shader = Shader("./vertex_fbquad.vs", "./fragment_fbquad.fs");
    fbq_shader.use();
    fbq_shader.setInt("screenTexture", 0);

    float cubeVertices[] = {
        // positions          // texture Coords
        // back
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        // front
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        // left
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        // right
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        // bottom
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        // top
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f
    };
    float transparentVerticePostions[] = {
        0.0f,  0.5f,  0.0f,
        0.0f, -0.5f,  0.0f,
        1.0f, -0.5f,  0.0f,

        0.0f,  0.5f,  0.0f,
        1.0f, -0.5f,  0.0f,
        1.0f,  0.5f,  0.0f
    };
    float transparentVerticeUVs[] = {        
        0.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,

        0.0f,  1.0f,
        1.0f,  0.0f,
        1.0f,  1.0f
    };


    std::vector<glm::vec3> transparencyPositions = {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f),
        glm::vec3(-3.3f, 0.0f, -2.3f),
        glm::vec3(-4.3f, 0.0f, 3.3f),
        glm::vec3(-1.3f, 0.0f, 0.3f),
        glm::vec3(0.3f, 0.0f, -0.1f),
    };

    
    Shader TexShader("./vertex.vs", "./fragment_generic_tex.fs");
    
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
    unsigned int cubeTexture = loadTexture("../res/container.jpg", GL_CLAMP_TO_EDGE);
    Texture cube_tex = {cubeTexture, "../res/container.jpg", "texture_diffuse"};
    Mesh M_cube = Mesh(cubeVerts, cubeInts, std::vector<Texture>{cube_tex});

    // pack model
    Shader ModelShader("./vertex.vs", "./fragment_model.fs");
    ModelShader.use();
    ModelShader.setInt("skybox", 0);
    Model pack = Model("../res/backpack/backpack.obj");

    // transparencies
    Shader trShader = Shader("./vertex.vs", "./fragment_transparency.fs");
    unsigned int trVAO, trVBO;
    glGenVertexArrays(1, &trVAO);
    glGenBuffers(1, &trVBO);
    glBindVertexArray(trVAO);
    glBindBuffer(GL_ARRAY_BUFFER, trVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVerticePostions) + sizeof(transparentVerticeUVs), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(transparentVerticePostions), &transparentVerticePostions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(transparentVerticePostions), sizeof(transparentVerticeUVs), &transparentVerticeUVs);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(transparentVerticePostions)));
    glBindVertexArray(0);
    unsigned int transparency = loadTexture("../res/blending_transparent_window.png", GL_CLAMP_TO_EDGE);

    // cube maps
    std::vector<std::string> cubemap_paths = {
        "../res/skybox/right.jpg",
        "../res/skybox/left.jpg",
        "../res/skybox/top.jpg",
        "../res/skybox/bottom.jpg",
        "../res/skybox/front.jpg",
        "../res/skybox/back.jpg"
    };
    unsigned int cubemap = loadCubeMap(cubemap_paths);
    Shader skybox_shader = Shader("./vertex_cubemap.vs", "./fragment_cubemap.fs");
    skybox_shader.use();
    skybox_shader.setInt("cubemap", 0);
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glBindVertexArray(0);
    
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

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cam.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), ( (float)window_width / (float)window_height ), 0.1f, 100.0f);
        
        glDepthMask(GL_FALSE);
        skybox_shader.use();
        glm::mat4 skybox_view = glm::mat4(glm::mat3(cam.GetViewMatrix()));
        skybox_shader.setMat4("view", skybox_view);
        skybox_shader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        TexShader.use();
        TexShader.setMat4("model", model);
        TexShader.setMat4("view", view);
        TexShader.setMat4("projection", projection);
        M_floor.Draw(TexShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.2f));
        TexShader.setMat4("model", model);
        M_cube.Draw(TexShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.2f, 0.3f, 0.5f));
        TexShader.setMat4("model", model);
        M_cube.Draw(TexShader);

        ModelShader.use();
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.2f));
        model = glm::rotate(model, sin(currentFrame), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.0f));
        ModelShader.setMat4("view", view);
        ModelShader.setMat4("projection", projection);
        ModelShader.setMat4("model", model);
        ModelShader.setVec3("cameraPos", cam.position);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        pack.Draw(ModelShader); 

        glDisable(GL_CULL_FACE);
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
        glEnable(GL_CULL_FACE);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        fbq_shader.use();
        glBindVertexArray(fbqVAO);
        glBindTexture(GL_TEXTURE_2D, fbo_texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &trVAO);
    glDeleteBuffers(1, &trVBO);
    glDeleteVertexArrays(1, &fbqVAO);
    glDeleteBuffers(1, &fbqVBO);

    glDeleteFramebuffers(1, &fbo);
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

unsigned int loadCubeMap(std::vector<std::string> faces){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++){
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else {
            std::cerr << "Cube map failed to load at path: " << faces[i] << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;
}