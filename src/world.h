#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <fastnoiselite/FastNoiseLite.h>
#include <cstdlib>
#include "chunk.h"

class World
{
    void GenerateWorld(std::string seed);
    int mapFloatToInt(float value);
public:
    int worldMap[256][256];
    Chunk* worldChunks[16][16];
    World(std::string seed = "")
    {
        GenerateWorld(seed);
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