#ifndef BLOCK_H
#define BLOCK_H
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "primitives.h"
#include <vector>
#include <iostream>

enum BLOCK_TYPE
{
    AIR,
    STONE,
    GOLD,
    GRASS,
    GRAVEL,
    IRON,
    ROSE,
    SPONGE
};

class Block
{
public:
    glm::ivec3 pos;
    BLOCK_TYPE type;
    Block(glm::ivec3 pos_ = glm::ivec3(0), BLOCK_TYPE type_ = AIR);
    std::vector<glm::vec2> GetBlockTexCoords();
};

#endif
