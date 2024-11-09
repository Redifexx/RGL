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
#include "camera.h"
#include "primitives.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "world.h"

// Resizeable Window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Input
void processInput(GLFWwindow *window);

// Mouse Cursor
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//Scroll Callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Texture helper function
unsigned int loadTexture(char const* path);

void TakeScreenshot();


// GLOBAL VARIABLES

// Screen Resolution
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
float fpsCap = 120.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// mouse cursor initialization + CAM
Camera camera(glm::vec3(0.0f, 18.0f, 0.0f));

//Cursor Lock
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Delta Time
double deltaTime_ = 0.0f;
double lastTime_ = 0.0f;
void calculateDeltaTime()
{
    double curTime = glfwGetTime();
    deltaTime_ = curTime - lastTime_;
    lastTime_ = curTime;
}


//Light Positions
//glm::vec3 lightPos(8.0f, 1.0f, 8.0f);
//glm::vec3 cubePos(0.0f, 0.0f, 0.0f);

//FUTURE OPTIMIZATION: DO MODEL, VIEW,  PROJECTION ON CPU

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

    // glfw window creation
    // --------------------
    glfwWindowHint(GLFW_SAMPLES, 0);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RGL (Dev Build 0.0.2)", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback); 
    glfwSetScrollCallback(window, scroll_callback); 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    // IMGUI SETUP
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../fonts/Monocraft.ttf", 20.0f);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0.0f, 0.0f);
    style.FramePadding = ImVec2(0.0f, 0.0f);
    style.ItemSpacing = ImVec2(0.0f, 0.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.0f);
    style.WindowBorderSize = 0.0f;
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    Shader phongShader("../shaders/source.vs", "../shaders/source.fs");
    Shader modelShader("../shaders/modelLoad.vs", "../shaders/modelLoad.fs");
    Shader stencilShader("../shaders/objOutline.vs", "../shaders/objOutline.fs");
    Shader lightCubeShader("../shaders/lightCube.vs", "../shaders/lightCube.fs");


    //Model backpack("../models/backpack/backpack.obj");
    //Model backpack("../models/gman-toilet/source/GmanToilet.fbx");


    ////EBO -- disabled for cube
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// color attribute
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    //glEnableVertexAttribArray(1);
    // texture attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);
    //glBindVertexArray(VAO);


    ////VBO
    //unsigned int VBO;
    //glGenBuffers(1, &VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
//
    ////Light VAO
    //unsigned int lightVAO;
    //glGenVertexArrays(1, &lightVAO);
    //glBindVertexArray(lightVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5* sizeof(float)));
    //glEnableVertexAttribArray(2);
//
    //unsigned int lightCubeVAO;
    //glGenVertexArrays(1, &lightCubeVAO);
    //glBindVertexArray(lightCubeVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //GL Settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE); 
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    //Stencil
    //glEnable(GL_STENCIL_TEST);
    //glStencilFunc(GL_ALWAYS, 1, 0xFF);

    glfwSwapInterval(0); // Disables VSync

    //Texture - Depricated
    unsigned int diffuseMap = loadTexture("../textures/mc_ss.png");
    unsigned int specularMap = loadTexture("../textures/mc_ss.png");
    
    phongShader.use();
    glUniform1i(glGetUniformLocation(phongShader.ID, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(phongShader.ID, "material.specularMap"), 1);
    //glUniform1i(glGetUniformLocation(lightingShader.ID, "material.specularMap"), 1);
    //glUniform1i(glGetUniformLocation(lightingShader.ID, "material.emissionMap"), 2);
    

    double lasttime = glfwGetTime();
    int frameCount = 0;
    double totalFPS = 0;
    double totalRenderTime = 0.0;

    //IMGUI Stuff
    std::string fpsText = "";
    std::string xText = ""; 
    std::string yText = ""; 
    std::string zText = "";

    //Minecraft Stuff
    std::cout << "CREATING WORLD OBJECT" << endl;
    World myWorld; //Generates World
    myWorld.SetupChunkLoader();
    std::cout << "FINISHED WORLD OBJECT" << endl;
    


    //MAIN RENDER LOOP------------------------------------------------------------------------------------------------------------------------------------------------------------
    std::cout << "RENDER LOOP!" << std::endl;
    while(!glfwWindowShouldClose(window))
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        //Process Input
        processInput(window);

        //fps capture
        calculateDeltaTime();   
        double fps = 1.0 / deltaTime_;
        totalFPS += fps;
        frameCount++;
        if (frameCount >= 30)
        {
            fpsText = "FPS: " + std::to_string(totalFPS / frameCount);
            frameCount = 0;
            totalFPS = 0;
            totalRenderTime = 0.0;
        }

        xText = "X: " + std::to_string(camera.Position.x);
        yText = "Y: " + std::to_string(camera.Position.y);
        zText = "Z: " + std::to_string(camera.Position.z);

        //Rendering Commands
        glClearColor(0.333f, 0.816f, 0.988f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoDecoration; 
        window_flags |= ImGuiWindowFlags_NoResize;

        ImGui::SetNextWindowSize(ImVec2(900, 900)); 
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        bool * open_ptr = nullptr;
        ImGui::Begin("I'm a Window!", open_ptr, window_flags);
        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::Text(fpsText.c_str());
        ImGui::SetCursorPos(ImVec2(20, 40));
        ImGui::Text(xText.c_str());
        ImGui::SetCursorPos(ImVec2(20, 60));
        ImGui::Text(yText.c_str());
        ImGui::SetCursorPos(ImVec2(20, 80));
        ImGui::Text(zText.c_str());


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        //Render Stuff
        //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
        //glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
        //glStencilMask(0xFF); // enable writing to the stencil buffer
        phongShader.use();

        // Cam Transformations
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.001f, 1000.0f);
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(glGetUniformLocation(phongShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(phongShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(phongShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //backpack.Draw(phongShader);



        // Stencil
        //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //glStencilMask(0x00); // disable writing to the stencil buffer
        //glDisable(GL_DEPTH_TEST);
        //stencilShader.use();
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(1.0f));
        //glUniformMatrix4fv(glGetUniformLocation(stencilShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glUniformMatrix4fv(glGetUniformLocation(stencilShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //glUniformMatrix4fv(glGetUniformLocation(stencilShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //backpack.Draw(stencilShader);
        //glStencilMask(0xFF);
        //glStencilFunc(GL_ALWAYS, 1, 0xFF);   
        //glEnable(GL_DEPTH_TEST);
                
        // Material Settings
        float materialSpecFactor = 0.0f;
        float materialEmisFactor = 0.0f;
        float materialShininess = 32.0f;
        glUniform1f(glGetUniformLocation(phongShader.ID, "material.specularFactor"), materialSpecFactor);
        glUniform1f(glGetUniformLocation(phongShader.ID, "material.emissiveFactor"), materialEmisFactor);
        glUniform1f(glGetUniformLocation(phongShader.ID, "material.shininess"), materialShininess);
        
        //Light Settings
        glm::vec3 white = glm::vec3(1.0f);
        glUniform3fv(glGetUniformLocation(phongShader.ID, "spotLight.position"), 1, glm::value_ptr(camera.Position));
        glUniform3fv(glGetUniformLocation(phongShader.ID, "spotLight.direction"), 1, glm::value_ptr(camera.Front));
        glUniform3fv(glGetUniformLocation(phongShader.ID, "spotLight.color"), 1, glm::value_ptr(white));
        float iC = glm::cos(glm::radians(12.5f));
        float oC = glm::cos(glm::radians(30.5f));
        glUniform1f(glGetUniformLocation(phongShader.ID, "spotLight.innerCutOff"), iC);
        glUniform1f(glGetUniformLocation(phongShader.ID, "spotLight.outerCutOff"), oC);
        glUniform1f(glGetUniformLocation(phongShader.ID, "spotLight.intensity"), 1.0f);
        glUniform3fv(glGetUniformLocation(phongShader.ID, "viewPos"), 1, glm::value_ptr(camera.Position));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);


        //std::lock_guard<std::mutex> lock(myWorld.chunkMutex);
        //for (Chunk* chunk : myWorld.renderableChunks)
        //{
        //    if (chunk && chunk->hasGenerated) // Check if the chunk exists and is generated
        //    {
        //        //std::cout << "RENDERING" << std::endl;
        //        chunk->RenderChunk();
        //    }
        //}

        std::vector<Chunk*> chunksToRender;
        {
            std::lock_guard<std::mutex> lock(myWorld.chunkMutex);
            chunksToRender = myWorld.renderableChunks; // Copy renderable chunks list
        }

        for (Chunk* chunk : chunksToRender)
        {
            if (chunk && chunk->hasGenerated) 
            {
                chunk->RenderChunk();
            }
        }
        

        while (glfwGetTime() < lasttime + 1.0/fpsCap) {
            //FPS
        }
        lasttime += 1.0/fpsCap;

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glBindVertexArray(0);

        //check and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
        auto endTime = std::chrono::high_resolution_clock::now();
        double renderTime = std::chrono::duration<double, std::micro>(endTime - startTime).count();
        totalRenderTime += renderTime;
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &lightVAO);
    //glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    float cameraSpeed = 2.5f * deltaTime;
    bool isShiftDown = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
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
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        TakeScreenshot();
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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

unsigned int loadTexture(char const* path) 
{
    unsigned int textureID;
    glGenTextures(1, &textureID); //generates a texture

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0); //loads image
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
        {
            format = GL_RED;
        }
        else if (nrChannels == 3)
        {
            format = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            format = GL_RGBA;
        }
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //generates mipmaps :)

        // Remake Texture Atlas with padding
        //Texture Settigns
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data); // cleans memory

    return textureID;
}

unsigned int HEADER_SIZE = 18;
void TakeScreenshot()
{
    std::cout << "Screenshot!" << std::endl;
    unsigned char* buffer;
    char filename[50];
    int w = SCR_WIDTH;
    int h = SCR_HEIGHT;
    int buf_size = HEADER_SIZE + (w * h * 3);
    int i;
    unsigned char temp;
    FILE* out_file;

    //Open File for Output
    if (!(out_file = fopen("../screenshots/screenshot.tga", "wb")))
    {
        return;
    }

    //Allocate memory to read from the frame buffer
    if (!(buffer = (unsigned char*) calloc(1, buf_size)))
    {
        return;
    }

    //Set Header Info
    buffer[2] = 2;
    buffer[12] = w & 255;
    buffer[13] = w >> 8;
    buffer[14] = h & 255;
    buffer[15] = h >> 8;
    buffer[16] = 24;

    //Read the fram ebuffer
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer + HEADER_SIZE);

    //RGB TO BGR (TGA)
    for (i = HEADER_SIZE; i < buf_size; i += 3)
    {
        temp = buffer[i];
        buffer[i] = buffer[i + 2];
        buffer[i + 2] = temp;
    }

    //Write Header + Color Buf to File
    fwrite(buffer, sizeof(unsigned char), buf_size, out_file);

    //cleanup
    fclose(out_file);
    free(buffer);
}