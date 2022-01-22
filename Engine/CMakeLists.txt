cmake_minimum_required(VERSION 3.22)

Project(CEENetAppEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED yes)

# Platform independant files.
set(EngineSources src/Application.cpp src/Buffer.cpp src/Camera.cpp src/Context.cpp src/Framebuffer.cpp src/ImGuiImpl.cpp src/ImGuiLayer.cpp src/Layer.cpp src/LayerStack.cpp src/Log.cpp src/OrthographicCameraController.cpp src/RenderCommand.cpp src/Renderer.cpp src/Renderer2D.cpp src/RendererAPI.cpp src/Shader.cpp src/Texture.cpp src/UniformBuffer.cpp src/VertexArray.cpp src/Window.cpp)

# Platform specific files (OpenGL).
list(APPEND EngineSources src/Platform/OpenGLBuffer.cpp src/Platform/OpenGLContext.cpp src/Platform/OpenGLFramebuffer.cpp src/Platform/OpenGLRendererAPI.cpp src/Platform/OpenGLShader.cpp src/Platform/OpenGLTexture.cpp src/Platform/OpenGLUniformBuffer.cpp src/Platform/OpenGLVertexArray.cpp)

# Platform specific files (Linux).
list(APPEND EngineSources src/Platform/LinuxInput.cpp src/Platform/LinuxWindow.cpp)

# ImGui files
list(APPEND EngineSources vendor/imgui/imgui.cpp vendor/imgui/imgui_demo.cpp vendor/imgui/imgui_draw.cpp vendor/imgui/imgui_tables.cpp vendor/imgui/imgui_widgets.cpp)

add_library(Engine ${EngineSources})

set(GLFW_BUILD_DOCS OFF)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/spdlog)

target_include_directories(Engine PUBLIC include/ vendor/glfw/include/ vendor/glad/include/ vendor/glm/ vendor/spdlog/include/ vendor/imgui/ vendor/stb/include)
target_link_libraries(Engine PUBLIC Common glfw glad spdlog)

