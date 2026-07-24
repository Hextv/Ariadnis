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
	std::vector<float> indices;
};