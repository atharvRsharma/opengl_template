#pragma once

#include <glad/glad.h>
#include <vector>
#include <cstdint>
#include <iostream>

class Mesh {
public:
    Mesh(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices = {},
        uint32_t floatsPerVertex = 3);

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) noexcept;
    Mesh& operator=(Mesh&&) noexcept;

    ~Mesh();

    void bind() const;
    void unbind() const;

    void draw(GLenum mode = GL_TRIANGLES) const;

    bool valid() const { return m_vao != 0; }
    unsigned int vertexCount() const { return m_vertexCount; }
    unsigned int indexCount()  const { return m_indexCount; }

private:
    void setup(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices,
        uint32_t floatsPerVertex);

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;

    unsigned int m_vertexCount = 0; 
    unsigned int m_indexCount = 0;  
    uint32_t m_floatsPerVertex = 3;
};
