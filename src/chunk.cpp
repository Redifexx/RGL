#include "chunk.h"

Chunk::Chunk(glm::ivec3 chunkPos_)
{
    this->chunkPos = chunkPos_;
}

void Chunk::GenerateChunk(int** worldMap, int width, int height)
{
    //std::cout << "Generating Chunk!" << std::endl;
    for (int i = 0; i < 16; i++)
    {
        for (int k = 0; k < 16; k++)
        {
            for (int j = 0; j < 16; j++)
            {
                Block* curBlock = new Block();
                curBlock->pos = glm::ivec3(i, j, k);
                BLOCK_TYPE bt;
                bt = AIR;

                if (j < worldMap[(this->chunkPos.x + i) + 128][(this->chunkPos.z + k) + 128])
                {
                    //if (j < 12)
                    //{
                    //    bt = STONE;
                    //}
                    //else
                    //{
                    //    bt = GRASS;
                    //}
                    bt = STONE;
                }
                curBlock->type = bt;
                chunkArr[i][j][k] = curBlock;
            }
        }
    }
    //std::cout << "Done Generating Chunk!" << std::endl;
    GenerateMesh();
    //std::cout << "Done Generating Chunk Mesh!" << std::endl;
    //std::cout << "Setting Up Chunk Buffers!" << std::endl;
    SetupChunkBuffers();
};

void Chunk::GenerateMesh()
{
    vertices = new std::vector<float>();
    indices = new std::vector<unsigned int>();
    //std::cout << "Generating Chunk Mesh!" << std::endl;
    unsigned int indexOffset = 0;
    for (int i = 0; i < 16; i++)
    {
        for (int k = 0; k < 16; k++)
        {
            for (int j = 0; j < 16; j++)
            {
                if (chunkArr[i][j][k]->type != AIR)
                {
                    //Check All 6 Sides
                    if (j == 15 || chunkArr[i][j+1][k]->type == AIR) //TOP
                    {
                        float cubeTop[] = { //Pos, Tex, Normal
                            this->chunkPos.x + i + -0.5f,      this->chunkPos.y + j + 0.5f, this->chunkPos.z + k  + -0.5f,  0.0f, 1.0f,     0.0f,  1.0f,  0.0f, // TOP FACE
                            this->chunkPos.x + i + 0.5f,       this->chunkPos.y + j + 0.5f, this->chunkPos.z + k  + -0.5f,  1.0f, 1.0f,     0.0f,  1.0f,  0.0f,
                            this->chunkPos.x + i + 0.5f,       this->chunkPos.y + j + 0.5f, this->chunkPos.z + k  +  0.5f,  1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
                            this->chunkPos.x + i + -0.5f,      this->chunkPos.y + j + 0.5f,  this->chunkPos.z + k +  0.5f,   0.0f, 0.0f,     0.0f,  1.0f,  0.0f
                        };

                        vertices->insert(vertices->end(), std::begin(cubeTop), std::end(cubeTop));

                        indices->insert(indices->end(), {
                            indexOffset, indexOffset + 1, indexOffset + 2,
                            indexOffset + 2, indexOffset + 3, indexOffset
                        });

                        indexOffset += 4;
                    }

                    
                    //std::cout << "Checking Bottom" << std::endl;
                    if (j == 0 || chunkArr[i][j-1][k]->type == AIR) //BOTTOM
                    {
                        float cubeBottom[] = { //Pos, Tex, Normal
                           this->chunkPos.x + i + -0.5f,  this->chunkPos.y + j + -0.5f,  this->chunkPos.z + k + -0.5f,    0.0f, 0.0f,     0.0f, -1.0f,  0.0f, //bottom
                           this->chunkPos.x + i + 0.5f,   this->chunkPos.y + j + -0.5f,  this->chunkPos.z + k + -0.5f,    1.0f, 0.0f,     0.0f, -1.0f,  0.0f,
                           this->chunkPos.x + i + 0.5f,   this->chunkPos.y + j + -0.5f,  this->chunkPos.z + k +  0.5f,    1.0f, 1.0f,     0.0f, -1.0f,  0.0f,
                           this->chunkPos.x + i + -0.5f,  this->chunkPos.y + j + -0.5f,  this->chunkPos.z + k +  0.5f,    0.0f, 1.0f,     0.0f, -1.0f,  0.0f
                        };

                        vertices->insert(vertices->end(), std::begin(cubeBottom), std::end(cubeBottom));

                        indices->insert(indices->end(), {
                            indexOffset, indexOffset + 1, indexOffset + 2,
                            indexOffset + 2, indexOffset + 3, indexOffset
                        });

                        indexOffset += 4;
                    }

                    
                    //std::cout << "Checking Left" << std::endl;
                    if (i == 0 || chunkArr[i-1][j][k]->type == AIR) //LEFT
                    {
                        float cubeLeft[] = { //Pos, Tex, Normal
                           this->chunkPos.x + i + -0.5f, this->chunkPos.y + j +  0.5f, this->chunkPos.z + k +  0.5f,    1.0f, 1.0f,     -1.0f, 0.0f, 0.0f, //LEFT
                           this->chunkPos.x + i + -0.5f, this->chunkPos.y + j +  0.5f, this->chunkPos.z + k + -0.5f,    0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,
                           this->chunkPos.x + i + -0.5f, this->chunkPos.y + j + -0.5f, this->chunkPos.z + k + -0.5f,    0.0f, 0.0f,     -1.0f, 0.0f, 0.0f,
                           this->chunkPos.x + i + -0.5f, this->chunkPos.y + j + -0.5f, this->chunkPos.z + k +  0.5f,    1.0f, 0.0f,     -1.0f, 0.0f, 0.0f
                        };

                        vertices->insert(vertices->end(), std::begin(cubeLeft), std::end(cubeLeft));

                        indices->insert(indices->end(), {
                            indexOffset, indexOffset + 1, indexOffset + 2,
                            indexOffset + 2, indexOffset + 3, indexOffset
                        });

                        indexOffset += 4;
                    }
                    
                    //std::cout << "Checking Right" << std::endl;
                    if (i == 15 || chunkArr[i+1][j][k]->type == AIR) //Right
                    {
                        float cubeRight[] = { //Pos, Tex, Normal
                           this->chunkPos.x + i + 0.5f, this->chunkPos.y + j +  0.5f, this->chunkPos.z + k +  0.5f,     0.0f, 1.0f,     1.0f,  0.0f,  0.0f, //RIGHT
                           this->chunkPos.x + i + 0.5f, this->chunkPos.y + j +  0.5f, this->chunkPos.z + k + -0.5f,     1.0f, 1.0f,     1.0f,  0.0f,  0.0f,
                           this->chunkPos.x + i + 0.5f, this->chunkPos.y + j + -0.5f, this->chunkPos.z + k + -0.5f,     1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
                           this->chunkPos.x + i + 0.5f, this->chunkPos.y + j + -0.5f, this->chunkPos.z + k +  0.5f,     0.0f, 0.0f,     1.0f,  0.0f,  0.0f
                        };

                        vertices->insert(vertices->end(), std::begin(cubeRight), std::end(cubeRight));

                        indices->insert(indices->end(), {
                            indexOffset, indexOffset + 1, indexOffset + 2,
                            indexOffset + 2, indexOffset + 3, indexOffset
                        });

                        indexOffset += 4;
                    }

                    //std::cout << "Checking Front" << std::endl;
                    if (k == 15 || chunkArr[i][j][k+1]->type == AIR) //Front
                    {
                        float cubeFront[] = { //Pos, Tex, Normal
                           this->chunkPos.x + i + -0.5f, this->chunkPos.y + j + -0.5f, this->chunkPos.z + k + 0.5f,    0.0f, 0.0f,     0.0f, 0.0f, 1.0f, //FRONT
                           this->chunkPos.x + i + 0.5f,  this->chunkPos.y + j + -0.5f, this->chunkPos.z + k + 0.5f,    1.0f, 0.0f,     0.0f, 0.0f, 1.0f,
                           this->chunkPos.x + i + 0.5f,  this->chunkPos.y + j +  0.5f, this->chunkPos.z + k + 0.5f,    1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
                           this->chunkPos.x + i + -0.5f, this->chunkPos.y + j +  0.5f, this->chunkPos.z + k + 0.5f,    0.0f, 1.0f,     0.0f, 0.0f, 1.0f
                        };

                        vertices->insert(vertices->end(), std::begin(cubeFront), std::end(cubeFront));

                        indices->insert(indices->end(), {
                            indexOffset, indexOffset + 1, indexOffset + 2,
                            indexOffset + 2, indexOffset + 3, indexOffset
                        });

                        indexOffset += 4;
                    }

                    //std::cout << "Checking Back" << std::endl;
                    if (k == 0 || chunkArr[i][j][k-1]->type == AIR) //Back
                    {
                        float cubeBack[] = { //Pos, Tex, Normal
                           this->chunkPos.x + i + -0.5f, this->chunkPos.y + j +  -0.5f, this->chunkPos.z + k +  -0.5f,    1.0f, 0.0f,     0.0f, 0.0f, -1.0f, //BACK
                           this->chunkPos.x + i + 0.5f,  this->chunkPos.y + j + -0.5f,  this->chunkPos.z + k + -0.5f,    0.0f, 0.0f,     0.0f, 0.0f, -1.0f,
                           this->chunkPos.x + i + 0.5f,  this->chunkPos.y + j +  0.5f,  this->chunkPos.z + k + -0.5f,    0.0f, 1.0f,     0.0f, 0.0f, -1.0f,
                           this->chunkPos.x + i + -0.5f, this->chunkPos.y + j +   0.5f, this->chunkPos.z + k +  -0.5f,    1.0f, 1.0f,     0.0f, 0.0f, -1.0f
                        };

                        vertices->insert(vertices->end(), std::begin(cubeBack), std::end(cubeBack));

                        indices->insert(indices->end(), {
                            indexOffset, indexOffset + 1, indexOffset + 2,
                            indexOffset + 2, indexOffset + 3, indexOffset
                        });

                        indexOffset += 4;
                    }
                    
                }
            }
        }
    }

    //Upload Vertex and Index Data
    //glBindBuffer(GL_ARRAY_BUFFER, this->cVBO);
    //glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cEBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);
    
}

void Chunk::SetupChunkBuffers()
{
    glGenVertexArrays(1, &this->cVAO);
    glGenBuffers(1, &this->cVBO);
    glGenBuffers(1, &this->cEBO);

    glBindVertexArray(this->cVAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->cVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5* sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    //std::cout << "Done Setting Up Chunk Buffers!" << std::endl;
}

void Chunk::RenderChunk()
{
    //if (!hasRendered)
    //{
    //    for (int i = 0; i < vertices->size(); i += 8)
    //    {
    //        std::cout << "V: ";
    //        for (int j = 0; j < 3; j++)
    //        {
    //            std::cout << vertices->at(i + j) << " ";
    //        }
    //        std::cout << std::endl;
    //    }
    //    hasRendered = true;
    //}
    glBindVertexArray(this->cVAO);
    glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}