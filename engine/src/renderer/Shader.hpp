#pragma once

#include "core/Log.hpp"
#include "core/Core.hpp"

namespace meteor
{
    class Shader
    {
    public:
        Shader(const std::filesystem::path vs_path, const std::filesystem::path fs_path);
        ~Shader();
        
        void Bind();
        void Unbind();

    private:
        uint32_t id_;
    };
}
