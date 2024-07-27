project(cjson)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    cJSON.c
)

target_sources(${PROJECT_NAME} PRIVATE
    .
)