#pragma once

#include <glm/glm.hpp>
#include "../core/terrain/terrain.h"

namespace Core::Tools {

	class Brush {

		virtual ~Brush() = default;
		// Applies the brush action at a given (X, Z) world hit location
		virtual void apply(Terrain& terrain, const glm::vec3& hitPosition, float deltaTime) = 0;

        // Settings getters/setters
        float getRadius() const { return radius; }
        void setRadius(float r) { radius = r; }

        float getStrength() const { return strength; }
        void setStrength(float s) { strength = s; }

    protected:
        float radius = 5.0f;     // Influence radius in world units
        float strength = 2.0f;   // How fast/strongly height changes
    };

}