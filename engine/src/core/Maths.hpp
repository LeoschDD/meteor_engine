#pragma once

#include "core/Headers.hpp"

namespace meteor
{
    inline glm::mat4 BuildTransformMatrix(glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
    {
        glm::mat4 transform_matrix = glm::mat4(1.0f);
        transform_matrix = glm::translate(transform_matrix, translation);
        transform_matrix *= glm::mat4_cast(glm::normalize(rotation));
        transform_matrix = glm::scale(transform_matrix, scale);
        return transform_matrix;
    }
}