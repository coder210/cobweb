file(GLOB CJSON_HEADER_FILES "./*.h")
file(GLOB CJSON_SOURCE_FILES "./*.c")



add_library(cjson SHARED ${CJSON_HEADER_FILES} ${CJSON_SOURCE_FILES})
target_link_libraries(cjson lua)

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CJSON_SOURCE_FILES} ${CJSON_HEADER_FILES})
