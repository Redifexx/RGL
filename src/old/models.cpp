#include "models.h"

Model::Model(string path, Shader* shader)
{
    //cout << "Adding Mesh!" << endl;
    addMesh(path);
    modelPos = glm::vec3(0.0f, 0.0f, 0.0f);
    modelScale = glm::vec3(1.0f, 1.0f, 1.0f);
    modelRot = glm::vec3(0.0f, 0.0f, 0.0f);
    this->nearClip = 0.0f;
    this->farClip = 0.0f;
    this->isFlat = false;
    this->isOrtho = false;
    //cout << "Shader Activate!" << endl;
    shader->Activate();
    //cout << "Model Matrix!" << endl;
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(this->modelMatrix));
    //cout << "World Matrix!" << endl;
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "worldMatrix"), 1, GL_FALSE, glm::value_ptr(this->worldMatrix));
    curShader = shader;
    this->moveModel(glm::vec3(0.0f, 0.0f, -5.0f));
    //cout << "Model Contstructed!" << endl;
}

Model::~Model()
{
    for (int i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
}

void Model::addMesh(string path)
{
    //cout << "Creating new Mesh!" << endl;
    Mesh* curMesh = new Mesh(path);
    //cout << "Pushing Back Mesh!" << endl;
    meshes.push_back(curMesh);
}

void Model::Draw(GLFWwindow* window)
{
    curShader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(curShader->ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(this->modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(curShader->ID, "worldMatrix"), 1, GL_FALSE, glm::value_ptr(this->worldMatrix));
    glUniformMatrix4fv(glGetUniformLocation(curShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
    GLint nearClipLoc = glGetUniformLocation(curShader->ID, "nearClip");
    GLint farClipLoc = glGetUniformLocation(curShader->ID, "farClip");
    GLint isOrthoLoc = glGetUniformLocation(curShader->ID, "isOrtho");
    if (nearClipLoc != -1)
    {
        glUniform1f(nearClipLoc, this->nearClip);
        //cout << "SETTING NEAR CLIP" << endl;
    }
    if (farClipLoc != -1)
    {
        glUniform1f(farClipLoc, this->farClip);
        //cout << "SETTING FAR CLIP" << endl;
    }
    if (isOrthoLoc != -1)
    {
        glUniform1i(isOrthoLoc, this->isOrtho);
    }
    getInput(window);
    for (Mesh* curMesh : meshes)
    {
        if (localTransformations)
        {
            curMesh->updateVertices();
            //cout << "LOCAL" << endl;
        }
        curMesh->Draw();
    }
}

void Model::enableCPUCalc()
{
    localTransformations = true;
    for (Mesh* curMesh : meshes)
    {
        for (Vertex& vertex : curMesh->vertices)
        {
            Vertex curVert;
            curVert.Position = vertex.Position;
            curVert.Normal = vertex.Normal;
            curVert.TexCoords = vertex.TexCoords;
            curMesh->originalVertices.push_back(curVert);
        }
    }
}

void Model::applyMatrixLocally()
{
    //cout << modelRot.x << " " << modelRot.y << " " << modelRot.z << endl;
    for (Mesh* curMesh : meshes)
    {
        int pos = 0;
        vector<Vertex> newVert;
        for (Vertex& vertex : curMesh->vertices)
        {
            Vertex curVert;
            curVert.Position = worldMatrix * modelMatrix * glm::vec4(curMesh->originalVertices[pos].Position, 1.0f);
            glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(worldMatrix * modelMatrix)));
            curVert.Normal = glm::normalize(normalMatrix * curMesh->originalVertices[pos].Normal);
            //cout << "NORMAL " << curVert.Normal.x << " " << curVert.Normal.y << " " << curVert.Normal.z << endl;
            curVert.TexCoords = curMesh->originalVertices[pos].TexCoords;
            newVert.push_back(curVert);
            pos++;
        }
        curMesh->vertices = newVert;
    }
    //modelMatrix = glm::mat4(1.0f);
    //projectionMatrix = glm::mat4(1.0f);
}

void Model::getInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->moveModelLocal(glm::vec3(0.0f, 0.005f, 0.0f));
        }
        else
        {
            this->moveModel(glm::vec3(0.0f, 0.005f, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->moveModelLocal(glm::vec3(0.0f, -0.005f, 0.0f));
        }
        else
        {
            this->moveModel(glm::vec3(0.0f, -0.005f, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->moveModelLocal(glm::vec3(-0.005f, 0.0f, 0.0f));
        }
        else
        {
            this->moveModel(glm::vec3(-0.005f, 0.0f, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->moveModelLocal(glm::vec3(0.005f, 0.0f, 0.0f));
        }
        else
        {
            this->moveModel(glm::vec3(0.005f, 0.0f, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->moveModelLocal(glm::vec3(0.0f, 0.0f, 0.05f));
        }
        else
        {
            this->moveModel(glm::vec3(0.0f, 0.0f, 0.05f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->moveModelLocal(glm::vec3(0.0f, 0.0f, -0.05f));
        }
        else
        {
            this->moveModel(glm::vec3(0.0f, 0.0f, -0.05f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->scaleModelLocal(glm::vec3(1.01f, 1.01f, 1.01f));
        }
        else
        {
            this->scaleModel(glm::vec3(1.01f, 1.01f, 1.01f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->scaleModelLocal(glm::vec3(0.99f, 0.99f, 0.99f));
        }
        else
        {
            this->scaleModel(glm::vec3(0.99f, 0.99f, 0.99f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        this->resetModel();
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->rotateModelLocal(glm::vec3(0.0f, 1.f, 0.0f));
        }
        else
        {
            this->rotateModel(glm::vec3(0.0f, 1.f, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->rotateModelLocal(glm::vec3(0.0f, -1.f, 0.0f));
        }
        else
        {
            this->rotateModel(glm::vec3(0.0f, -1.f, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->rotateModelLocal(glm::vec3(1.f, 0.0f, 0.0f));
        }
        else
        {
            this->rotateModel(glm::vec3(1.f, 0.0f, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->rotateModelLocal(glm::vec3(-1.f, 0.0f, 0.0f));
        }
        else
        {
            this->rotateModel(glm::vec3(-1.f, 0.0f, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->rotateModelLocal(glm::vec3(0.f, 0.0f, -1.0f));
        }
        else
        {
            this->rotateModel(glm::vec3(0.f, 0.0f, -1.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->rotateModelLocal(glm::vec3(0.f, 0.0f, 1.0f));
        }
        else
        {
            this->rotateModel(glm::vec3(0.f, 0.0f, 1.0f));
        }
    }
}

void Model::moveModelLocal(glm::vec3 position)
{
    modelPosLocal += position;
    modelMatrix = glm::translate(modelMatrix, position);
    if (localTransformations)
    {
        applyMatrixLocally();
    }
}

void Model::moveModel(glm::vec3 position)
{
    modelPos += position;
    glm::vec3 curPos = worldMatrix[3];
    glm::vec3 newPos = curPos + position;
    worldMatrix[3] = glm::vec4(newPos, 1.0f);
    if (localTransformations)
    {
        applyMatrixLocally();
    }
}

void Model::scaleModelLocal(glm::vec3 scalar)
{
    modelScaleLocal *= scalar;
    modelMatrix = glm::scale(modelMatrix, scalar);
    if (localTransformations)
    {
        applyMatrixLocally();
    }
}

void Model::scaleModel(glm::vec3 scalar)
{
    modelScale *= scalar;
    cout << modelScale.x << " " << modelScale.y << " " << modelScale.z << endl;
    worldMatrix = glm::scale(worldMatrix, scalar);
    if (localTransformations)
    {
        applyMatrixLocally();
    }
}

void Model::rotateModelLocal(glm::vec3 rotation)
{
    modelRotLocal += rotation;
    float angleX = rotation.x;
    float angleY = rotation.y;
    float angleZ = rotation.z;

    glm::mat4 rotMatrix = glm::mat4(1.0f);
    rotMatrix = glm::rotate(rotMatrix, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotMatrix = glm::rotate(rotMatrix, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotMatrix = glm::rotate(rotMatrix, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix *= rotMatrix;

    if (localTransformations)
    {
        applyMatrixLocally();
    }
}

void Model::rotateModel(glm::vec3 rotation)
{
    modelRot += rotation;
    float angleX = rotation.x;
    float angleY = rotation.y;
    float angleZ = rotation.z;

    glm::mat4 rotMatrix = glm::mat4(1.0f);
    rotMatrix = glm::rotate(rotMatrix, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotMatrix = glm::rotate(rotMatrix, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotMatrix = glm::rotate(rotMatrix, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
    worldMatrix *= rotMatrix;
    if (localTransformations)
    {
        applyMatrixLocally();
    }
}

void Model::resetModel()
{
    //if (!localTransformations)
    //{
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    //    glm::mat4 rotMatrix = glm::mat4(1.0f);
    //    rotMatrix = glm::rotate(rotMatrix, glm::radians(-modelRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    //    rotMatrix = glm::rotate(rotMatrix, glm::radians(-modelRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //    rotMatrix = glm::rotate(rotMatrix, glm::radians(-modelRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    //    modelMatrix *= rotMatrix;
    //    modelMatrix = glm::scale(modelMatrix, modelScale / modelScale);
    //}
    moveModel(-modelPos);
    //rotateModel(glm::vec3(-modelRot.x, 0.0f, 0.0f));
    //rotateModel(glm::vec3(0.0f, -modelRot.y, 0.0f));
    //rotateModel(glm::vec3(0.0f, 0.0f, -modelRot.z));
    //modelRot = glm::vec3(0.0f);
    //glm::mat4 rotMatrix = glm::mat4(1.0f);
    //rotMatrix = glm::rotate(rotMatrix, glm::radians(-modelRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    //rotMatrix = glm::rotate(rotMatrix, glm::radians(-modelRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //rotMatrix = glm::rotate(rotMatrix, glm::radians(-modelRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    //modelRot = glm::vec3(0.0f);
    //worldMatrix *= rotMatrix;
    scaleModel(1.0f / modelScale);
    moveModel(glm::vec3(0.0f, 0.0f, -5.0f));
}

glm::vec3 Model::getScale(glm::mat4 matrix)
{
    glm::vec3 scale;
    scale.x = glm::length(glm::vec3(matrix[0]));
    scale.y = glm::length(glm::vec3(matrix[1]));
    scale.z = glm::length(glm::vec3(matrix[2]));
    return scale;
}

void Model::updateShader(Shader* shader)
{
    shader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(this->modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "worldMatrix"), 1, GL_FALSE, glm::value_ptr(this->worldMatrix));
    curShader = shader;
}