#include "block.h"

Block::Block(glm::ivec3 pos_, BLOCK_TYPE type_)
{
    this->pos = pos_;
    this->type = type_;
}