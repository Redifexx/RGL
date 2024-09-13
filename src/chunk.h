#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"


class Chunk
{
public:
    Block chunkArr[16][16][16];
    glm::ivec3 chunkPos;

    Chunk(glm::ivec3 chunkPos_);
    void GenerateChunk();
};

#endif