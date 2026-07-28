#pragma once

#include <glm/glm.hpp>
#include <algorithm>
#include "../../core/paint/paint_layer.h"

namespace Core::Tools {
	class PaintBrush {
	public:

		glm::vec4 color = glm::vec4(0.2f, 0.8f, 0.2f, 1.0f); // Default: soft green
		float radius = 1.0f;
		float strength = 1.0f;
	};
} 