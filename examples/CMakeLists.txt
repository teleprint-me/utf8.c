# @file utf8/examples/CMakeLists.txt

set(EXAMPLES
    "append"
)

set(INPUT_DIR ${PROJECT_SOURCE_DIR}/examples)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/examples)

foreach(test IN LISTS EXAMPLES)
    add_executable(${test} ${INPUT_DIR}/${test}.c)
    target_link_libraries(${test} utf8)
    target_include_directories(${test} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
    add_custom_target("run_${test}" COMMAND ${test} DEPENDS ${test} COMMENT "Running tests for ${test}")
    add_test(NAME ${test} COMMAND ${test} WORKING_DIRECTORY ${OUTPUT_DIR})
endforeach()
