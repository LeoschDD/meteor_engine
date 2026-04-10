#pragma once

namespace meteor
{
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