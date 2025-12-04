cmake_minimum_required(VERSION 3.20)
project(cpp-project CXX)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(cpp-project ${SRC_FILES} "include/Figura3D.hpp" "include/Menu.hpp")
target_include_directories(cpp-project PUBLIC "${CMAKE_SOURCE_DIR}/include", cpp-project PRIVATE
    ${CMAKE_SOURCE_DIR}/external/eigen-3.4.0)
set_target_properties(cpp-project PROPERTIES CXX_STANDARD 23)
if(MINGW)
  target_link_libraries(cpp-project "-lstdc++exp")
endif()
