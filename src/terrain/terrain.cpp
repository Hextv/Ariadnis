#include "terrain.h"

// Default Constructor
Terrain::Terrain()
    : width(10), depth(10), cellSize(1.0f), VAO(0), VBO(0), EBO(0) {
}

// Parameterized Constructor
Terrain::Terrain(int w, int d, float size)
    : width(w), depth(d), cellSize(size), VAO(0), VBO(0), EBO(0) {
}

// Destructor - cleans up GPU resources
Terrain::~Terrain() {
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (EBO != 0) glDeleteBuffers(1, &EBO);
}

void Terrain::generateMesh() {
    vertices.clear();
    indices.clear();

    vertices.reserve(width * depth * 3);
    indices.reserve((width - 1) * (depth - 1) * 6);

    // 1. Generate Vertices
    for (int z = 0; z < depth; z++) {
        for (int x = 0; x < width; x++) {
            float posX = x * cellSize;
            float posY = 0.0f;
            float posZ = z * cellSize;

            vertices.push_back(posX);
            vertices.push_back(posY);
            vertices.push_back(posZ);
        }
    }

    // 2. Generate Indices (Triangles)
    for (int z = 0; z < depth - 1; z++) {
        for (int x = 0; x < width - 1; x++) {
            int topLeft = z * width + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * width + x;
            int bottomRight = bottomLeft + 1;

            // First Triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second Triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

void Terrain::setupBuffers() {
    // Delete existing buffers if re-initializing
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    // Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Terrain::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}