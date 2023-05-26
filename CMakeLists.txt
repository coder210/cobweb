cmake_minimum_required(VERSION 3.5)

#设置变量
set(COBWEB_NAME cobweb)
set(KCP_NAME kcp)
set(BOX2D_NAME box2d)
set(LUA_NAME lua)
set(SAJSON_NAME sajson)


project(${COBWEB_NAME} C CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

add_subdirectory("extern/kcp")
add_subdirectory("extern/lua")
add_subdirectory("extern/cjson")
add_subdirectory("extern/box2d")
add_subdirectory("extern/sajson")
add_subdirectory("extern/glad")
add_subdirectory("extern/glfw")
add_subdirectory("extern/imgui")
add_subdirectory("extern/crypt")
#add_subdirectory("extern/libuv")
add_subdirectory("extern/libevent")

#设置包含的目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/box2d/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory("testbed")


if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	#添加宏定义
 	add_definitions(-D_COBWEB_LINUX)

	#设置包含的目录
	include_directories(/usr/local/include)

	#设置链接库搜索目录
	link_directories(${CMAKE_CURRENT_SOURCE_DIR})
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
	link_directories(/usr/local/lib)

	IF(CMAKE_CL_64)
	    message("current Platform: Linux 64")
	ELSE(CMAKE_CL_32)
	    message("current Platform: Linux 32")
	ENDIF(CMAKE_CL_64)
 
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	#添加宏定义
 	add_definitions(-D_COBWEB_WIN32)
 	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
 	add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)

	#设置链接库搜索目录
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/Debug)
	IF(CMAKE_CL_64)
    	    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win64)
	    message("current platform: Windows 64")
	ELSE(CMAKE_CL_32)
    	    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win32)
	    message("current platform: Windows 32")
	ENDIF(CMAKE_CL_64)
else()
	message("current Platform: unkonw")
endif()


add_subdirectory("extern/mylib")
add_subdirectory("luaclib")
add_subdirectory("cservice")

file(GLOB COBWEB_HEADER_FILES "include/*.h")
file(GLOB COBWEB_SOURCE_FILES "src/*.cpp")

add_executable(${COBWEB_NAME} ${COBWEB_HEADER_FILES} ${COBWEB_SOURCE_FILES})

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_libraries(${COBWEB_NAME} ${KCP_NAME} ${LUA_NAME} dl pthread)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_libraries(${COBWEB_NAME} ${KCP_NAME} ${LUA_NAME})
else()
	message("current Platform: unkonw ")
endif()



