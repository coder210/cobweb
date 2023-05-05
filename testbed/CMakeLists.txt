file(GLOB TESTBED_HEADER_FILES "*.h" "./*.h")

file(GLOB TESTBED_SOURCE_FILES "*.cpp" "./*.cpp")

add_executable(testbed ${TESTBED_HEADER_FILES} ${TESTBED_SOURCE_FILES})

target_link_libraries(testbed box2d)
target_link_libraries(testbed glad)
target_link_libraries(testbed glfw)
target_link_libraries(testbed imgui)

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TESTBED_SOURCE_FILES} ${TESTBED_HEADER_FILES})
