cmake_minimum_required(VERSION 3.25)
project(PTG)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address -fno-omit-frame-pointer")

add_compile_definitions(GLFW_HAS_GETPLATFORM=0)
add_compile_definitions(RES_PATH="${CMAKE_SOURCE_DIR}/res/")
add_compile_definitions(TEXTURES_PATH="${CMAKE_SOURCE_DIR}/res/textures/")
# -----------------------------
# Encontrar OpenGL e GLFW
# -----------------------------
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Bibliotecas locais (GLEW e GLFW)
set(GLEW_LIB $ENV{HOME}/.local/lib/libGLEW.a)

# -----------------------------
# Includes
# -----------------------------
include_directories(
    $ENV{HOME}/.local/include      # GLEW e GLFW
    ${CMAKE_SOURCE_DIR}/src/vendor/imgui
    ${CMAKE_SOURCE_DIR}/src/vendor/stb_img
)

# -----------------------------
# ImGui fontes
# -----------------------------
set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/vendor/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/imgui/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/imgui/imgui_impl_opengl3.cpp
)

# -----------------------------
# stb_image
# -----------------------------
set(STB_IMAGE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/vendor/stb_img/stb_image.cpp
)

# -----------------------------
# Executável
# -----------------------------
add_executable(app
    src/main.cpp
    src/IndexBuffer.cpp
    src/Renderer.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/TileSet.cpp
    src/VertexArray.cpp
    src/VertexBuffer.cpp
    src/VertexBufferLayout.cpp
    src/Wfc.cpp
    src/PerlinNoise.cpp
    ${IMGUI_SOURCES}
    ${STB_IMAGE_SOURCES}
)

# -----------------------------
# Linkagem
# -----------------------------
target_link_libraries(app
    glfw
    ${GLEW_LIB}
    OpenGL::GL
    dl pthread X11 Xrandr Xi
)
