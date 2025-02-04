cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
enable_language(C ASM)

project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

add_executable(${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES "app/src/*.*")
add_subdirectory(app)

target_link_libraries(${CMAKE_PROJECT_NAME}
    app
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    app/inc
)

include(.empm/empm_import.cmake)