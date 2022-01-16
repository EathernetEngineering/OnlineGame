cmake_minimum_required(VERSION 3.22)

Project(CEENetAppEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED yes)

set(EngineSources src/Log.cpp src/Application.cpp src/Layer.cpp src/LayerStack.cpp src/Window.cpp src/Context.cpp src/RendererAPI.cpp src/RenderCommand.cpp src/Renderer.cpp src/VertexArray.cpp src/Buffer.cpp src/Shader.cpp)

list(APPEND EngineSources src/Platform/LinuxWindow.cpp src/Platform/OpenGLContext.cpp src/Platform/OpenGLRendererAPI.cpp src/Platform/OpenGLVertexArray.cpp src/Platform/OpenGLBuffer.cpp src/Platform/OpenGLShader.cpp)

add_library(Engine ${EngineSources})

set(GLFW_BUILD_DOCS OFF)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/spdlog)

target_include_directories(Engine PUBLIC include/ vendor/glfw/include/ vendor/glad/include/ vendor/glm/ vendor/spdlog/include/)
target_link_libraries(Engine PUBLIC Common glfw glad spdlog)

