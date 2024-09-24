#include "block.h"

Block::Block(glm::ivec3 pos_, BLOCK_TYPE type_)
{
    this->pos = pos_;
    this->type = type_;
}

// Big ugly switch case
std::vector<glm::vec2> Block::GetBlockTexCoords()
{
    std::vector<glm::vec2> texTop;
    std::vector<glm::vec2> texBottom;
    std::vector<glm::vec2> texLeft;
    std::vector<glm::vec2> texRight;
    std::vector<glm::vec2> texFront;
    std::vector<glm::vec2> texBack;
    
    std::vector<glm::vec2> texCoords;

    switch(this->type)
    {
        case STONE:
            //Top
            texTop.push_back(glm::vec2(0.0f, 0.9375f));
            texTop.push_back(glm::vec2(0.0625f, 0.9375f));
            texTop.push_back(glm::vec2(0.0625f, 1.0f));
            texTop.push_back(glm::vec2(0.0f, 1.0f));

            // Bottom
            texBottom.push_back(glm::vec2(0.0f, 0.9375f));
            texBottom.push_back(glm::vec2(0.0625f, 0.9375f));
            texBottom.push_back(glm::vec2(0.0625f, 1.0f));
            texBottom.push_back(glm::vec2(0.0f, 1.0f));

            // Left
            texLeft.push_back(glm::vec2(0.0f, 0.9375f));
            texLeft.push_back(glm::vec2(0.0625f, 0.9375f));
            texLeft.push_back(glm::vec2(0.0625f, 1.0f));
            texLeft.push_back(glm::vec2(0.0f, 1.0f));

            // Right
            texRight.push_back(glm::vec2(0.0f, 0.9375f));
            texRight.push_back(glm::vec2(0.0625f, 0.9375f));
            texRight.push_back(glm::vec2(0.0625f, 1.0f));
            texRight.push_back(glm::vec2(0.0f, 1.0f));

            // Front
            texFront.push_back(glm::vec2(0.0f, 0.9375f));
            texFront.push_back(glm::vec2(0.0625f, 0.9375f));
            texFront.push_back(glm::vec2(0.0625f, 1.0f));
            texFront.push_back(glm::vec2(0.0f, 1.0f));

            // Back
            texRight.push_back(glm::vec2(0.0f, 0.9375f));
            texRight.push_back(glm::vec2(0.0625f, 0.9375f));
            texRight.push_back(glm::vec2(0.0625f, 1.0f));
            texRight.push_back(glm::vec2(0.0f, 1.0f));
            break;

        case GOLD:
            //Top
            texTop.push_back(glm::vec2(0.0625f, 0.9375f));
            texTop.push_back(glm::vec2(0.125f, 0.9375f));
            texTop.push_back(glm::vec2(0.125f, 1.0f));
            texTop.push_back(glm::vec2(0.0625f, 1.0f));

            // Bottom
            texBottom.push_back(glm::vec2(0.0625f, 0.9375f));
            texBottom.push_back(glm::vec2(0.125f, 0.9375f));
            texBottom.push_back(glm::vec2(0.125f, 1.0f));
            texBottom.push_back(glm::vec2(0.0625f, 1.0f));

            // Left
            texLeft.push_back(glm::vec2(0.125f, 0.9375f)); // 2 AND 3
            texLeft.push_back(glm::vec2(0.1875f, 0.9375f));
            texLeft.push_back(glm::vec2(0.1875f, 1.0f));
            texLeft.push_back(glm::vec2(0.125f, 1.0f));

            // Right
            texRight.push_back(glm::vec2(0.125f, 0.9375f)); 
            texRight.push_back(glm::vec2(0.1875f, 0.9375f));
            texRight.push_back(glm::vec2(0.1875f, 1.0f));
            texRight.push_back(glm::vec2(0.125f, 1.0f));

            // Front
            texFront.push_back(glm::vec2(0.125f, 0.9375f)); 
            texFront.push_back(glm::vec2(0.1875f, 0.9375f));
            texFront.push_back(glm::vec2(0.1875f, 1.0f));
            texFront.push_back(glm::vec2(0.125f, 1.0f));

            // Back
            texRight.push_back(glm::vec2(0.125f, 0.9375f)); 
            texRight.push_back(glm::vec2(0.1875f, 0.9375f));
            texRight.push_back(glm::vec2(0.1875f, 1.0f));
            texRight.push_back(glm::vec2(0.125f, 1.0f));
            break;
            
        case GRASS:
            //Top
            texTop.push_back(glm::vec2(0.25f, 0.9375f));
            texTop.push_back(glm::vec2(0.3125f, 0.9375f));
            texTop.push_back(glm::vec2(0.3125f, 1.0f));
            texTop.push_back(glm::vec2(0.25f, 1.0f));

            // Bottom
            texBottom.push_back(glm::vec2(0.625f, 0.9375f));
            texBottom.push_back(glm::vec2(0.6875f, 0.9375f));
            texBottom.push_back(glm::vec2(0.6875f, 1.0f));
            texBottom.push_back(glm::vec2(0.625f, 1.0f));

            // Left
            texLeft.push_back(glm::vec2(0.1875f, 0.9375f));     // 2 AND 3
            texLeft.push_back(glm::vec2(0.25f, 0.9375f));
            texLeft.push_back(glm::vec2(0.25f, 1.0f));
            texLeft.push_back(glm::vec2(0.1875f, 1.0f));

            // Right
            texRight.push_back(glm::vec2(0.1875f, 0.9375f));
            texRight.push_back(glm::vec2(0.25f, 0.9375f));
            texRight.push_back(glm::vec2(0.25f, 1.0f));
            texRight.push_back(glm::vec2(0.1875f, 1.0f));

            // Front
            texFront.push_back(glm::vec2(0.1875f, 0.9375f));
            texFront.push_back(glm::vec2(0.25f, 0.9375f));
            texFront.push_back(glm::vec2(0.25f, 1.0f));
            texFront.push_back(glm::vec2(0.1875f, 1.0f));

            // Back
            texBack.push_back(glm::vec2(0.1875f, 0.9375f));
            texBack.push_back(glm::vec2(0.25f, 0.9375f));
            texBack.push_back(glm::vec2(0.25f, 1.0f));
            texBack.push_back(glm::vec2(0.1875f, 1.0f));
            break;

        case GRAVEL:
            //Top
            texTop.push_back(glm::vec2(0.3125f, 0.9375f));
            texTop.push_back(glm::vec2(0.375f, 0.9375f));
            texTop.push_back(glm::vec2(0.375f, 1.0f));
            texTop.push_back(glm::vec2(0.3125f, 1.0f));

            // Bottom
            texBottom.push_back(glm::vec2(0.3125f, 0.9375f));
            texBottom.push_back(glm::vec2(0.375f, 0.9375f));
            texBottom.push_back(glm::vec2(0.375f, 1.0f));
            texBottom.push_back(glm::vec2(0.3125f, 1.0f));

            // Left
            texLeft.push_back(glm::vec2(0.3125f, 0.9375f));       // 2 AND 3
            texLeft.push_back(glm::vec2(0.375f, 0.9375f));
            texLeft.push_back(glm::vec2(0.375f, 1.0f));
            texLeft.push_back(glm::vec2(0.3125f, 1.0f));

            // Right
            texRight.push_back(glm::vec2(0.3125f, 0.9375f));
            texRight.push_back(glm::vec2(0.375f, 0.9375f));
            texRight.push_back(glm::vec2(0.375f, 1.0f));
            texRight.push_back(glm::vec2(0.3125f, 1.0f));

            // Front
            texFront.push_back(glm::vec2(0.3125f, 0.9375f));
            texFront.push_back(glm::vec2(0.375f, 0.9375f));
            texFront.push_back(glm::vec2(0.375f, 1.0f));
            texFront.push_back(glm::vec2(0.3125f, 1.0f));

            // Back
            texBack.push_back(glm::vec2(0.3125f, 0.9375f));
            texBack.push_back(glm::vec2(0.375f, 0.9375f));
            texBack.push_back(glm::vec2(0.375f, 1.0f));
            texBack.push_back(glm::vec2(0.3125f, 1.0f));
            break;

        case IRON: //TBD
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
            texLeft.push_back(glm::vec2(0.2f, 0.0f)); // 2 AND 3
            texLeft.push_back(glm::vec2(0.3f, 0.0f));
            texLeft.push_back(glm::vec2(0.3f, 1.0f));
            texLeft.push_back(glm::vec2(0.2f, 1.0f));

            // Right
            texRight.push_back(glm::vec2(0.2f, 0.0f));
            texRight.push_back(glm::vec2(0.3f, 0.0f));
            texRight.push_back(glm::vec2(0.3f, 1.0f));
            texRight.push_back(glm::vec2(0.2f, 1.0f));

            // Front
            texFront.push_back(glm::vec2(0.2f, 0.0f));
            texFront.push_back(glm::vec2(0.3f, 0.0f));
            texFront.push_back(glm::vec2(0.3f, 1.0f));
            texFront.push_back(glm::vec2(0.2f, 1.0f));

            // Back
            texBack.push_back(glm::vec2(0.2f, 0.0f));
            texBack.push_back(glm::vec2(0.3f, 0.0f));
            texBack.push_back(glm::vec2(0.3f, 1.0f));
            texBack.push_back(glm::vec2(0.2f, 1.0f));
            break;

        case SPONGE: //TBD
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
            texLeft.push_back(glm::vec2(0.2f, 0.0f)); // 2 AND 3
            texLeft.push_back(glm::vec2(0.3f, 0.0f));
            texLeft.push_back(glm::vec2(0.3f, 1.0f));
            texLeft.push_back(glm::vec2(0.2f, 1.0f));

            // Right
            texRight.push_back(glm::vec2(0.2f, 0.0f));
            texRight.push_back(glm::vec2(0.3f, 0.0f));
            texRight.push_back(glm::vec2(0.3f, 1.0f));
            texRight.push_back(glm::vec2(0.2f, 1.0f));

            // Front
            texFront.push_back(glm::vec2(0.2f, 0.0f));
            texFront.push_back(glm::vec2(0.3f, 0.0f));
            texFront.push_back(glm::vec2(0.3f, 1.0f));
            texFront.push_back(glm::vec2(0.2f, 1.0f));

            // Back
            texBack.push_back(glm::vec2(0.2f, 0.0f));
            texBack.push_back(glm::vec2(0.3f, 0.0f));
            texBack.push_back(glm::vec2(0.3f, 1.0f));
            texBack.push_back(glm::vec2(0.2f, 1.0f));
            break;
            
        default:
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
            texLeft.push_back(glm::vec2(0.2f, 0.0f)); // 2 AND 3
            texLeft.push_back(glm::vec2(0.3f, 0.0f));
            texLeft.push_back(glm::vec2(0.3f, 1.0f));
            texLeft.push_back(glm::vec2(0.2f, 1.0f));

            // Right
            texRight.push_back(glm::vec2(0.2f, 0.0f));
            texRight.push_back(glm::vec2(0.3f, 0.0f));
            texRight.push_back(glm::vec2(0.3f, 1.0f));
            texRight.push_back(glm::vec2(0.2f, 1.0f));

            // Front
            texFront.push_back(glm::vec2(0.2f, 0.0f));
            texFront.push_back(glm::vec2(0.3f, 0.0f));
            texFront.push_back(glm::vec2(0.3f, 1.0f));
            texFront.push_back(glm::vec2(0.2f, 1.0f));

            // Back
            texBack.push_back(glm::vec2(0.2f, 0.0f));
            texBack.push_back(glm::vec2(0.3f, 0.0f));
            texBack.push_back(glm::vec2(0.3f, 1.0f));
            texBack.push_back(glm::vec2(0.2f, 1.0f));
            break;
    }

    //Top Verts
    for (int i = 0; i < texRight.size(); i++)
    {
        texCoords.push_back(texRight[i]);
    }

    //Bottom
    for (int i = 0; i < texBottom.size(); i++)
    {
        texCoords.push_back(texBottom[i]);
    }

    //Left
    for (int i = 0; i < texLeft.size(); i++)
    {
        texCoords.push_back(texLeft[i]);
    }

    //Right
    for (int i = 0; i < texRight.size(); i++)
    {
        texCoords.push_back(texRight[i]);
    }

    //Front
    for (int i = 0; i < texFront.size(); i++)
    {
        texCoords.push_back(texFront[i]);
    }

    //Back
    for (int i = 0; i < texBack.size(); i++)
    {
        texCoords.push_back(texBack[i]);
    }

    return texCoords;
}