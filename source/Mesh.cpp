#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices,
    uint32_t floatsPerVertex)
{
    setup(vertices, indices, floatsPerVertex);
}

void Mesh::setup(const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices,
    uint32_t floatsPerVertex)
{
    m_floatsPerVertex = floatsPerVertex;

    if (vertices.empty()) {
        std::cerr << "Mesh::setup - vertices empty\n";
        return;
    }
    if (vertices.size() % floatsPerVertex != 0) {
        std::cerr << "Mesh::setup - vertex data size not divisible by floatsPerVertex\n";
        return;
    }

    m_vertexCount = static_cast<unsigned int>(vertices.size() / floatsPerVertex);
    m_indexCount = static_cast<unsigned int>(indices.size());

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    if (m_indexCount > 0) glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    //vbo
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
        vertices.data(),
        GL_STATIC_DRAW);

    //ebo(if exists)
    if (m_indexCount > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
            indices.data(),
            GL_STATIC_DRAW);
    }

    //stride in bytes
    const GLsizei stride = static_cast<GLsizei>(m_floatsPerVertex * sizeof(float));
    // position = first 3 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);

    m_ebo = m_vbo = m_vao = 0;
    m_vertexCount = m_indexCount = 0;
}

void Mesh::bind() const {
    glBindVertexArray(m_vao);
}

void Mesh::unbind() const {
    glBindVertexArray(0);
}

void Mesh::draw(GLenum mode) const {
    if (!m_vao) return;

    glBindVertexArray(m_vao);
    if (m_indexCount > 0) {
        glDrawElements(mode, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, nullptr);
    }
    else {
        glDrawArrays(mode, 0, static_cast<GLsizei>(m_vertexCount));
    }
    glBindVertexArray(0);
}

//move constructor
Mesh::Mesh(Mesh&& other) noexcept
    : m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo),
    m_vertexCount(other.m_vertexCount), m_indexCount(other.m_indexCount),
    m_floatsPerVertex(other.m_floatsPerVertex)
{
    other.m_vao = other.m_vbo = other.m_ebo = 0;
    other.m_vertexCount = other.m_indexCount = 0;
    other.m_floatsPerVertex = 3;
}

//move assignment
Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        if (m_ebo) glDeleteBuffers(1, &m_ebo);
        if (m_vbo) glDeleteBuffers(1, &m_vbo);
        if (m_vao) glDeleteVertexArrays(1, &m_vao);

        m_vao = other.m_vao;
        m_vbo = other.m_vbo;
        m_ebo = other.m_ebo;
        m_vertexCount = other.m_vertexCount;
        m_indexCount = other.m_indexCount;
        m_floatsPerVertex = other.m_floatsPerVertex;

        other.m_vao = other.m_vbo = other.m_ebo = 0;
        other.m_vertexCount = other.m_indexCount = 0;
        other.m_floatsPerVertex = 3;
    }
    return *this;
}
