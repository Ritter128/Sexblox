#include "modelloader.h"

ModelLoader::ModelLoader(RawModel& rawModel)
{
    // Generate and bind vertex array
    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    // Generate vertex buffer, and fill it with vertex data
    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, rawModel.vertexSize, rawModel.vertices, GL_STATIC_DRAW);

    // Generate index buffer, and fill it with index data
    unsigned int indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rawModel.indexSize, rawModel.indices, GL_STATIC_DRAW);

    // aPosition attribute, for storing position of a vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
    glEnableVertexArrayAttrib(m_VertexArray, 0);
    // aColor attribute, for storing color of a vertex
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));
    glEnableVertexArrayAttrib(m_VertexArray, 1);
    // aTexCoord attribute, for storing tex coordinate of vertex
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));
    glEnableVertexArrayAttrib(m_VertexArray, 2);
}

void ModelLoader::Unload()
{
    // Clean up buffers
    glDeleteVertexArrays(1, &m_VertexArray);
    glDeleteBuffers(1, &m_VertexBuffer);
    glDeleteBuffers(1, &m_IndexBuffer);
}