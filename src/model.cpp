#include "model.h"

void Model::Draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

// recursively travels through assimp's tree structure
void Model::processNode(aiNode *node, const aiScene *scene)
{
    //std::cout << "PROCESS" << std::endl;
    //process all node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        //std::cout << "MESH" << std::endl;
        //std::cout << node->mNumMeshes << std::endl;
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    //process each node's child mesh
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        //std::cout << "CHILD" << std::endl;
        //std::cout << node->mNumChildren << std::endl;
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    //std::cout << "PROCESSMESH" << std::endl;
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    //Vertex Data
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        //std::cout << "VERTEX" << std::endl;
        //std::cout << mesh->mNumVertices << std::endl;
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        //std::cout << "NORMAL" << std::endl;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;


        if (mesh->mTextureCoords[0])
        {
            //std::cout << "TEXCOORDSSTART" << std::endl;
            glm::vec2 tex;
            tex.x = mesh->mTextureCoords[0][i].x; 
            tex.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = tex;

            if (mesh->mTangents)
            {
                //std::cout << "Tangent" << std::endl;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
            }
            
            if (mesh->mBitangents)
            {
                //std::cout << "Bitangent" << std::endl;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }

            /*
            if (scene->mNumMaterials > mesh->mMaterialIndex)
            {
            const auto& mat = scene->mMaterials[mesh->mMaterialIndex];
            aiColor4D diffuse;
            if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
            {
                vertex.color = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
            }

            if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                vertex.useDiffuseTexture = 1.f;
            }
            else
            {
                vertex.useDiffuseTexture = 0.f;
            }
            */
            //std::cout << "TEXCOORDSEND" << std::endl;
        }
        else
        {
            //std::cout << "NOTEXCOORDS" << std::endl;
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
        //std::cout << "VERTEXEND" << std::endl;
    }

    //Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    //Material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
    
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Material loadMaterial(aiMaterial* mat)
{
    Material material;
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.Diffuse = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.Ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.Specular = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.Shininess = shininess;

    return material;
}

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
