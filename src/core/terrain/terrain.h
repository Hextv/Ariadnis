#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Terrain {
public:
    int width;
    int depth;
    float cellSize;

    std::vector<float> vertices;
    std::vector<int> indices;

    unsigned int VAO, VBO, EBO;

    // Constructors & Destructor
    Terrain();
    Terrain(int width, int depth, float cellSize);
    ~Terrain();

    // Member Functions
    void generateMesh();
    void setupBuffers();
    void updateBuffers();
    void draw();

    std::vector<float>& getVertices() { return vertices; }
};