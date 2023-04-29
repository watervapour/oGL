#ifndef SHADER_H
#define SHADER_H

#include <glad\glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
public:
    // the program's ID
    unsigned int ID;

    // read and build shader
    Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath){
        // 1. retrieve source code for vertex and fragment shaders
        std::string vertexCode;
        std::string geometryCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream gShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            // opwn files
            vShaderFile.open(vertexPath);
            gShaderFile.open(geometryPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, gShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            gShaderStream << gShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            gShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            geometryCode = gShaderStream.str();
            fragmentCode = fShaderStream.str();

        } catch (std::ifstream::failure e){
			std::cerr << "File vertexPath: " << vertexPath << ", geometryPath: " << geometryPath << ", fragmentPath: " << fragmentPath; 
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n"; 
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* gShaderCode = geometryCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, geometry, fragment;
        int success;
        char infoLog[512];

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print any errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success){
			std::cerr << "File vertexPath: " << vertexPath <<"\n"; 
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
        }

        // geometry shader
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        // print any errors
        glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
        if (!success){
			std::cerr << "File geometryPath: " << geometryPath <<"\n"; 
            glGetShaderInfoLog(geometry, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_ERROR\n" << 
				infoLog << std::endl;
        }
        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // print any errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success){
			std::cerr << "File fragmentPath: " << fragmentPath <<"\n"; 
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_ERROR\n" <<
                infoLog << std::endl;
        }

        // finally make program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, geometry);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // print errors
        glGetShaderiv(ID, GL_LINK_STATUS, &success);
        if (!success){
			std::cerr << "File vertexPath: " << vertexPath << ", geometry: " << geometryPath << ", fragmentPath: " << fragmentPath; 
            glGetShaderInfoLog(ID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
                infoLog << std::endl;
        }

        // delete the now linked shaders
        glDeleteShader(vertex);
        glDeleteShader(geometry);
        glDeleteShader(fragment);
    }

    // use/activate shader
    void use(){
        glUseProgram(ID);
    }

    // utility uniform functions
    void setBool(const std::string &name, bool value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const{
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec3(const std::string &name, float x, float y, float z) const{
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    void setVec3(const std::string &name, glm::vec3 &vec){
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};

#endif // SHADER_H
