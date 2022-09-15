//
//  Shader.hpp
//  OpenglTest
//
//  Created by 蔡永建 on 2022/6/20.
//
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

class Shader
{
public:
    // 程序ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    // 使用/激活程序
    void use();
    // uniform工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMatrix(const std::string& name, glm::mat4& mat4) const;
    void setMatrix(unsigned int location, glm::mat4& mat4) const;
    void setMat4(const std::string& name, glm::mat4& mat4) const;
    void setMat4(unsigned int location, glm::mat4& mat4) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
};

#endif
