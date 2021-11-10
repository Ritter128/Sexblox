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
glm::vec3 worldPosition = glm::vec3(0.0f);
glm::vec3 cameraPosition = glm::vec3(0.0f);
glm::vec3 cameraRotation = glm::vec3(0.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f);
float cameraAngle = 0.0f;

const std::string vsSource = R"(
#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uCameraMatrix;

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
uniform int u_BUseTexture;

in vec3 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

void main()
{
    if (u_BUseTexture == 1)
        fragColor = texture(textureSample, texCoord) * vec4(vertexColor, 1.0);
    else
        fragColor = vec4(vertexColor, 1.0);
}
)";

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E)
    {
        cameraRotation.y += 0.04f;
    }
    if (key == GLFW_KEY_Q)
    {
        cameraRotation.y -= 0.04f;
    }


    if (key == GLFW_KEY_W)
    {
        worldPosition.z -= 0.01;
    }
    if (key == GLFW_KEY_S)
    {
        worldPosition.z += 0.01;
    }


    if (key == GLFW_KEY_D)
    {
        worldPosition.x -= 0.01;
    }
    if (key == GLFW_KEY_A)
    {
        worldPosition.x += 0.01;
    }
}

int main()
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

    TexturedModel model = {12, vertices, indices, sizeof(vertices), sizeof(indices)};
    model.texture.Init(0, "Textures/crate.jpg");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);

    Shader shaderProgram(vsSource, fsSource);
    ModelLoader loader(model.rawModel);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, worldPosition);
        glm::mat4 projMatrix = glm::perspective(glm::radians(65.0f), (float)600/400, 0.001f, 100.0f);
        projMatrix = glm::translate(projMatrix, cameraPosition);
        projMatrix = glm::rotate(projMatrix, cameraRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        //glm::mat4 cameraMatrix = glm::rotate(projMatrix, cameraRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 cameraMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f));
        cameraMatrix = glm::translate(cameraMatrix, cameraPosition);
        
        shaderProgram.SetUniformInt("textureSample", 0);
        
        shaderProgram.SetUniformInt("u_BUseTexture", 1);
        shaderProgram.SetUniformInt("u_BUseTexture", 1);

        shaderProgram.SetUniformMatrix4("uModelMatrix", modelMatrix);
        shaderProgram.SetUniformMatrix4("uProjMatrix", projMatrix);
        shaderProgram.SetUniformMatrix4("uCameraMatrix", cameraMatrix);

        glDrawElements(GL_TRIANGLES, model.rawModel.count, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    shaderProgram.Unload();
    loader.Unload();
    model.texture.Unload();
    glfwTerminate();
    return 0;
}
