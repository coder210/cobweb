file(GLOB PB_HEADER_FILES "./*.h")

file(GLOB PB_SOURCE_FILES "./*.c")

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_definitions(-DLUA_BUILD_AS_DLL)
endif()

add_library(pb SHARED ${PB_HEADER_FILES} ${PB_SOURCE_FILES})

target_link_libraries(pb lua)
