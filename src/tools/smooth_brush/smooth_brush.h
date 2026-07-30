#pragma once

#include "../brush.h"

namespace Core::Tools {

	class SmoothBrush : public Brush {
	public:
		SmoothBrush() = default;
		~SmoothBrush() override = default;

		void apply(Terrain& terrain, const glm::vec3& hitPosition, float deltaTime) override;
	};
}