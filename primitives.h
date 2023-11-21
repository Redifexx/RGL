#pragma once
float triangle[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

float triforce[] = {
    -0.25f, 0.0f, 0.0f,
     0.25f, 0.0f, 0.0f,
     0.0f,  0.5f, 0.0f,
     -0.5f, -0.5f, 0.0f,
     0.0f, -0.5f, 0.0f,
     -0.25f, 0.0f, 0.0f,
     0.0f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.25f, 0.0f, 0.0f
};

float rectangle[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int rectIndices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};