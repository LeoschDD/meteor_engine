#include "renderer/Buffer.hpp"
#include "core/Headers.hpp"

////////////////////////////////////////////////////
// Buffer layout ///////////////////////////////////
////////////////////////////////////////////////////

void meteor::BufferLayout::Push(uint32_t type, uint32_t count, bool normalized)
{
    uint32_t size = LayoutElement::GetTypeSize(type); 
    if (size != 0) 
    {
        elements_.push_back({type, count, normalized}); 
        stride_ += size * count;
    }
}

////////////////////////////////////////////////////
// Vertex buffer ///////////////////////////////////
////////////////////////////////////////////////////

meteor::VertexBuffer::VertexBuffer(const std::vector<float>& vertices)
{
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

meteor::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &id_);
}

void meteor::VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id_);
}

void meteor::VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

////////////////////////////////////////////////////
// Index buffer ////////////////////////////////////
////////////////////////////////////////////////////

meteor::IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices)
    : count_(indices.size())
{
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
}

meteor::IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &id_);
}

void meteor::IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void meteor::IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
