cmake_minimum_required(VERSION 3.10.2)

project(SEXBLOX)

add_subdirectory(depends/glfw)
add_subdirectory(depends/glad)
add_subdirectory(depends/stb)
add_subdirectory(depends/glm)

add_executable(${PROJECT_NAME} 
    src/main.cpp src/model 
    src/modelloader.cpp src/modelloader.h
    src/shader.cpp src/shader.h
    src/texture.cpp src/texture.h
)


target_link_directories(${PROJECT_NAME} PRIVATE depends/glfw/src depends/glad/src)
target_include_directories(${PROJECT_NAME} PUBLIC depends/glfw/include depends/glad/include depends/stb depends/glm/glm)

target_link_libraries(${PROJECT_NAME} glfw glad stb)

