project(app)

add_library(${PROJECT_NAME} INTERFACE)

enable_language(C ASM)

target_include_directories(${PROJECT_NAME} INTERFACE
    inc
    # ${CMAKE_SOURCE_DIR}/drivers/led_smg
)

file(GLOB_RECURSE SOURCES_APP "src/*.*")

target_sources(${PROJECT_NAME} INTERFACE
    # ${SOURCES_APP}
    src/app.c
    src/ledsmg.c
    src/rtc.c
)