//
//  instancing2.cpp
//  OpenglTest
//
//  Created by caiyongjian on 2022/10/31.
//

#include "instancing2.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "utils/filesystem.h"


namespace instancing2 {

float quadVertices[] = {
    // 位置          // 颜色
    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
     0.05f,  0.05f,  0.0f, 1.0f, 1.0f
};



using std::vector;
using std::string;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path, unsigned int repeatMode);

float mixValue = 0.2f;
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  200.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
bool keys[1024];
GLfloat deltaTime = 0.0f;   // 当前帧遇上一帧的时间差
GLfloat lastFrame = 0.0f;   // 上一帧的时间
GLfloat lastX =  SCR_WIDTH  / 2.0;
GLfloat lastY =  SCR_HEIGHT / 2.0;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Camera camera(cameraPos);

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


int instancing_main() {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    
    glEnable(GL_DEPTH_TEST);
        
    // build and compile shaders
     // -------------------------
    Shader shader("instancing2_vs.vs", "instancing2_fs.fs");
    Shader asteroidShader("instancing2_asteroids_vs.vs", "instancing2_asteroids_fs.fs");
    
    // model
    Assimp::Model rock(FileSystem::getPath("resources/objects/rock/rock.obj"));
    Assimp::Model planet(FileSystem::getPath("resources/objects/planet/planet.obj"));
    
    // generate a large list of semi-random model transformation matrices
        // ------------------------------------------------------------------
        unsigned int amount = 100000;
        glm::mat4* modelMatrices;
        modelMatrices = new glm::mat4[amount];
        srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
        float radius = 150.0;
        float offset = 25.0f;
        for (unsigned int i = 0; i < amount; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            // 1. translation: displace along circle with 'radius' in range [-offset, offset]
            float angle = (float)i / (float)amount * 360.0f;
            float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. scale: Scale between 0.05 and 0.25f
            float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
            model = glm::scale(model, glm::vec3(scale));

            // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rotAngle = static_cast<float>((rand() % 360));
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. now add to list of matrices
            modelMatrices[i] = model;
        }
    
    // 顶点缓冲对象
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for(unsigned int i = 0; i < rock.meshes.size(); i++)
    {
        unsigned int VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        // 顶点属性
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }


    
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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        asteroidShader.use();
        asteroidShader.setMat4("projection", projection);
        asteroidShader.setMat4("view", view);
        
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        shader.setMat4("model", model);
        planet.Draw(shader);
        
        // 绘制小行星
        asteroidShader.use();
        asteroidShader.setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
        unsigned int meshes_size = rock.meshes.size();
        for (unsigned int i = 0; i < meshes_size; i++)
        {
            glBindVertexArray(rock.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rock.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
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
       if(keys[GLFW_KEY_W])
           camera.ProcessKeyboard(FORWARD, deltaTime);
       if(keys[GLFW_KEY_S])
           camera.ProcessKeyboard(BACKWARD, deltaTime);
       if(keys[GLFW_KEY_A])
           camera.ProcessKeyboard(LEFT, deltaTime);
       if(keys[GLFW_KEY_D])
           camera.ProcessKeyboard(RIGHT, deltaTime);
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path, unsigned int repeatMode)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


}

