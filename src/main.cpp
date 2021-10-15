#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <string>


struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
};

const std::string vsSource = R"(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

void main()
{
    gl_Position = aPosition;
}
)";

const std::string fsSource = R"(
#version 330 core

out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Failed to load GLAD\n";
        return -1;
    }

    Vertex vertices[] = {
        glm::vec3(-0.5f, -0.5f,  0.0f), glm::vec3( 1.0f,  0.0f,  0.0f),
        glm::vec3( 0.5f, -0.5f,  0.0f), glm::vec3( 0.0f,  1.0f,  0.0f),
        glm::vec3(-0.5f,  0.5f,  0.0f), glm::vec3( 0.0f,  0.0f,  1.0f),
        glm::vec3( 0.5f,  0.5f,  0.0f), glm::vec3( 1.0f,  1.0f,  0.0f),
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2,
    };

    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    const char* cVSSource = vsSource.c_str();
    glShaderSource(vertexShaderID, 1, &cVSSource, 0);
    glCompileShader(vertexShaderID);

    unsigned int fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    const char* cFSSource = fsSource.c_str();
    glShaderSource(fragShaderID, 1, &cFSSource, 0);
    glCompileShader(fragShaderID);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragShaderID);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragShaderID);

    unsigned int vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    unsigned int vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // aPosition
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // aColor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &indexBufferID);
    glfwTerminate();
    return 0;
}