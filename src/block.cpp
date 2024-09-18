#include "block.h"

Block::Block(glm::ivec3 pos_, BLOCK_TYPE type_)
{
    this->pos = pos_;
    this->type = type_;
}

// Big ugly switch case
glm::vec2 Block::GetBlockTexCoords()
{
    std::vector<glm::vec2> texTop;
    std::vector<glm::vec2> texBottom;
    std::vector<glm::vec2> texLeft;
    std::vector<glm::vec2> texRight;
    std::vector<glm::vec2> texFront;
    std::vector<glm::vec2> texBack;

    switch(this->type)
    {
        case STONE:
            //Top
            texTop.push_back(glm::vec2(0.0f, 0.0f));
            texTop.push_back(glm::vec2(0.1f, 0.0f));
            texTop.push_back(glm::vec2(0.1f, 1.0f));
            texTop.push_back(glm::vec2(0.0f, 1.0f));

            // Bottom
            texBottom.push_back(glm::vec2(0.0f, 0.0f));
            texBottom.push_back(glm::vec2(0.1f, 0.0f));
            texBottom.push_back(glm::vec2(0.1f, 1.0f));
            texBottom.push_back(glm::vec2(0.0f, 1.0f));

            // Left
            texLeft.push_back(glm::vec2(0.0f, 0.0f));
            texLeft.push_back(glm::vec2(0.1f, 0.0f));
            texLeft.push_back(glm::vec2(0.1f, 1.0f));
            texLeft.push_back(glm::vec2(0.0f, 1.0f));

            // Right
            texRight.push_back(glm::vec2(0.0f, 0.0f));
            texRight.push_back(glm::vec2(0.1f, 0.0f));
            texRight.push_back(glm::vec2(0.1f, 1.0f));
            texRight.push_back(glm::vec2(0.0f, 1.0f));

            // Front
            texFront.push_back(glm::vec2(0.0f, 0.0f));
            texFront.push_back(glm::vec2(0.1f, 0.0f));
            texFront.push_back(glm::vec2(0.1f, 1.0f));
            texFront.push_back(glm::vec2(0.0f, 1.0f));

            // Back
            texRight.push_back(glm::vec2(0.0f, 0.0f));
            texRight.push_back(glm::vec2(0.1f, 0.0f));
            texRight.push_back(glm::vec2(0.1f, 1.0f));
            texRight.push_back(glm::vec2(0.0f, 1.0f));
            break;

        case GOLD:
            //Top
            texTop.push_back(glm::vec2(0.1f, 0.0f));
            texTop.push_back(glm::vec2(0.2f, 0.0f));
            texTop.push_back(glm::vec2(0.2f, 1.0f));
            texTop.push_back(glm::vec2(0.1f, 1.0f));

            // Bottom
            texBottom.push_back(glm::vec2(0.1f, 0.0f));
            texBottom.push_back(glm::vec2(0.2f, 0.0f));
            texBottom.push_back(glm::vec2(0.2f, 1.0f));
            texBottom.push_back(glm::vec2(0.1f, 1.0f));

            // Left
            texLeft.push_back(glm::vec2(0.0f, 0.0f)); // 2 AND 3
            texLeft.push_back(glm::vec2(0.1f, 0.0f));
            texLeft.push_back(glm::vec2(0.1f, 1.0f));
            texLeft.push_back(glm::vec2(0.0f, 1.0f));

            // Right
            texRight.push_back(glm::vec2(0.0f, 0.0f));
            texRight.push_back(glm::vec2(0.1f, 0.0f));
            texRight.push_back(glm::vec2(0.1f, 1.0f));
            texRight.push_back(glm::vec2(0.0f, 1.0f));

            // Front
            texFront.push_back(glm::vec2(0.0f, 0.0f));
            texFront.push_back(glm::vec2(0.1f, 0.0f));
            texFront.push_back(glm::vec2(0.1f, 1.0f));
            texFront.push_back(glm::vec2(0.0f, 1.0f));

            // Back
            texRight.push_back(glm::vec2(0.0f, 0.0f));
            texRight.push_back(glm::vec2(0.1f, 0.0f));
            texRight.push_back(glm::vec2(0.1f, 1.0f));
            texRight.push_back(glm::vec2(0.0f, 1.0f));
            break;
    }

    return glm::vec2(0.0f, 0.0f);
}