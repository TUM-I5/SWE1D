add_library(${META_PROJECT_NAME} OBJECT)
display_header("Creating Makefile of ${META_PROJECT_NAME}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*")
list(FILTER SOURCES EXCLUDE REGEX ".*Main\\.cpp$")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
target_sources(${META_PROJECT_NAME} PRIVATE ${SOURCES})
target_include_directories(${META_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SWE-Solvers REQUIRED)
target_link_libraries(${META_PROJECT_NAME}
    PUBLIC
        project_options
        project_warnings
        SWE-Solvers
)

option(ENABLE_SINGLE_PRECISION "Enable single floating-point precision" OFF)
if(ENABLE_SINGLE_PRECISION)
    target_compile_definitions(${META_PROJECT_NAME} PUBLIC ENABLE_SINGLE_PRECISION)
endif()

add_executable(${META_PROJECT_NAME}-Runner Main.cpp)
target_link_libraries(${META_PROJECT_NAME}-Runner PRIVATE ${META_PROJECT_NAME})
