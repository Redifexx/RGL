#include "mesh.h"

Mesh::Mesh(string path)
{
    loadMesh(path);
    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex texture
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex normals
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    cout << "Mesh has been setup!" << endl;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("../textures/checkerfloor.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}   

void Mesh::updateVertices()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
}

void Mesh::loadMesh(string path)
{
    std::vector<Vertex*> allVerts;
    std::vector<unsigned int> allInds;
    std::vector<unsigned int> allIndsNorm;
    std::vector<unsigned int> allIndsTex;
    bool hasNormal = false;
    std::ifstream curFile(path);
    if (!curFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
    }
    cout << "FILE IS OPEN!" << endl;
    std::string line;
    int vnPos = 0;
    int vtPos = 0;
    int posCount = 0;
    int normCount = 0;
    int texCount = 0;
    while (std::getline(curFile, line)) {

        string firstChar = line.substr(0, 2);
        string firstCharNorm = line.substr(0, 3);
        std::istringstream iss(line);
        std::string cur;
        //cout << firstChar << endl;
        if (firstChar == "v ")
        {  
            //std::cout << line << std::endl;
            int pos = 0;
            Vertex* curVertex = new Vertex();
            float vertXYZ[3];
            glm::vec3 curPos;
            while (iss >> cur)
            {
                if (cur != "v")
                {
                    vertXYZ[pos] = stof(cur);
                    pos++;
                }
            }
            curPos.x = vertXYZ[0];
            curPos.y = vertXYZ[1];
            curPos.z = vertXYZ[2];
            curVertex->Position = curPos;
            //cout << "Added: " << curPos.x << " " << curPos.y << " " << curPos.z << endl;
            allVerts.push_back(curVertex);
            posCount++;
        }
        else if (firstCharNorm == "vn ")
        {
            hasNormal = true;
            int pos = 0;
            float vertNormXYZ[3];
            glm::vec3 curNorm;
            while (iss >> cur)
            {
                if (cur != "vn")
                {
                    vertNormXYZ[pos] = stof(cur);
                    pos++;
                }
            }
            curNorm.x = vertNormXYZ[0]; 
            curNorm.y = vertNormXYZ[1];
            curNorm.z = vertNormXYZ[2];
            if (vnPos < allVerts.size())
            {
                allVerts[vnPos]->Normal = curNorm;
                //cout << "Normal: " << vnPos << " " << curNorm.x << " " << curNorm.y << " "<< curNorm.x << endl;
            }
            else
            {
                Vertex* curVertex = new Vertex();
                curVertex->Normal = curNorm;
                allVerts.push_back(curVertex);
            }
            //cout << "Normal: " << vnPos << " " << curNorm.x << " " << curNorm.y << " "<< curNorm.x << endl;
            vnPos++;
            normCount++;
        }
        else if (firstCharNorm == "vt ")
        {
            //cout << "VT!" << endl;
            int pos = 0;
            float vertTexXYZ[2];
            glm::vec2 curTex;
            while (iss >> cur)
            {
                if (cur != "vt")
                {
                    vertTexXYZ[pos] = stof(cur);
                    pos++;
                }
            }
            //cout << "VT ISS DONE" << endl;
            curTex.x = vertTexXYZ[0];
            curTex.y = vertTexXYZ[1];
            //cout << "ALL VERTS SIZE: " << allVerts.size() << " Pos: " << vtPos << endl;
            //allVerts[vtPos]->TexCoords = curTex;
            if (vtPos < allVerts.size())
            {
                allVerts[vtPos]->TexCoords = curTex;
                //cout << "Normal: " << vnPos << " " << curNorm.x << " " << curNorm.y << " "<< curNorm.x << endl;
            }
            else
            {
                Vertex* curVertex = new Vertex();
                curVertex->TexCoords = curTex;
                allVerts.push_back(curVertex);
            }
            vtPos++;
            texCount++;
        }
        else if (firstChar == "f ")
        {
            //cout << "F!" << endl;
            vector<unsigned int> tempInd;
            vector<unsigned int> tempIndNorm;
            vector<unsigned int> tempIndTex;
            while (iss >> cur)
            {
                if (cur != "f")
                {
                    size_t slashPosOne = cur.find("/");
                    size_t slashPosTwo = cur.find("/", slashPosOne + 1);
                    //cout << "CUR: " << cur << " " << slashPosOne << " " << slashPosTwo << endl;
                    if (slashPosOne != string::npos && slashPosTwo != string::npos)
                    {
                        string vertexIndexStr = cur.substr(0, slashPosOne);
                        int vertIndex = stoi(vertexIndexStr);
                        //allInds.push_back(vertIndex - 1);
                        tempInd.push_back(vertIndex - 1);

                        if (slashPosTwo - (slashPosOne + 1) != 0)
                        {
                            string texIndexStr = cur.substr(slashPosOne + 1, slashPosTwo - (slashPosOne + 1));
                            //cout << "AHH: " << texIndexStr << endl;
                            int texIndex = stoi(texIndexStr);
                            //allIndsTex.push_back(texIndex - 1);
                            tempIndTex.push_back(texIndex - 1);
                        }

                        string normIndexStr = cur.substr(slashPosTwo + 1);
                        //cout << "NORM INDEX: " << normIndexStr << endl;
                        int normIndex = stoi(normIndexStr);
                        //allIndsNorm.push_back(normIndex - 1);
                        tempIndNorm.push_back(normIndex - 1);
                    }
                    else
                    {
                        int vertIndex = stoi(cur);
                        tempInd.push_back(vertIndex - 1);
                        tempIndTex.push_back(0);
                        tempIndNorm.push_back(0);
                    }
                }
            }
            //cout << "After ISS!" << endl;
            if (tempInd.size() > 3)
            {
                //Triangulization
                int triCount = tempInd.size() - 2;
                for (int i = 0; i < triCount; i++)
                {
                    allInds.push_back(tempInd[0]);
                    allIndsNorm.push_back(tempIndNorm[0]);
                    if (allIndsTex.size() > 0)
                    {
                        allIndsTex.push_back(tempIndTex[0]);
                    }
                    //cout << tempInd[0] << " ";

                    allInds.push_back(tempInd[i+1]);
                    allIndsNorm.push_back(tempIndNorm[i+1]);
                    if (allIndsTex.size() > 0)
                    {
                        allIndsTex.push_back(tempIndTex[i+1]);
                    }
                    //cout << tempInd[i+1] << " ";

                    allInds.push_back(tempInd[i+2]);
                    allIndsNorm.push_back(tempIndNorm[i+2]);
                    if (allIndsTex.size() > 0)
                    {
                        allIndsTex.push_back(tempIndTex[i+2]);
                    }
                    //cout << tempInd[i+2] << endl;
                }
            }
            else
            {
                for (int i = 0; i < tempInd.size(); i++)
                {
                    allInds.push_back(tempInd[i]);
                    allIndsNorm.push_back(tempIndNorm[i]);
                    if (allIndsTex.size() > 0)
                    {
                        allIndsTex.push_back(tempIndTex[i]);
                    }
                }
            }
        }
    }
    curFile.close();
    cout << "FILE CLOSED!" << endl;

    for (Vertex* curVert : allVerts)
    {
        this->vertices.push_back(*curVert);
    }
    this->indices = allInds;
    this->indicesNorm = allIndsNorm;
    this->indicesTex = allIndsTex;

    //if (!hasNormal)
    hasNormal = false;
    if (!hasNormal)
    {
        cout << "Calculating Normals!" << endl;
        //Automatically Calculates Normal Based on Position Vertex
        for (int i = 0; i < allInds.size(); i += 3)
        {
            glm::vec3 v0 = vertices[allInds[i]].Position;
            glm::vec3 v1 = vertices[allInds[i+1]].Position;
            glm::vec3 v2 = vertices[allInds[i+2]].Position;

            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

            vertices[allInds[i]].Normal += faceNormal;
            vertices[allInds[i+1]].Normal += faceNormal;
            vertices[allInds[i+2]].Normal += faceNormal;
            polyCount++;
        }
    }
    else
    {
        cout << "Using Given Normals!" << endl;
        for (int i = 0; i < vertices.size(); i++)
        {
            vertices[i].Normal = allVerts[i]->Normal;
            //cout << "Normal Added: " << vertices[i].Normal.x << " " << vertices[i].Normal.y << " " << vertices[i].Normal.z << endl;
        }
    }
    cout << "Pos Count: " << posCount << endl;
    cout << "Normal Count: " << normCount << endl;
    cout << "TextureCoord Count: " << texCount << endl;
    cout << "Total Tri Count: " << polyCount << endl;
}

void Mesh::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}