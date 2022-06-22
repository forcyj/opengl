//
//  main.cpp
//  OpenglTest
//
//  Created by 蔡永建 on 2022/1/6.
//

#include <stdio.h>

//#include <GLUT/GLUT.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "Shader.hpp"
#include "Texture.hpp"

//void myDisplay(void) {
//    glClear(GL_COLOR_BUFFER_BIT);
//    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
//    glFlush();
//}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float mixValue = 0.0f;

float vertices[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
};

unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

void checkError(unsigned int target, unsigned int status) {
    int  success;
    char infoLog[512];
    glGetShaderiv(target, status, &success);
    if(!success)
    {
        glGetShaderInfoLog(target, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}


int main(int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
//        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // preprare
    Shader shader("vertex.glsl", "fragment.glsl");

    shader.use();
//    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
    //glUniform1i设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    
    // 创建VAO/VBO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    {
        //相当于分配两组内存，VAO保存指针，可以简单获取每个顶点的指针，VBO是顶点的指针的具体位置==，
        // 后续可以直接绑定VAO就可以访问VBO中的顶点位置了。
        // 另外，后面也可以更新VBO来更新顶点位置===
        // 绑定VAO
        glBindVertexArray(VAO);
        {
            //把顶点数组复制到缓冲中供OpenGL使用
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            //把index数组复制到缓冲中
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 3. 设置顶点属性指针
            // location=0 ==> 0
            //GL_FALSE => true 则所有的数据都会被映射到0和1之间
            // stride： 第二个顶点开始的地方和第一个顶点开始的地方==
            // offset：顶点开始的位移
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            // 解除绑定GL_ARRAY_BUFFER，VBO
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        //解除绑定VAO
        glBindVertexArray(0);
    }

    
    Texture container("container.jpeg", GL_RGB);
    Texture awesomeface("awesomeface.png", GL_RGBA);
    

//    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");


    while(!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

//        float timeValue = glfwGetTime();
//        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
//        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container.ID);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, awesomeface.ID);
        
        shader.setFloat("mixValue", mixValue);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
//    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//    glutInitWindowPosition(100, 100);
//    glutInitWindowSize(400, 400);
//    glutCreateWindow("第一个 OpenGL 程序");
//    glutDisplayFunc(&myDisplay);
//    glutMainLoop();
//    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      {
          mixValue += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
          if(mixValue >= 1.0f)
              mixValue = 1.0f;
      }
      if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      {
          mixValue -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
          if (mixValue <= 0.0f)
              mixValue = 0.0f;
      }
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
