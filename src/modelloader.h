#pragma once
#include <glad/glad.h>
#include "model.h"

class ModelLoader
{
public:
    ModelLoader(RawModel& rawModel);
    
    void Unload();
private:
    unsigned int m_VertexArray;
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
};