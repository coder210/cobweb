set(LUA_COBWEB_NAME lcobweb)
set(LUA_NETWORK_NAME lnetwork)
set(LUA_CRYPT_NAME lcrypt)
set(LUA_MYSQL_NAME lmysql)
set(LUA_BOX2D_NAME lbox2d)
set(LUA_EVENT_NAME lev)
#set(LUA_UV_NAME luv)

file(GLOB LUA_COBWEB_SOURCE_FILES "${LUA_COBWEB_NAME}.cpp")
file(GLOB LUA_NETWORK_SOURCE_FILES "${LUA_NETWORK_NAME}.cpp")
#file(GLOB LUA_CRYPT_SOURCE_FILES "${LUA_CRYPT_NAME}.cpp")
#file(GLOB LUA_MYSQL_SOURCE_FILES "${LUA_MYSQL_NAME}.cpp")
file(GLOB LUA_BOX2D_SOURCE_FILES "${LUA_BOX2D_NAME}.cpp")
file(GLOB LUA_EVENT_SOURCE_FILES "${LUA_EVENT_NAME}.cpp")
#file(GLOB LUA_UV_SOURCE_FILES "${LUA_UV_NAME}.cpp")


add_library(${LUA_COBWEB_NAME} SHARED ${LUA_COBWEB_SOURCE_FILES})
add_library(${LUA_NETWORK_NAME} SHARED ${LUA_NETWORK_SOURCE_FILES})
#add_library(${LUA_CRYPT_NAME} SHARED ${LUA_CRYPT_SOURCE_FILES})
#add_library(${LUA_MYSQL_NAME} SHARED ${LUA_MYSQL_SOURCE_FILES})
add_library(${LUA_BOX2D_NAME} SHARED ${LUA_BOX2D_SOURCE_FILES})
add_library(${LUA_EVENT_NAME} SHARED ${LUA_EVENT_SOURCE_FILES})
#add_library(${LUA_UV_NAME} SHARED ${LUA_UV_SOURCE_FILES})


target_link_libraries(${LUA_COBWEB_NAME} lua)
target_link_libraries(${LUA_NETWORK_NAME} lua mylib)
#target_link_libraries(${LUA_CRYPT_NAME} lua crypt)
#target_link_libraries(${LUA_MYSQL_NAME} lua libmysql)
target_link_libraries(${LUA_BOX2D_NAME} lua box2d mylib)



if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_libraries(${LUA_EVENT_NAME} lua mylib libevent libevent_core libevent_extra)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_libraries(${LUA_EVENT_NAME} lua mylib ws2_32 event event_core event_extra)
	#target_link_libraries(${LUA_UV_NAME} lua libuv ws2_32 Dbghelp Iphlpapi Userenv)
else()
	message("current Platform: unkonw")
endif()




