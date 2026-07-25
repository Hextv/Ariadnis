#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Terrain {
public:
	int width;
	int depth;
	float cellSize; // Size of a single square

	std::vector<float> vertices;
	std::vector<int> indices;

	unsigned int VAO, VBO, EBO; // OpenGL buffers

	// Controls the Mesh generation
	void generateMesh() {
		// Walk through each grid point on the X and Z axes to build the terrain mesh
		for (int z = 0; z < depth; z++) {
			for (int x = 0; x < width; x++) {
				// Code
			}
		}
	}

	// Future send information toward the GPU
	void setupBuffers() {

	}
};