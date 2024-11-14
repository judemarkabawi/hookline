cmake_minimum_required(VERSION 3.19)

# Make sure to init vcpkg correctly before this
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
project(hookline)

#=======================================================#
# Compiling
#=======================================================#

# Enforce C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source dir
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add source files
file(GLOB_RECURSE SOURCES
    ${SOURCE_DIR}/*.cpp
    ${SOURCE_DIR}/*.hpp
)

# Create executable
if (MSVC)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
else ()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif ()

# Include headers
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

#=======================================================#
# Compiler Flags
#=======================================================#

# Compiler flags for Clang (Linux, Mac) and MSVC (Windows)
if(MSVC)
    # Compiler flags for MSVC
    target_compile_options(${PROJECT_NAME} PRIVATE
        # Debug vs. Release compiler options
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/O2>
        # Always on compiler options
        /W4  # Warning level 4 and treat warnings as errors
    )
else()
    # Compiler flags for Clang
    target_compile_options(${PROJECT_NAME} PRIVATE
        # Debug vs. Release compiler options
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O3>
        # Always on compiler options
        -Wall -Wextra -Werror 
        # Uncomment if needed: -Wpedantic -fsanitize=address -fsanitize=undefined
    )
endif()

#=======================================================#
# Libraries 
#=======================================================#
find_package(EnTT CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(OpusFile CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE
    # entt
    EnTT::EnTT
    # GLEW
    GLEW::GLEW
    # OpenGL
    OpenGL::GL
    OpenGL::GLU
    # libpng
    PNG::PNG
    # SDL
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    # OpusFile
    OpusFile::opusfile
)
# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor)


#=======================================================#
# Formatting 
#=======================================================#
add_custom_target(format
    COMMAND clang-format -i ${SOURCES}
)
