cmake_minimum_required(VERSION 3.0.0)
project(fractal_project VERSION 0.1.0)

include(CTest)
enable_testing()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")

# MY_DEBUG flag only defined in debug mode
# https://stackoverflow.com/questions/8591762/ifdef-debug-with-cmake-independent-from-platform
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMY_DEBUG")

add_executable(${PROJECT_NAME} "")

add_subdirectory("src")
add_subdirectory("header")
add_subdirectory("extlibs_src/SFML-2.5.1")
add_subdirectory("extlibs_src/TGUI-0.9/TGUI")


TARGET_LINK_LIBRARIES(fractal_project
  sfml-graphics tgui
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


