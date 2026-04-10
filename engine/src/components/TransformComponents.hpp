#pragma once

#include "core/Headers.hpp"

namespace meteor
{
    ////////////////////////////////////////////////////
    // 3D local and global transform ///////////////////
    ////////////////////////////////////////////////////

    struct Transform3DComponent
    {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    };

    struct GlobalTransform3DComponent
    {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    };

    ////////////////////////////////////////////////////
    // 2D local and global transform ///////////////////
    ////////////////////////////////////////////////////

    struct Transform2DComponent
    {
        glm::vec2 position{0.0f, 0.0f};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation{0.0f};
    };

    struct GlobalTransform2DComponent
    {
        glm::vec2 position{0.0f, 0.0f};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation{0.0f};
    };
}