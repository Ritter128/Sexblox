#include "texture.h"
#include <stb_image.h>
#include <iostream>

void Texture::Init(int level, const char* path)
{
    unsigned char* imageData;
    int imageX, imageY, nrChannels;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    imageData = stbi_load(path, &imageX, &imageY, &nrChannels, 0);
    if (!imageData)
    {
        std::cout << "Could not load image\n";
        stbi_image_free(imageData);
    }
    else 
    {
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageX, imageY, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(imageData);
}

void Texture::Unload()
{
    glDeleteTextures(1, &m_TextureID);
}

unsigned int Texture::GetID()
{
    return m_TextureID;
}