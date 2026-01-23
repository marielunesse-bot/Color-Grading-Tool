cmake_minimum_required(VERSION 3.20)

project(ColorGradingTool
    VERSION 0.1.0
    LANGUAGES CXX
)

# ===============================
# Compiler settings
# ===============================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Utiliser clang++
if (NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER clang++)
endif()

# ===============================
# Dependencies
# ===============================

find_package(OpenCV REQUIRED)
...
target_link_libraries(${PROJECT_NAME}
      PRIVATE
     ${OpenCV_LIBS}
     SDL3::SDL3
     OpenGL::GL
)

# SDL3 (préparé pour plus tard)
find_package(SDL3 REQUIRED)
...
target_link_libraries(${PROJECT_NAME}
      PRIVATE
     ${OpenCV_LIBS}
     SDL3::SDL3
)

# OpenGL (préparé pour plus tard)
find_package(OpenGL QUIET)

# ===============================
# Sources
# ===============================

set(SOURCE_FILES
    src/main.cpp
    src/Core/Application.cpp
    src/Core/Logger.cpp
    src/Engine/Image/ImageLoader.cpp
)

# ===============================
# Executable
# ===============================
add_subdirectory(src/External/imgui)

target_link_libraries(${PROJECT_NAME}
 PRIVATE 
    ${OpenCV_LIBS}
    SDL3::SDL3
    OpenGL::GL
    imgui
)



add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

# ===============================
# Include directories
# ===============================

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        ${OpenCV_INCLUDE_DIRS}
)

# ===============================
# Link libraries
# ===============================

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS}
)

# ===============================
# Warnings (pro)
# ===============================

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()