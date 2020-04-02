#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
private:
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    GLuint programID;

public:
    Shader(void);
    ~Shader(void);

    void load(const char* vertexShaderPath, const char* fragmentShaderPath);
    void use(void);
    void sendUniformMat4(const GLchar* name, glm::mat4 src);
    void sendUniformVec3(const GLchar* name, glm::vec3 src);
    GLuint getProgramID(void);
};

#endif // SHADER_HPP_