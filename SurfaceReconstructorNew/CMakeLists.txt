cmake_minimum_required (VERSION 2.6)
project (SurfaceReconstruction)

set(PROJECT_LINK_LIBS glfw png16 soil tinyxml2)

find_library(COREVIDEO CoreVideo )
if (NOT COREVIDEO)
    message(FATAL_ERROR "yeah")
endif()

find_library(OPENGL OpenGL)
find_library(IOKIT IOKit)
find_library(COCOA Cocoa)
find_library(CARBON Carbon)

set(PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} ${COREVIDEO} ${OPENGL} ${IOKIT} ${COCOA} ${CARBON})

include_directories(. include)
link_directories(../lib)

set (SOURCES glad.c )
file (GLOB InterfaceSrc "Interface/*.cpp")
file (GLOB UtilSrc "catpaw/*.cpp")
file (GLOB Src "*.cpp")

set (SOURCES ${SOURCES} ${Src})

add_executable(main ${SOURCES} ${UtilSrc})
target_link_libraries(main ${PROJECT_LINK_LIBS})

