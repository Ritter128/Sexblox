#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "modelloader.h"
#include "shader.h"

/* GLOBALS */
glm::vec3 cratePosition = glm::vec3(0.0f);

const std::string vsSource = R"(
#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;

out vec3 vertexColor;
out vec2 texCoord;

void main()
{
    gl_Position = uProjMatrix * uModelMatrix * vec4(aPosition, 1.0);
    vertexColor = aColor;
    texCoord = aTexCoord;
}
)";

const std::string fsSource = R"(
#version 460 core

uniform sampler2D textureSample;

in vec3 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

void main()
{
    fragColor = texture(textureSample, texCoord) * vec4(vertexColor, 1.0);
}
)";

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W)
    {
        cratePosition.z += 0.01;
    }
    if (key == GLFW_KEY_S)
    {
        cratePosition.z -= 0.01;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 400, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, OnKey);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Failed to load GLAD\n";
        return -1;
    }
    Vertex vertices[] = {
        glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f),
        glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f),
        glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 1.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f),

	    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f),
        glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f),
        glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 1.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f),
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2,
	    4, 5, 6,
	    5, 7, 6
    };

    RawModel model = {12, vertices, indices, sizeof(vertices), sizeof(indices)};

    unsigned int textureID;

    unsigned char* imageData;
    int imageX, imageY, nrChannels;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    imageData = stbi_load("Textures/crate.jpg", &imageX, &imageY, &nrChannels, 0);
    if (!imageData)
    {
        std::cout << "Could not load image\n";
        stbi_image_free(imageData);
    }
    else 
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageX, imageY, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(imageData);

    Shader shaderProgram(vsSource, fsSource);

    ModelLoader loader(model);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, cratePosition);
        glm::mat4 projMatrix = glm::perspective(glm::radians(65.0f), (float)600/400, 0.001f, 100.0f);

        shaderProgram.SetUniformInt("textureSample", 0);
        shaderProgram.SetUniformMatrix4("uModelMatrix", modelMatrix);
        shaderProgram.SetUniformMatrix4("uProjMatrix", projMatrix);

        glDrawElements(GL_TRIANGLES, model.count, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteTextures(1, &textureID);
    shaderProgram.Unload();
    loader.Unload();
    glfwTerminate();
    return 0;
}
