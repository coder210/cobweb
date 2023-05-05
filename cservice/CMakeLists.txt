set(GATE_NAME gate)
set(HARBOR_NAME harbor)
set(LOGGER_NAME logger)
set(LUAX_NAME luax)

file(GLOB GATE_SOURCE_FILES "gate.cpp")
file(GLOB HARBOR_SOURCE_FILES "harbor.cpp")
file(GLOB LOGGER_SOURCE_FILES "logger.cpp")
file(GLOB LUAX_SOURCE_FILES "luax.cpp")


add_library(${GATE_NAME} SHARED ${GATE_SOURCE_FILES})
add_library(${HARBOR_NAME} SHARED ${HARBOR_SOURCE_FILES})
add_library(${LOGGER_NAME} SHARED ${LOGGER_SOURCE_FILES})
add_library(${LUAX_NAME} SHARED ${LUAX_SOURCE_FILES})


target_link_libraries(${GATE_NAME} platform)
target_link_libraries(${HARBOR_NAME} platform)
target_link_libraries(${LOGGER_NAME} platform)
target_link_libraries(${LUAX_NAME} platform lua)


