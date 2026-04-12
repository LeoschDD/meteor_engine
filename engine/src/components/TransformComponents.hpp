#pragma once

#include "core/Headers.hpp"

namespace meteor
{
    class TransformSystem;

    ////////////////////////////////////////////////////
    // 3D transform ////////////////////////////////////
    ////////////////////////////////////////////////////

    class Transform3DComponent
    {
    private:
        friend class TransformSystem;

    public:
        Transform3DComponent() = default;

        [[nodiscard]] glm::vec3& Translation() {dirty_ = true; return local_translation_;}
        [[nodiscard]] const glm::vec3& Translation() const {return local_translation_;}
        [[nodiscard]] glm::quat& Rotation() {dirty_ = true; return local_rotation_;}
        [[nodiscard]] const glm::quat& Rotation() const {return local_rotation_;}
        [[nodiscard]] glm::vec3& Scale() {dirty_ = true; return local_scale_;}
        [[nodiscard]] const glm::vec3& Scale() const {return local_scale_;}

        [[nodiscard]] const glm::vec3& GlobalTranslation() const {return global_translation_;}
        [[nodiscard]] const glm::quat& GlobalRotation() const {return global_rotation_;}
        [[nodiscard]] const glm::vec3& GlobalScale() const {return global_scale_;}

    private:
        bool dirty_{false};

        glm::vec3 local_translation_{0.0f, 0.0f, 0.0f};
        glm::quat local_rotation_{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 local_scale_{1.0f, 1.0f, 1.0f};

        glm::vec3 global_translation_{0.0f, 0.0f, 0.0f};
        glm::quat global_rotation_{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 global_scale_{1.0f, 1.0f, 1.0f};
    };

    ////////////////////////////////////////////////////
    // 2D transform ////////////////////////////////////
    ////////////////////////////////////////////////////

    class Transform2DComponent
    {
    private:
        friend class TransformSystem;
        
    public:
        Transform2DComponent() = default;

        [[nodiscard]] glm::vec2& Translation() {dirty_ = true; return local_translation_;}
        [[nodiscard]] const glm::vec2& Translation() const {return local_translation_;}
        [[nodiscard]] float& Rotation() {dirty_ = true; return local_rotation_;}
        [[nodiscard]] const float& Rotation() const {return local_rotation_;}
        [[nodiscard]] glm::vec2& Scale() {dirty_ = true; return local_scale_;}
        [[nodiscard]] const glm::vec2& Scale() const {return local_scale_;}

        [[nodiscard]] const glm::vec2& GlobalTranslation() const {return global_translation_;}
        [[nodiscard]] const float& GlobalRotation() const {return global_rotation_;}
        [[nodiscard]] const glm::vec2& GlobalScale() const {return global_scale_;}

    private:
        bool dirty_{false};

        glm::vec2 local_translation_{0.0f, 0.0f};
        float local_rotation_{0.0f};
        glm::vec2 local_scale_{1.0f, 1.0f};

        glm::vec2 global_translation_{0.0f, 0.0f};
        float global_rotation_{0.0f};
        glm::vec2 global_scale_{1.0f, 1.0f};
    };
}