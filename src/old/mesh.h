//#define GLEW_STATIC
//#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <chrono>

using namespace std;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f);
    // texCoords
    glm::vec2 TexCoords;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<Vertex>       originalVertices;
        std::vector<unsigned int> indices;
        std::vector<unsigned int> indicesNorm;
        std::vector<unsigned int> indicesTex;

        int polyCount = 0;
        void setupMesh();
        void updateVertices();
        Mesh(string path);
        void Draw();
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        void loadMesh(string path);
};
