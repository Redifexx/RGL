#ifndef BLOCK_H
#define BLOCK_H
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum BLOCK_TYPE
{
    AIR,
    STONE
};

class Block
{
    glm::ivec3 pos;
    BLOCK_TYPE type;
public:
    Block(glm::ivec3 pos_ = glm::ivec3(0.0f), BLOCK_TYPE type_ = AIR);
};

#endif
