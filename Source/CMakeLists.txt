add_executable(${META_PROJECT_NAME})
display_header("Creating Makefile of ${META_PROJECT_NAME}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
target_sources(${META_PROJECT_NAME} PRIVATE ${SOURCES})
target_include_directories(${META_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(swe-solvers REQUIRED)
target_link_libraries(${META_PROJECT_NAME}
    PRIVATE
        project_options
        project_warnings
        SWE-Solvers
)

option(ENABLE_SINGLE_PRECISION "Enable single floating-point precision" OFF)
if(ENABLE_SINGLE_PRECISION)
    target_compile_definitions(${META_PROJECT_NAME} PRIVATE ENABLE_SINGLE_PRECISION)
endif()
