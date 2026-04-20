#include "renderer/VertexArray.hpp"

meteor::VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &id_);
    glBindVertexArray(id_);
}

meteor::VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &id_);
}

void meteor::VertexArray::AddBuffer(VertexBuffer& vertex_buffer, BufferLayout& buffer_layout)
{
    Bind();
    vertex_buffer.Bind();

    int offset = 0;
    for (int i = 0; i < buffer_layout.Elements().size(); i++)
    {
        const auto& element = buffer_layout.Elements()[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, 
            buffer_layout.Stride(), reinterpret_cast<const void*>(static_cast<uintptr_t>(offset)));
            
        offset += LayoutElement::GetTypeSize(element.type) * element.count;
    }
}

void meteor::VertexArray::Bind()
{
    glBindVertexArray(id_);
}

void meteor::VertexArray::Unbind()
{
    glBindVertexArray(0);
}
