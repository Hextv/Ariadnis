#include "smooth_brush.h"
#include "../../core/terrain/terrain.h"

#include <glm/gtc/constants.hpp>
#include <cmath>
#include <vector>

namespace Core::Tools {

    void SmoothBrush::apply(Terrain& terrain, const glm::vec3& hitPosition, float deltaTime) {
        auto& vertices = terrain.getVertices();
        bool meshModified = false;

        // Gather indices and original heights of vertices within brush radius
        struct AffectedVertex {
            size_t heightIndex;
            float originalY;
            float falloff;
            float vx, vz;
        };

        std::vector<AffectedVertex> targetVertices;

        for (size_t i = 0; i < vertices.size(); i += 3) {
            float vx = vertices[i];
            float vy = vertices[i + 1];
            float vz = vertices[i + 2];

            float distance = glm::distance(
                glm::vec2(vx, vz),
                glm::vec2(hitPosition.x, hitPosition.z)
            );

            if (distance <= radius) {
                float normDistance = distance / radius;
                float falloff = 0.5f * (1.0f + std::cos(glm::pi<float>() * normDistance));

                targetVertices.push_back({ i + 1, vy, falloff, vx, vz });
            }
        }

        if (targetVertices.empty()) return;

        // Calculate average target height across affected area
        float avgHeight = 0.0f;
        for (const auto& v : targetVertices) {
            avgHeight += v.originalY;
        }
        avgHeight /= static_cast<float>(targetVertices.size());

        // Smoothly interpolate (lerp) current height toward local average
        for (const auto& v : targetVertices) {
            float& vy = vertices[v.heightIndex];
            float blendFactor = glm::clamp(strength * v.falloff * deltaTime, 0.0f, 1.0f);
            vy = glm::mix(vy, avgHeight, blendFactor);

            meshModified = true;
        }

        if (meshModified) {
            terrain.updateBuffers();
        }
    }

}