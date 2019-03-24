cmake_minimum_required(VERSION 3.0.2)

project(opengl-training)

file(GLOB_RECURSE OGLT_INCLUDE_FILES
    ${CMAKE_SOURCE_DIR}/include/*.h
    ${CMAKE_SOURCE_DIR}/include/*.hpp
)

file(GLOB_RECURSE OGLT_SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)


include_directories(${CMAKE_SOURCE_DIR}/include)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_library(GLAD ${CMAKE_SOURCE_DIR}/src/glad/glad.c)


set(LIBS GLAD assimp)

add_executable(${PROJECT_NAME} ${OGLT_INCLUDE_FILES} ${OGLT_SRC_FILES})
target_link_libraries(${PROJECT_NAME} -lglfw -ldl -lpng ${LIBS})



