file(GLOB MYLIB_HEADER_FILES "./*.h")

file(GLOB MYLIB_SOURCE_FILES "./*.cpp")


add_library(mylib STATIC ${MYLIB_HEADER_FILES} ${MYLIB_SOURCE_FILES})
target_link_libraries(mylib box2d glad glfw imgui)

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MYLIB_SOURCE_FILES} ${MYLIB_HEADER_FILES})
