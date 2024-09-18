#include "block.h"

Block::Block(glm::ivec3 pos_, BLOCK_TYPE type_)
{
    this->pos = pos_;
    this->type = type_;
}

glm::vec2 Block::GetBlockTexCoords()
{
    return glm::vec2(0.0f, 0.0f);
}