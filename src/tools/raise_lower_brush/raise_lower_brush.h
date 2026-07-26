#pragma once

#include "../brush.h"

namespace Core::Tools {

    class RaiseLowerBrush : public Brush {
    public:
        RaiseLowerBrush() = default;
        ~RaiseLowerBrush() override = default;

        void apply(Terrain& terrain, const glm::vec3& hitPosition, float deltaTime) override;

        bool isLowering = false; // Set to true to lower terrain
    };

}