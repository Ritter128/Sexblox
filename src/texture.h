#pragma once
#include <glad/glad.h>

class Texture 
{
public:
    void Init(int level, const char* path);

    void Unload();

    unsigned int GetID();
private:
    unsigned int m_TextureID;
};