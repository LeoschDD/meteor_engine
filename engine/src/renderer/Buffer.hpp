#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "core/Core.hpp"

namespace meteor
{
    ////////////////////////////////////////////////////
    // Buffer layout ///////////////////////////////////
    ////////////////////////////////////////////////////

    struct LayoutElement
    {
        uint32_t type;
        uint32_t count;
        bool normalized;

        static uint32_t GetTypeSize(uint32_t type)
        {
            switch (type)
            {
                case GL_FLOAT: return 4;
                case GL_UNSIGNED_BYTE: return 4;
                case GL_UNSIGNED_INT: return 4;
                case GL_INT: return 4;
                case GL_BOOL: return 1;               
            }
            METEOR_CORE_ASSERT(false, "Tried to get unsupported layout element type");  
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;

        void Push(uint32_t type, uint32_t count, bool normalized = false);
        
        [[nodiscard]] const std::vector<LayoutElement>& Elements() const {return elements_;}
        [[nodiscard]] uint32_t Stride() {return stride_;}

    private:
        std::vector<LayoutElement> elements_;
        uint32_t stride_{0};
    };

    ////////////////////////////////////////////////////
    // Vertex buffer ///////////////////////////////////
    ////////////////////////////////////////////////////

    class VertexBuffer
    {
    public:
        VertexBuffer(const std::vector<float>& vertices);
        ~VertexBuffer();

        void Bind();
        void Unbind();

    private:
        uint32_t id_;
    };

    ////////////////////////////////////////////////////
    // Index buffer ////////////////////////////////////
    ////////////////////////////////////////////////////

    class IndexBuffer
    {
    public:
        IndexBuffer(const std::vector<uint32_t>& indices);
        ~IndexBuffer();        

        void Bind();
        void Unbind();
        
        [[nodiscard]] uint32_t GetCount() {return count_;}

    private:
        uint32_t id_;
        uint32_t count_;
    };  
}