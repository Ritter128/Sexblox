#include <vector>
#include <glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
};

struct RawModel
{
    unsigned int count;
    Vertex* vertices;
    unsigned int* indices;
    unsigned int vertexSize, indexSize;
};