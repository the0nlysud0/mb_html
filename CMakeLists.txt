cmake_minimum_required(VERSION 3.16)

project(mb_html)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories("Include/")

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/mb_html-lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Adding Source Files
# add_executable(ModelLoader "Source/main.cpp"
#               "Source/mb_logger.cpp")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    Source/core/*.cpp
    Source/utils/*.cpp
    Source/*.cpp
    Source/*.c
)

add_executable(mb_html ${SRC_FILES})

# macOS config
if(APPLE)
    link_directories("${CMAKE_SOURCE_DIR}/Libs/macOS")
    target_compile_definitions(mb_html PRIVATE MACOS)
    # Link against glfw3 library and the apple default frameworks
    target_link_libraries(mb_html)
elseif(WIN32)
    link_directories("${CMAKE_SOURCE_DIR}/Libs/windows")
    target_compile_definitions(mb_html PRIVATE WINDOWS)
    find_package(PNG REQUIRED)
    target_link_libraries(mb_html
        "${CMAKE_SOURCE_DIR}/Libs/liblexbor_static.a"
    )
elseif(UNIX)
    link_directories("${CMAKE_SOURCE_DIR}/Libs/linux")
    target_compile_definitions(mb_html PRIVATE LINUX)
    find_package(PNG REQUIRED)
    target_link_libraries(mb_html)


endif()

target_compile_definitions(mb_html PRIVATE DEBUG)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets"
        "${OUTPUT_DIR}/assets"
    COMMENT "Copying assets to build directory..."
)

add_dependencies(mb_html copy_assets)
