#ifndef SHADER_H_
#define SHADER_H_

#include <glad\glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class shader{
public:
    // the program's ID
    unsigned int ID;

    // read and build shader
    shader(const char* vertexPath, const char* fragmentPath){
        // 1. retrieve source code for vertex and fragment shaders
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            // opwn files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

        } catch (std::ifstream::failure e){
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n"; 
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print any errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
        }

        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // print any errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_ERROR\n" <<
                infoLog << std::endl;
        }

        // finally make program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // print errors
        glGetShaderiv(ID, GL_LINK_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(ID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
                infoLog << std::endl;
        }

        // delete the now linked shaders
        glDeleteShader(vertex);
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
};

#endif // SHADER_H_