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
    std::cout << "Generating World Chunks Async! ---" << std::endl;
    //Uses threads to generate chunks
    for (int i = 0; i < 16; i++)
    {
        for (int k = 0; k < 16; k++) //i, k, - returns chunk
        {
            storedChunks[i][k] = std::async(std::launch::async, &World::GenerateSingleChunk, this, i, k);
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

//Helper for threading
Chunk* World::GenerateSingleChunk(int i_, int k_)
{
    Chunk* curChunk = new Chunk(glm::ivec3((i_ - 8) * 16, 0, (k_ - 8) * 16));
    int** worldMapPtr = new int*[256];
    for (int c = 0; c < 256; ++c)
    {
        worldMapPtr[c] = worldMap[c];
    }
    curChunk->GenerateChunk(worldMapPtr, 256, 256);
    delete[] worldMapPtr;
    //std::cout << "Chunk: " << i_ << " " << k_ << " - Generated!" << std::endl;
    return curChunk;
}

void World::BackgroundChunkLoader()
{
    std::cout << "Setting Stored Chunks! ---" << std::endl;
    bool allValid = false;
    while(!allValid)
    {
        allValid = true;
        //very inefficient, could be fixed later
        for (int i = 0; i < 16; i++)
        {
            for (int k = 0; k < 16; k++) //i, k, - returns chunk
            {
                if (storedChunks[i][k].valid())
                {
                    worldChunks[i][k] = storedChunks[i][k].get();
                    worldChunks[i][k]->hasGenerated = true;
                }
                else
                {
                    allValid = false;
                }
            }
        }
    }
}

void World::SetupChunkLoader()
{
    std::thread chunks(BackgroundChunkLoader, this);
    chunks.detach();
}