#define GLFW_INCLUDE_NONE
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "model.h" //
#include "camera.h" // 
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


// Resizeable Window
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Mouse Cursor
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//Scroll Callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Input - to be made in class
void processInput(GLFWwindow *window);

// Global Variables
//-----------------------
int* SCR_WIDTH = new int;
int* SCR_HEIGHT = new int;
float m_width = 0.0f;
float m_height = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float fpsCap = 170.0f;
float lastX = *SCR_WIDTH / 2.0f;
float lastY = *SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
void calculateDeltaTime()
{
    double curTime = glfwGetTime();
    deltaTime = curTime - lastFrame;
    lastFrame = curTime;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__ //If on MacOS
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation and setup
    // --------------------
    glfwWindowHint(GLFW_SAMPLES, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "RedifexxGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwGetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);   
    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback); 
    glfwSetScrollCallback(window, scroll_callback); 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    //GL Settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);

    glfwSwapInterval(0); // Disables VSync

    //Scene Setup
    Shader shader("../shaders/source.vs", "../shaders/simplePhong.fs");
    Model dolphins("../model/monke.obj");


    double lasttime = glfwGetTime();
    int frameCount = 0;
    double totalFPS = 0;
    double totalRenderTime = 0.0;

    //RENDER LOOP
    //----------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.439f, 0.439f, 0.439f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //Starts Timer
        auto startTime = std::chrono::high_resolution_clock::now();

        //Process Input
        processInput(window);

        calculateDeltaTime();
        double fps = 1.0 / deltaTime;
        totalFPS += fps;
        frameCount++;
        if (frameCount >= 30)
        {
            std::cout << "FPS: " + std::to_string(totalFPS / frameCount) << std::endl;
            frameCount = 0;
            totalFPS = 0;
            totalRenderTime = 0.0;
        }

        //Render
        //---------
        // Set up Camera Matrix Transforms
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)*SCR_WIDTH / (float)*SCR_HEIGHT, 0.001f, 1000.0f);
        glm::mat4 model = glm::mat4(1.0f);

        shader.use();

        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Material Settings for Phong Shader
        glm::vec3 diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 emissiveColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(glGetUniformLocation(shader.ID, "defaultMaterial.diffuseColor"),  1,  glm::value_ptr(diffuseColor));
        glUniform3fv(glGetUniformLocation(shader.ID, "defaultMaterial.specularColor"), 1, glm::value_ptr(specularColor));
        glUniform3fv(glGetUniformLocation(shader.ID, "defaultMaterial.emissiveColor"), 1, glm::value_ptr(emissiveColor));
        float materialSpecFactor = 0.0f;
        float materialEmisFactor = 0.0f;
        float materialShininess = 32.0f;
        glUniform1f(glGetUniformLocation(shader.ID, "defaultMaterial.specularFactor"), materialSpecFactor);
        glUniform1f(glGetUniformLocation(shader.ID, "defaultMaterial.emissiveFactor"), materialEmisFactor);
        glUniform1f(glGetUniformLocation(shader.ID, "defaultMaterial.shininess"), materialShininess);
        
        // Sets up Spot Light for Camera
        glm::vec3 white = glm::vec3(1.0f);
        glUniform3fv(glGetUniformLocation(shader.ID, "spotLight.position"), 1, glm::value_ptr(camera.Position));
        glUniform3fv(glGetUniformLocation(shader.ID, "spotLight.direction"), 1, glm::value_ptr(camera.Front));
        glUniform3fv(glGetUniformLocation(shader.ID, "spotLight.color"), 1, glm::value_ptr(white));
        float iC = glm::cos(glm::radians(12.5f));
        float oC = glm::cos(glm::radians(30.5f));
        glUniform1f(glGetUniformLocation(shader.ID, "spotLight.innerCutOff"), iC);
        glUniform1f(glGetUniformLocation(shader.ID, "spotLight.outerCutOff"), oC);
        glUniform1f(glGetUniformLocation(shader.ID, "spotLight.intensity"), 1.0f);
        glUniform3fv(glGetUniformLocation(shader.ID, "viewPos"), 1, glm::value_ptr(camera.Position));

        dolphins.Draw(shader);

        while (glfwGetTime() < lasttime + 1.0/fpsCap) {
            //FPS Enforcer
        }
        lasttime += 1.0/fpsCap;

        //check and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
        auto endTime = std::chrono::high_resolution_clock::now();
        double renderTime = std::chrono::duration<double, std::micro>(endTime - startTime).count();
        totalRenderTime += renderTime;
    }

    glfwTerminate();
    return 0;
}

void glfwSetWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    *SCR_WIDTH = width;
    *SCR_HEIGHT = height;

    if (m_height > *SCR_HEIGHT)
    {
        m_height = *SCR_HEIGHT;
    }

    if (m_width > *SCR_WIDTH)
    {
        m_width = *SCR_WIDTH;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    float cameraSpeed = 20.5f * deltaTime;
    bool isShiftDown = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        isShiftDown = true;
    }
    else
    {
        isShiftDown = false;
    }

    //Walking
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime, isShiftDown);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime, isShiftDown);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime, isShiftDown);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime, isShiftDown);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime, isShiftDown);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime, isShiftDown);
    }
}