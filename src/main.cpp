// template based on material from learnopengl.com
//#define GLEW_STATIC
//#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include "models.h" //Mesh Class
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
double lastTime_ = 0.0;
double deltaTime_ = 0.0;
bool isGPUcalc = true;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        isGPUcalc = !isGPUcalc;
    }
}

void calculateDeltaTime()
{
    double curTime = glfwGetTime();
    deltaTime_ = curTime - lastTime_;
    lastTime_ = curTime;
}

void toggleAA()
{
    if (glIsEnabled(GL_MULTISAMPLE))
    {
        glDisable(GL_MULTISAMPLE);
    }
    else
    {
        glEnable(GL_MULTISAMPLE);
    }
}

// Screen Resolution
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

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
    glfwWindowHint(GLFW_SAMPLES, 8);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RGL (Dev Build 0.0.2)", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    

    // // glew: load all OpenGL function pointers
    //glewInit();

    // IMGUI SETUP
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../fonts/F25_Bank_Printer.ttf", 13.0f);

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    //Projection Cam
    float fov = glm::radians(60.0f); //Camera FOV
    float aspectRatio = SCR_WIDTH / (float)SCR_HEIGHT;
    float nearClip = 0.1f;
    float farClip = 10.0f; // Far Camera Plane
    glm::mat4 projection = glm::perspective(fov, aspectRatio, nearClip, farClip);

    //Orthographic Cam
    float left = -(SCR_WIDTH / 200.0);
    float right = (SCR_WIDTH / 200.0);
    float top = -(SCR_HEIGHT / 200.0);
    float bottom = (SCR_HEIGHT / 200.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glCullFace(GL_FRONT);

    // build and compile our shader program
    // ------------------------------------
    // Can Later Be Abstracted
    Shader* phongShader = new Shader("../shaders/phong.vert", "../shaders/phong.frag");
    Shader* depthShader = new Shader("../shaders/phong.vert", "../shaders/depth.frag");
    Shader* normalShader = new Shader("../shaders/phong.vert", "../shaders/normal.frag");
    Shader* flatShader = new Shader("../shaders/flat.vert", "../shaders/flat.frag");
    Shader* gouraudShader = new Shader("../shaders/gouraud.vert", "../shaders/gouraud.frag");

    //Created a model object above main
    Model cube("../model/cube.obj", phongShader);
    vector<Model*> allModels;
    allModels.push_back(&cube);

    for (int i = 0; i < allModels.size(); i++)
    {
        allModels.at(i)->projectionMatrix = projection;
        allModels.at(i)->nearClip = nearClip;
        allModels.at(i)->farClip = farClip;
    }

    glfwSetKeyCallback(window, keyCallback);
    
    // render loop
    // -----------
    double lasttime = glfwGetTime();
    int frameCount = 0;
    double totalFPS = 0;
    double totalRenderTime = 0.0;

    //IMGUI Pointer Variables
    string fpsText = "";
    float camDeg = glm::degrees(fov);
    float* camFOVDeg = &camDeg;
    float* aspectRatioSetting = &aspectRatio;
    float* nearClipSetting = &nearClip;
    float* farClipSetting = &farClip;

    float* leftSetting = &left;
    float* rightSetting = &right;
    float* topSetting = &top;
    float* bottomSetting = &bottom;

    float* orthoScale = new float(1.0f);
    int aaSampleCount = 4;
    int *aaSC = &aaSampleCount;
    int nSW = SCR_WIDTH;
    int nSH = SCR_HEIGHT;
    int* newScreenWidth = &nSW;
    int* newScreenHeight = &nSH;
    float newFOV;
    bool isPerspective = true;
    bool isFlat = false;
    bool hasAA = false;
    if (glIsEnabled(GL_MULTISAMPLE))
    {
        hasAA = true;
    }
    glEnable(GL_MULTISAMPLE);
    ImVec4 bckColor = ImVec4(50.0/255.0, 50.0/255.0, 50.0/255.0, 1.0); //Background Color
    Model* curModel = &cube;
    
    //MAIN LOOP
    while (!glfwWindowShouldClose(window))
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        // input
        processInput(window);

        calculateDeltaTime();   
        double fps = 1.0 / deltaTime_;
        totalFPS += fps;
        frameCount++;
        if (frameCount >= 300)
        {
            //cout << "FPS: " << totalFPS / frameCount << " | Avg Render Time per Frame (1/1,000,000 of sec)): " << totalRenderTime / frameCount << endl;
            fpsText = "FPS: " + to_string(totalFPS / frameCount);
            frameCount = 0;
            totalFPS = 0;
            totalRenderTime = 0.0;
        }

        // render
        glClearColor(bckColor.x, bckColor.y, bckColor.z, bckColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //IMGUI SETUP
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        curModel->Draw(window);

        ImGui::Begin("RGL Settings");
        ImGui::Text("-Current Model-");
        ImGui::Columns(1, "Model");
        if (ImGui::Button("Feyd"))
        {
            curModel = &cube;
        }   
        ImGui::Columns(1);
        ImGui::Text(fpsText.c_str());
        string modelPosition = "Model Pos: " + to_string(curModel->modelPos.x) + " " + to_string(curModel->modelPos.y) + " " + to_string(curModel->modelPos.z);
        string modelRotation = "Model Rotation: " + to_string(curModel->modelRot.x) + " " + to_string(curModel->modelRot.y) + " " + to_string(curModel->modelRot.z);
        string modelScale = "Model Scale: " + to_string(curModel->modelScale.x) + " " + to_string(curModel->modelScale.y) + " " + to_string(curModel->modelScale.z);
        ImGui::Text(modelPosition.c_str());
        ImGui::Text(modelRotation.c_str());
        ImGui::Text(modelScale.c_str());
        if (ImGui::Button("Reset Model Position & Scale"))
        {
            curModel->resetModel();
        } 
        ImGui::ColorEdit4("Background Color", (float*)&bckColor);
        ImGui::Text("-Render Mode-");
        ImGui::Columns(3, "RenderMode");
        if (ImGui::Button("Faces"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }   
        ImGui::NextColumn();
        if (ImGui::Button("Edges"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }   
        ImGui::NextColumn();
        if (ImGui::Button("Vertices"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }   
        ImGui::Columns(1);
        ImGui::Text("-Shader Selection-");
        ImGui::Columns(5, "ShaderMode");
        if (ImGui::Button("Depth"))
        {
            for (int i = 0; i < allModels.size(); i++)
            {
                allModels.at(i)->updateShader(depthShader);
            }
        }   
        ImGui::NextColumn();
        if (ImGui::Button("Normal"))
        {
            for (int i = 0; i < allModels.size(); i++)
            {
                allModels.at(i)->updateShader(normalShader);
            }
        }   
        ImGui::NextColumn();
        if (ImGui::Button("Phong"))
        {
            for (int i = 0; i < allModels.size(); i++)
            {
                allModels.at(i)->updateShader(phongShader);
            }
        }  
        ImGui::NextColumn();
        if (ImGui::Button("Gouraud"))
        {
            for (int i = 0; i < allModels.size(); i++)
            {
                allModels.at(i)->updateShader(gouraudShader);
            }
        }    
        ImGui::NextColumn();
        if (ImGui::Button("Flat"))
        {
            for (int i = 0; i < allModels.size(); i++)
            {
                allModels.at(i)->updateShader(flatShader);
            }
        }   
        ImGui::Columns(1);
        ImGui::Text("-Cam Settings-");
        if (isPerspective)
        {
            if (ImGui::Button("Switch to Orthographic"))
            {
                projection = glm::ortho(*leftSetting * (1 / *orthoScale), *rightSetting * (1 / *orthoScale), *topSetting * (1 / *orthoScale), *bottomSetting * (1 / *orthoScale), *nearClipSetting, *farClipSetting);
                isPerspective = false;
                for (int i = 0; i < allModels.size(); i++)
                {
                    allModels.at(i)->projectionMatrix = projection;
                    allModels.at(i)->isOrtho = (!isPerspective);
                }
            }   
            ImGui::InputFloat("FOV", camFOVDeg);
            ImGui::InputFloat("Aspect Ratio", aspectRatioSetting);
            ImGui::InputFloat("Near Clip", nearClipSetting);
            ImGui::InputFloat("Far Clip", farClipSetting);
            if (ImGui::Button("Update Cam Settings"))
            {
                projection = glm::perspective(glm::radians(*camFOVDeg), *aspectRatioSetting, *nearClipSetting, *farClipSetting);
                for (int i = 0; i < allModels.size(); i++)
                {
                    allModels.at(i)->projectionMatrix = projection;
                    allModels.at(i)->nearClip = nearClip;
                    allModels.at(i)->farClip = farClip;
                }
            }   
        }
        else
        {
            if (ImGui::Button("Switch to Perspective"))
            {
                projection = glm::perspective(glm::radians(*camFOVDeg), *aspectRatioSetting, *nearClipSetting, *farClipSetting);
                isPerspective = true;
                for (int i = 0; i < allModels.size(); i++)
                {
                    allModels.at(i)->projectionMatrix = projection;
                    allModels.at(i)->isOrtho = (!isPerspective);
                }
            }   
            ImGui::InputFloat("Orthographic Scale", orthoScale);
            ImGui::InputFloat("Near Clip", nearClipSetting);
            ImGui::InputFloat("Far Clip", farClipSetting);
            if (ImGui::Button("Update Cam Settings"))
            {
                projection = glm::ortho(*leftSetting * (1 / *orthoScale), *rightSetting * (1 / *orthoScale), *topSetting * (1 / *orthoScale), *bottomSetting * (1 / *orthoScale), *nearClipSetting, *farClipSetting);
                for (int i = 0; i < allModels.size(); i++)
                {
                    allModels.at(i)->projectionMatrix = projection;
                    allModels.at(i)->nearClip = nearClip;
                    allModels.at(i)->farClip = farClip;
                }
            }   
        }

        //ImGui::Text(to_string(*camFOVDeg).c_str());
        ImGui::Text("-Window Settings-");
        ImGui::InputInt("Screen Width", newScreenWidth);
        ImGui::InputInt("Screen Height", newScreenHeight);
        if (ImGui::Button("Update Window Size"))
        {
            glfwSetWindowSize(window, *newScreenWidth, *newScreenHeight);
        }   
        if (ImGui::Button("Fix Aspect Ratio"))
        {
            if (isPerspective)
            {
                float ar = (*newScreenWidth)/float(*newScreenHeight);
                aspectRatioSetting = &ar;
                projection = glm::perspective(glm::radians(*camFOVDeg), *aspectRatioSetting, *nearClipSetting, *farClipSetting);
                for (int i = 0; i < allModels.size(); i++)
                {
                    allModels.at(i)->projectionMatrix = projection;
                }
            }
            else
            {
                left = -(SCR_WIDTH / 200.0);
                right = (SCR_WIDTH / 200.0);
                top = -(SCR_HEIGHT / 200.0);
                bottom = (SCR_HEIGHT / 200.0);
                leftSetting = &left;
                rightSetting = &right;
                topSetting = &top;
                bottomSetting = &bottom;
                projection = glm::ortho(*leftSetting * (1 / *orthoScale), *rightSetting * (1 / *orthoScale), *topSetting * (1 / *orthoScale), *bottomSetting * (1 / *orthoScale), *nearClipSetting, *farClipSetting);
                for (int i = 0; i < allModels.size(); i++)
                {
                    allModels.at(i)->projectionMatrix = projection;
                }
            }
        }
        ImGui::Text("-Depth Testing-");
        ImGui::Columns(2, "Depth Testing");
        if (ImGui::Button("On"))
        {
            glEnable(GL_DEPTH_TEST);
        }
        ImGui::NextColumn();
        if (ImGui::Button("Off"))
        {
            glDisable(GL_DEPTH_TEST);
        }
        ImGui::Columns(1);
        if (ImGui::Button("Exit"))
        {
            glfwSetWindowShouldClose(window, true);
        }      
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glBindVertexArray(0);
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        auto endTime = std::chrono::high_resolution_clock::now();
        double renderTime = std::chrono::duration<double, std::micro>(endTime - startTime).count();
        totalRenderTime += renderTime;
        // This is for limiting fps
        while (glfwGetTime() < lasttime + 1.0/170.0) {
            //FPS
        }
        lasttime += 1.0/170.0;
    }
    delete orthoScale;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //model.scaleModel(glm::vec3(yoffset / 10.f, yoffset / 10.f, yoffset / 10.f));
}