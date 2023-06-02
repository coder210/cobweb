set(LOGGER_NAME logger)
set(KCPSERVER_NAME kcpserver)
set(TCPSERVER_NAME tcpserver)
set(ECHOSERVER_NAME echoserver)
set(EVSERVER_NAME evserver)
set(LUAX_NAME luax)


if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

file(GLOB LOGGER_SOURCE_FILES "${LOGGER_NAME}.cpp")
file(GLOB KCPSERVER_SOURCE_FILES "${KCPSERVER_NAME}.cpp")
file(GLOB TCPSERVER_SOURCE_FILES "${TCPSERVER_NAME}.cpp")
file(GLOB ECHOSERVER_SOURCE_FILES "${ECHOSERVER_NAME}.cpp")
file(GLOB EVSERVER_SOURCE_FILES "${EVSERVER_NAME}.cpp")
file(GLOB LUAX_SOURCE_FILES "luax.cpp")

add_library(${LOGGER_NAME} SHARED ${LOGGER_SOURCE_FILES})
add_library(${KCPSERVER_NAME} SHARED ${KCPSERVER_SOURCE_FILES})
add_library(${TCPSERVER_NAME} SHARED ${TCPSERVER_SOURCE_FILES})
add_library(${ECHOSERVER_NAME} SHARED ${ECHOSERVER_SOURCE_FILES})
add_library(${EVSERVER_NAME} SHARED ${EVSERVER_SOURCE_FILES})
add_library(${LUAX_NAME} SHARED ${LUAX_SOURCE_FILES})

target_link_libraries(${KCPSERVER_NAME} kcp mylib)
target_link_libraries(${TCPSERVER_NAME} mylib)
target_link_libraries(${ECHOSERVER_NAME} mylib)
target_link_libraries(${LUAX_NAME} lua)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_libraries(${EVSERVER_NAME} mylib libevent libevent_core libevent_extra)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_libraries(${EVSERVER_NAME} mylib ws2_32 event event_core event_extra)
	message("current Platform: unkonw")
endif()


