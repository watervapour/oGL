#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <string>
#ifdef _PLAT_WIN
	#include <Windows.h>
#else

#endif

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

int main(int argc, char **argv){

	#ifdef _PLAT_WIN
		std::string basePath = std::string(argv[0]);
		int pathMarker = basePath.rfind('\\');
		basePath = (pathMarker != -1) ? basePath.substr(0, pathMarker) : ".";
		if(!SetCurrentDirectory(basePath.c_str())){
			std::cerr << "Failed to change working directory!\n";
			return -1;
		}
	#else
		std::cerr << "linux dir changing not setup!\n";
		return -1;
	#endif

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
    logo.pixels = stbi_load("./res/oGL-logo.png", &logo.width, &logo.height, 0, 4);
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

	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-right
	};

	unsigned int pointsVAO, pointsVBO;
	glGenVertexArrays(1, &pointsVAO);
	glGenBuffers(1, &pointsVBO);
	glBindVertexArray(pointsVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3));
	
	Shader pointsShader("./src/geo-simple.vs", "./src/geo-house.gs", "./src/geo-simple.fs");
	pointsShader.use();

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
        std::string windowTitle = "oGL | FOV: ";
		windowTitle+=std::to_string((int)cam.fov)+" | FPS = "+std::to_string(fpsHistoric)+", frameTime = "+std::to_string(deltaTime*1000.0f)+"ms";
        glfwSetWindowTitle(window, windowTitle.c_str());

        processInput(window);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		// main rendering section
		pointsShader.use();
		glBindVertexArray(pointsVAO);
		
		glDrawArrays(GL_POINTS, 0, 4);


		// end main rendering section

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	glDeleteVertexArrays(1, &pointsVAO);
    glDeleteBuffers(1, &pointsVBO);

    glfwTerminate();
    return 0;
}
