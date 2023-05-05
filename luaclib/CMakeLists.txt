set(LUA_COBWEB_NAME lcobweb)
set(LUA_UDP_NAME ludp)
set(LUA_TCP_NAME ltcp)
set(LUA_NETPACK_NAME lnetpack)
set(LUA_CRYPT_NAME lcrypt)
set(LUA_MYSQL_NAME lmysql)
set(LUA_BOX2D_NAME lbox2d)

file(GLOB LUA_COBWEB_SOURCE_FILES "${LUA_COBWEB_NAME}.cpp")
file(GLOB LUA_UDP_SOURCE_FILES "${LUA_UDP_NAME}.cpp")
file(GLOB LUA_TCP_SOURCE_FILES "${LUA_TCP_NAME}.cpp")
file(GLOB LUA_NETPACK_SOURCE_FILES "${LUA_NETPACK_NAME}.cpp")
file(GLOB LUA_CRYPT_SOURCE_FILES "${LUA_CRYPT_NAME}.cpp")
file(GLOB LUA_MYSQL_SOURCE_FILES "${LUA_MYSQL_NAME}.cpp")
file(GLOB LUA_BOX2D_SOURCE_FILES "${LUA_BOX2D_NAME}.cpp")


add_library(${LUA_COBWEB_NAME} SHARED ${LUA_COBWEB_SOURCE_FILES})
add_library(${LUA_UDP_NAME} SHARED ${LUA_UDP_SOURCE_FILES})
add_library(${LUA_TCP_NAME} SHARED ${LUA_TCP_SOURCE_FILES})
add_library(${LUA_NETPACK_NAME} SHARED ${LUA_NETPACK_SOURCE_FILES})
add_library(${LUA_CRYPT_NAME} SHARED ${LUA_CRYPT_SOURCE_FILES})
add_library(${LUA_MYSQL_NAME} SHARED ${LUA_MYSQL_SOURCE_FILES})
add_library(${LUA_BOX2D_NAME} SHARED ${LUA_BOX2D_SOURCE_FILES})


target_link_libraries(${LUA_COBWEB_NAME} lua)
target_link_libraries(${LUA_UDP_NAME} lua platform)
target_link_libraries(${LUA_TCP_NAME} lua platform)
target_link_libraries(${LUA_NETPACK_NAME} lua)
target_link_libraries(${LUA_CRYPT_NAME} lua crypt)
target_link_libraries(${LUA_MYSQL_NAME} lua libmysql)
target_link_libraries(${LUA_BOX2D_NAME} lua box2d)



