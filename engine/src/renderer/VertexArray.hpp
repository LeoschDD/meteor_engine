#pragma once

#include "renderer/Buffer.hpp"

namespace meteor
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(VertexBuffer& vertex_buffer, BufferLayout& buffer_layout);

        void Bind();
        void Unbind();

    private:
        uint32_t id_;
    };
}