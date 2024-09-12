#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
    public:
        vector<Texture> textures_loaded;
        Model(char* path)
        {
            loadModel(path);
        }
        void Draw(Shader &shader);
    private:
        vector<Mesh> meshes;
        string directory;

        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};


#endif