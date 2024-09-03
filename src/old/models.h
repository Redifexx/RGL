#include "mesh.h"
#include "shader.h"

class Model
{
    public:
        vector<Mesh*> meshes;
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 worldMatrix = glm::mat4(1.0f);
        Shader* curShader;
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
        glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 modelScale = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 modelRot = glm::vec3(0.0f, 0.0f, 0.0f);
        float nearClip;
        float farClip;
        bool isFlat;
        bool isOrtho;

        void Draw(GLFWwindow* window);
        void addMesh(string path);

        void moveModelLocal(glm::vec3 position);
        void moveModel(glm::vec3 position);

        void scaleModelLocal(glm::vec3 scalar);
        void scaleModel(glm::vec3 scalar);

        void rotateModelLocal(glm::vec3 rotation);
        void rotateModel(glm::vec3 rotation);

        void resetModel();

        void enableCPUCalc();
        void applyMatrixLocally();
        void getInput(GLFWwindow* window);

        void updateShader(Shader* shader);

        Model(string path, Shader* shader); //Initial Mesh
        ~Model();

    private:
        glm::vec3 modelPosLocal = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 modelScaleLocal = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 modelRotLocal = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 getScale(glm::mat4 matrix);
        bool localTransformations = false;

};