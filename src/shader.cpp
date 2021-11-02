#include "shader.h"
#include <iostream>

Shader::Shader(const std::string& vs, const std::string& fs)
{
    // Compile source code of shaders
    unsigned int vertexShaderID = CompileShader(vs, GL_VERTEX_SHADER);
    unsigned int fragShaderID = CompileShader(fs, GL_FRAGMENT_SHADER);

    // Create program, attach compiled shaders and link the program
    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vertexShaderID);
    glAttachShader(m_ProgramID, fragShaderID);
    glLinkProgram(m_ProgramID);

    // Activate program
    glUseProgram(m_ProgramID);

    // Delete shaders
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragShaderID);
}

void Shader::SetUniformInt(const std::string name, int value)
{
    unsigned int location = glGetUniformLocation(m_ProgramID, name.c_str());
    glUniform1i(location, value);
}

void Shader::SetUniformMatrix4(const std::string name, glm::mat4 value)
{
    unsigned int location = glGetUniformLocation(m_ProgramID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::Unload()
{
    // Clean up program
    glDeleteProgram(m_ProgramID);
}

unsigned int Shader::GetProgramID()
{
    return m_ProgramID;
}

unsigned int Shader::CompileShader(const std::string& src, unsigned int type)
{
    unsigned int shaderID = glCreateShader(type);
    const char* cSrc = src.c_str();
    glShaderSource(shaderID, 1, &cSrc, 0);
    glCompileShader(shaderID);

    int compileStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE)
    {
        switch (type)
        {
            case GL_VERTEX_SHADER:
            {
                std::cout << "[VERTEX SHADER ERROR]\n";
                break;
            }
            case GL_FRAGMENT_SHADER:
            {
                std::cout << "[FRAGMENT SHADER ERROR]\n";
                break;
            }
        }   

        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

        char* infoLog = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(shaderID, length, &length, infoLog);

        std::cout << infoLog << "\n";

        return 0;
    }
    return shaderID;
}