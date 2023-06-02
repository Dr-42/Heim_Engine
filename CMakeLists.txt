cmake_minimum_required(VERSION 3.10)

project(heimdall-engine)

# Set the compiler to use
set(CMAKE_C_COMPILER gcc)
# Add the source files for the libheimdall-engine library
file(GLOB LIBHEIMDALL_ENGINE_SOURCES
    "engine/ecs/*.c"
    "engine/core/*.c"
    "engine/renderer/*.c"
    "engine/math/*.c"
)

add_library(libheimdall-engine SHARED
    ${LIBHEIMDALL_ENGINE_SOURCES}
)

# Set the include directories for the libheimdall-engine library
target_include_directories(libheimdall-engine PUBLIC
    engine/include
)

# Set the compile flags for the libheimdall-engine library
set_target_properties(libheimdall-engine PROPERTIES
    COMPILE_FLAGS "-g -Wall -Wextra"
)

# Link the libheimdall-engine library with the required libraries
if(CYGWIN)
    target_link_libraries(libheimdall-engine
        glew32
        glfw3
        opengl32
        m
    )
else()
    target_link_libraries(libheimdall-engine
        GLEW
        glfw
        GL
        m
    )
endif()

# Add the source files for the testbed executable
add_executable(testbed
    testbed/main.c
)

# Set the include directories for the testbed executable
target_include_directories(testbed PUBLIC
    include
)

# Set the compile flags for the testbed executable
set_target_properties(testbed PROPERTIES
    COMPILE_FLAGS "-g -Wall"
)

# Link the testbed executable with the libheimdall-engine library
target_link_libraries(testbed
    libheimdall-engine
)