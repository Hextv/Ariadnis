#include "raise_lower_brush.h"
#include "../../core/terrain/terrain.h"

#include <glm/gtc/constants.hpp>
#include <cmath>

namespace Core::Tools {

    void RaiseLowerBrush::apply(Terrain& terrain, const glm::vec3& hitPosition, float deltaTime) {
        // +1.0f raises terrain, -1.0f lowers terrain
        float direction = isLowering ? -1.0f : 1.0f;

        // Retrieve reference to flat vertex float array
        auto& vertices = terrain.getVertices();

        bool meshModified = false;

        // Iterate through vertex array stepping by 3 (X, Y, Z)
        for (size_t i = 0; i < vertices.size(); i += 3) {
            float vx = vertices[i];       // X coordinate
            float& vy = vertices[i + 1];  // Y coordinate (Height reference)
            float vz = vertices[i + 2];   // Z coordinate

            // Distance on the XZ plane (ignore Y height)
            float distance = glm::distance(
                glm::vec2(vx, vz),
                glm::vec2(hitPosition.x, hitPosition.z)
            );

            if (distance <= radius) {
                // Smooth Cosine falloff (1.0 at center, 0.0 at radius edge)
                float normDistance = distance / radius;
                float falloff = 0.5f * (1.0f + std::cos(glm::pi<float>() * normDistance));

                // Apply height change directly to the Y reference
                vy += direction * strength * falloff * deltaTime;
                meshModified = true;
            }
        }

        // Re-upload vertex buffer to GPU if modifications occurred
        if (meshModified) {
            terrain.updateBuffers();
        }
    }

}