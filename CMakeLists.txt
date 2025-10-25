cmake_minimum_required(VERSION 3.29)
project(untitled2)
set(CMAKE_CXX_STANDARD 11)  # ✅ Zmeň na C++11 (stačí pre projekt)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========================================
# HOMEBREW SETUP
# ========================================
execute_process(
        COMMAND brew --prefix
        OUTPUT_VARIABLE HOMEBREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Homebrew prefix: ${HOMEBREW_PREFIX}")

# ========================================
# GLFW
# ========================================
find_path(GLFW_INCLUDE_DIR
        NAMES GLFW/glfw3.h
        HINTS ${HOMEBREW_PREFIX}/include
)
if (GLFW_INCLUDE_DIR)
    message(STATUS "Found GLFW include directory: ${GLFW_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find GLFW include directory")
endif()

find_library(GLFW_LIB
        NAMES glfw glfw3
        HINTS ${HOMEBREW_PREFIX}/lib
)
if (GLFW_LIB)
    message(STATUS "Found GLFW library: ${GLFW_LIB}")
else()
    message(FATAL_ERROR "Could not find GLFW library")
endif()

# ========================================
# GLM (header-only)
# ========================================
find_path(GLM_INCLUDE_DIR
        NAMES glm/glm.hpp
        HINTS ${HOMEBREW_PREFIX}/include
)
if (GLM_INCLUDE_DIR)
    message(STATUS "Found GLM include directory: ${GLM_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find GLM include directory")
endif()

# ========================================
# GLEW
# ========================================
find_path(GLEW_INCLUDE_DIR
        NAMES GL/glew.h
        HINTS ${HOMEBREW_PREFIX}/include
)
if (GLEW_INCLUDE_DIR)
    message(STATUS "Found GLEW include directory: ${GLEW_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find GLEW include directory")
endif()

find_library(GLEW_LIB
        NAMES GLEW glew
        HINTS ${HOMEBREW_PREFIX}/lib
)
if (GLEW_LIB)
    message(STATUS "Found GLEW library: ${GLEW_LIB}")
else()
    message(FATAL_ERROR "Could not find GLEW library")
endif()

# ========================================
# SOURCE FILES (BEZ DUPLICÍT!)
# ========================================
set(SOURCES
        # Main
        src/main.cpp

        # Application
        src/Application.cpp
        src/Application.h

        # Shader system
        src/Shader.cpp
        src/Shader.h
        src/ShaderProgram.cpp
        src/ShaderProgram.h

        # Models
        src/Model.cpp
        src/Model.h

        # Transformations
        src/TransformComponent.cpp
        src/TransformComponent.h
        src/Translate.cpp
        src/Translate.h
        src/Rotate.cpp
        src/Rotate.h
        src/Scale.cpp
        src/Scale.h
        src/TransformComposite.cpp
        src/TransformComposite.h

        # Objects & Scenes
        src/DrawableObject.cpp
        src/DrawableObject.h
        src/Scene.cpp
        src/Scene.h
        src/SceneManager.cpp
        src/SceneManager.h
        src/Camera.cpp
        src/Camera.h
        src/models/plane.h
        src/models/lamp.h
        src/models/bench.h
        src/Contoller.cpp
        src/Contoller.h
        src/Light.cpp
        src/Light.h
        src/ShaderLoader.cpp
        src/ShaderLoader.h
        src/models/triangle.h
        src/Subject.h
        src/Subject.cpp
        src/SceneFactory.cpp
        src/SceneFactory.h
)

# ========================================
# EXECUTABLE
# ========================================
add_executable(untitled2 ${SOURCES})

# ========================================
# INCLUDE DIRECTORIES
# ========================================
target_include_directories(untitled2 PRIVATE
        ${GLFW_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/src        # ✅ src/ priečinok
        ${CMAKE_SOURCE_DIR}/src/models # ✅ models/ priečinok!
)

# ========================================
# LINKING
# ========================================
target_link_libraries(untitled2 PRIVATE
        ${GLFW_LIB}
        ${GLEW_LIB}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework OpenGL"
        "-framework CoreFoundation"
        "-lobjc"
)

# Potlačenie deprecated warnings
target_compile_definitions(untitled2 PRIVATE GL_SILENCE_DEPRECATION)