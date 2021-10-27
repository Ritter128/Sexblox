#pragma once
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>

class Shader 
{
public:
    Shader(const std::string& vs, const std::string& fs);

    void SetUniformInt(const std::string name, int value);
    void SetUniformMatrix4(const std::string name, glm::mat4 value);

    unsigned int GetProgramID();

    void Unload();  
private:
    unsigned int m_ProgramID;

    unsigned int CompileShader(const std::string& src, unsigned int type);
};