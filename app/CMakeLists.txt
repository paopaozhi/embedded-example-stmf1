project(app)

add_library(${PROJECT_NAME} INTERFACE)

enable_language(C ASM)

target_include_directories(${PROJECT_NAME} INTERFACE
    inc
)

file(GLOB_RECURSE SOURCES_APP "src/*.*")

target_sources(${PROJECT_NAME} INTERFACE
    ${SOURCES_APP}
    startup_stm32f103xb.s
    # src/app.c
)