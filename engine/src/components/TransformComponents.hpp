#pragma once

#include "core/Headers.hpp"

namespace meteor
{
    ////////////////////////////////////////////////////
    // 3D local and global transform ///////////////////
    ////////////////////////////////////////////////////

    struct Transform3DComponent
    {
        glm::vec3 position;
        glm::vec3 scale;
        glm::quat rotation;
    };

    struct GlobalTransform3DComponent
    {
        glm::vec3 position;
        glm::vec3 scale;
        glm::quat rotation;
    };

    ////////////////////////////////////////////////////
    // 2D local and global transform ///////////////////
    ////////////////////////////////////////////////////

    struct Transform2DComponent
    {
        glm::vec2 position;
        glm::vec2 scale;
        float rotation;
    };

    struct GlobalTransform2DComponent
    {
        glm::vec2 position;
        glm::vec2 scale;
        float rotation;
    };
}