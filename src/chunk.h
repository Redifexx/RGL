#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"


class Chunk
{
public:
    Block* chunkArr[16][16][16];
    glm::ivec3 chunkPos;
    std::vector<float>* vertices;
    std::vector<unsigned int>* indices;

    unsigned int cVAO, cVBO, cEBO;
    Chunk(glm::ivec3 chunkPos_ = glm::ivec3(0, 0, 0));
    void GenerateChunk(int** worldMap_, int width, int height);
    void GenerateMesh();
    void SetupChunkBuffers();
    void RenderChunk();
};

#endif