add_library(${SWE_PROJECT_NAME} OBJECT)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*")
list(FILTER SOURCES EXCLUDE REGEX ".*Main\\.cpp$")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

target_sources(${SWE_PROJECT_NAME} PRIVATE ${SOURCES})

target_link_libraries(${SWE_PROJECT_NAME} PUBLIC SWE-Interface SWE-Solvers)
target_include_directories(${SWE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${SWE_PROJECT_NAME}-Runner Main.cpp)
target_link_libraries(${SWE_PROJECT_NAME}-Runner PRIVATE ${SWE_PROJECT_NAME})
