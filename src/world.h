#ifndef WORLD_H
#define WORLD_H

#include <thread>
#include <string>
#include <future>
#include <fastnoiselite/FastNoiseLite.h>
#include <cstdlib>
#include "chunk.h"

class World
{
    //svoid GenerateWorld();
    int mapFloatToInt(float value);
public:
    void GenerateWorld();
    void BackgroundChunkLoader();
    void SetupChunkLoader();
    int worldMap[256][256];
    int renderDistance = 16;
    Chunk* worldChunks[16][16];
    std::mutex chunkMutex; // Mutex for synchronizing access to worldChunks
    std::vector<Chunk*> renderableChunks;
    std::future<Chunk*> storedChunks[16][16];
    Chunk* GenerateSingleChunk(Chunk* chunk);
    std::string seed;
    World(std::string seed = "")
    {
        this->seed = seed;
        GenerateWorld();
    }
    //~World()
    //{
    //    for (int i = 0; i < 16; i++)
    //    {
    //        for (int k = 0; i < 16; k++)
    //        {
    //            delete worldChunks[i][k];
    //        }
    //    }
    //}
};


#endif