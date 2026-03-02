cmake_minimum_required(VERSION 3.20)
project(LearnOpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(ENABLE_CLANG_TIDY_ON_BUILD "Run clang-tidy during normal builds" ON)

find_package(glfw3  CONFIG REQUIRED)
find_package(glad   CONFIG REQUIRED)
find_package(glm    CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(Stb    REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h")
file(GLOB_RECURSE SHADERS CONFIGURE_DEPENDS "shaders/*.vert" "shaders/*.frag")

add_executable(LearnOpenGL ${SOURCES} ${HEADERS} ${SHADERS})

source_group(TREE "${CMAKE_SOURCE_DIR}/src"     PREFIX "Source Files" FILES ${SOURCES} ${HEADERS})
source_group(TREE "${CMAKE_SOURCE_DIR}/shaders" PREFIX "Shader Files" FILES ${SHADERS})

target_include_directories(LearnOpenGL PRIVATE
    src
    ${Stb_INCLUDE_DIR}
)

if(MSVC)
    target_compile_options(LearnOpenGL PRIVATE
        /W4
        /utf-8
        /permissive-
        /external:W0
        $<$<CONFIG:Release>:/Gy /Oi>
        /WX
        /w44365
        /sdl
    )
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT _ipo_supported)
if(_ipo_supported)
    set_property(TARGET LearnOpenGL PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

get_filename_component(_compiler_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
find_program(CLANG_TIDY_EXE
    NAMES clang-tidy
    HINTS "${_compiler_dir}"
)
if(CLANG_TIDY_EXE)
    if(ENABLE_CLANG_TIDY_ON_BUILD)
        set_target_properties(LearnOpenGL PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-quiet;--extra-arg=/EHsc;--extra-arg=-Wno-everything;--extra-arg=-Wno-error=unused-function;--extra-arg=-Wno-error=unused-command-line-argument"
        )
    endif()

    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXE}
            -quiet
            -p "${CMAKE_BINARY_DIR}"
            --extra-arg=-w
            --extra-arg=-Wno-everything
            --extra-arg=-Wno-error=unused-function
            --extra-arg=-Wno-error=unused-command-line-argument
            ${SOURCES}
        COMMENT "Running clang-tidy"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

add_custom_command(TARGET LearnOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:LearnOpenGL>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:LearnOpenGL>/resources"
    COMMENT "Copying runtime assets"
)

target_link_libraries(LearnOpenGL PRIVATE
    glfw
    glad::glad
    glm::glm
    assimp::assimp
)
