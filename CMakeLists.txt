cmake_minimum_required(VERSION 3.24)
project(Briscola)

set(CMAKE_CXX_STANDARD 20)

file(GLOB Briscola_SRC CONFIGURE_DEPENDS "*.h" "*.cpp")
add_executable(Briscola ${Briscola_SRC} graphviz.h)
target_link_libraries(Briscola tbb)
