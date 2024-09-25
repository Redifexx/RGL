#include "world.h"

void World::GenerateWorld()
{
    std::cout << "Generating World!..." << std::endl;
    FastNoiseLite noise;

    //Sets Seed
    if (seed != "")
    {
        noise.SetSeed(std::stoi(this->seed));
    }
    else
    {
        srand(time(nullptr));
        noise.SetSeed(rand());
    }

    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(4);
    noise.SetFractalGain(0.5f);
    noise.SetFractalWeightedStrength(-0.5f);
    noise.SetFrequency(0.015f);


    //Height Map Generation
    for (int i = 0; i < 256; i++)
    {
        for (int k = 0; k < 256; k++)
        {
            worldMap[i][k] = mapFloatToInt(noise.GetNoise((float)i, (float)k));
            //worldMap[i][k] = mapFloatToInt(0.8f);
        }
    }

    std::cout << "Height Map Done!" << std::endl;
    std::cout << "Generating World Chunks! ---" << std::endl;
    for (int i = 0; i < 16; i++)
    {
        for (int k = 0; k < 16; k++)
        {
            Chunk* curChunk = new Chunk(glm::ivec3((i - 8) * 16, 0, (k - 8) * 16));

            int** worldMapPtr = new int*[256];
            for (int c = 0; c < 256; ++c)
            {
                worldMapPtr[c] = worldMap[c];
            }
            curChunk->GenerateChunk(worldMapPtr, 256, 256);
            //std::cout << "i: " << i << " z: " << k << " Chunk X: " << curChunk->chunkPos.x << " Z: " << curChunk->chunkPos.z << std::endl;
            worldChunks[i][k] = curChunk;
            delete[] worldMapPtr;
        }
    }
    std::cout << "World Done!" << std::endl;
}

int World::mapFloatToInt(float value)
{
    float normalizedValue = (value + 1.0f) / 2.0f;

    int intValue = static_cast<int>(std::round(normalizedValue * 15.0f)) + 1;

    if (intValue < 1) intValue = 1;
    if (intValue > 16) intValue = 16;

    return intValue - 1;
}