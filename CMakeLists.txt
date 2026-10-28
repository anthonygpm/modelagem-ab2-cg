cmake_minimum_required(VERSION 3.16)
project(SalaDeAula3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

# OpenGL + GLU (já vêm com o sistema operacional / compilador)
find_package(OpenGL REQUIRED)

# GLUT: usa o instalado no sistema; se não houver, baixa e compila o freeglut
find_package(GLUT QUIET)
if(GLUT_FOUND)
    set(BIBLIOTECA_GLUT GLUT::GLUT)
else()
    message(STATUS "GLUT nao encontrado no sistema - baixando o freeglut")
    include(FetchContent)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)   # necessário no CMake 4.x
    set(FREEGLUT_BUILD_DEMOS       OFF CACHE BOOL "" FORCE)
    set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(FREEGLUT_BUILD_STATIC_LIBS ON  CACHE BOOL "" FORCE)
    FetchContent_Declare(freeglut
        URL https://github.com/freeglut/freeglut/releases/download/v3.6.0/freeglut-3.6.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_MakeAvailable(freeglut)
    set(BIBLIOTECA_GLUT freeglut_static)
    include_directories(${freeglut_SOURCE_DIR}/include)
endif()

add_executable(sala-de-aula
    src/main.cpp
    src/Camera.cpp
    src/Primitivas.cpp
)

target_include_directories(sala-de-aula PRIVATE src)
target_link_libraries(sala-de-aula PRIVATE ${BIBLIOTECA_GLUT} OpenGL::GLU OpenGL::GL)
