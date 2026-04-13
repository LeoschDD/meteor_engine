#pragma once

#include "core/Headers.hpp"

namespace meteor
{
    struct TransformComponent
    {
        glm::vec3 translation{0.0f, 0.0f, 0.0f};
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
    };

    struct GlobalTransformComponent
    {
        glm::mat4 transform{1.0f};
    };
}