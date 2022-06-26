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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//void myDisplay(void) {
//    glClear(GL_COLOR_BUFFER_BIT);
//    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
//    glFlush();
//}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float mixValue = 1.0f;
unsigned int WIDTH = 800;
unsigned int HEIGHT = 600;

//float vertices[] = {
////     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
//     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
//    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
//    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
//};

float vertices[] = {
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

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
bool keys[1024];
GLfloat deltaTime = 0.0f;   // 当前帧遇上一帧的时间差
GLfloat lastFrame = 0.0f;   // 上一帧的时间
GLfloat yaw   = -90.0f;    // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch =   0.0f;
GLfloat lastX =  WIDTH  / 2.0;
GLfloat lastY =  HEIGHT / 2.0;
float aspect = 45.0f;


unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
    
//    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//    // 译注：下面就是矩阵初始化的一个例子，如果使用的是0.9.9及以上版本
//    // 下面这行代码就需要改为:
//    // glm::mat4 trans = glm::mat4(1.0f)
//    // 之后将不再进行提示
//    glm::mat4 trans;
//    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//    vec = trans * vec;
//    std::cout << vec.x << vec.y << vec.z << std::endl;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
//        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
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

//    unsigned int EBO;
//    glGenBuffers(1, &EBO);

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
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 3. 设置顶点属性指针
            // location=0 ==> 0
            //GL_FALSE => true 则所有的数据都会被映射到0和1之间
            // stride： 第二个顶点开始的地方和第一个顶点开始的地方==
            // offset：顶点开始的位移
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

//            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
//            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // 解除绑定GL_ARRAY_BUFFER，VBO
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        //解除绑定VAO
        glBindVertexArray(0);
    }

    
    Texture container("container.jpeg", GL_RGB);
    Texture awesomeface("awesomeface.png", GL_RGBA);
    

//    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

//    unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
//    {
//        glm::mat4 trans;
//        trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
//        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
//        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
//    }
//    
//    glm::mat4 model;
//    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //模型矩阵
//    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    
    glm::mat4 view;
    // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); //观察矩阵
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);//透视矩阵

//    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//
//    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//
//    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);




    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window))
    {
        // input
        // -----
        glfwPollEvents();
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        do_movement();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

//        float timeValue = glfwGetTime();
//        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
//        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container.ID);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, awesomeface.ID);
        
        shader.setFloat("mixValue", mixValue);
//        glm::mat4 model;
//        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
//        shader.setMatrix("model", model);
        
        
//        glm::mat4 trans;
//        //建议的操作顺序是：1.缩放，2.旋转，3.位移，而采用算法，则按过来
//        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f)); //位移
//        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));//2.旋转
//        trans = glm::scale(trans, glm::vec3(0.5f, 1.0f, 0.3f));//2.旋转
//        shader.setMatrix(transformLoc, trans);
        
        glBindVertexArray(VAO);
        

        projection = glm::perspective(aspect, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 100.0f);
        shader.setMatrix("projection", projection);

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        shader.setMatrix("view", view);
        
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle) , glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMatrix("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 针对GL_ELEMENT_ARRAY_BUFFER
//        glDrawArrays(GL_TRIANGLES, 0, 36); // 针对非GL_ELEMENT_ARRAY_BUFFER
        

//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{
    // Camera controls
    GLfloat cameraSpeed = 0.5f * deltaTime;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(aspect >= 1.0f && aspect <= 45.0f)
    aspect -= yoffset;
  if(aspect <= 1.0f)
    aspect = 1.0f;
  if(aspect >= 45.0f)
    aspect = 45.0f;
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
