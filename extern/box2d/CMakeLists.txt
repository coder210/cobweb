set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#设置包含的目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB BOX2D_HEADER_FILES "*.h" "./include/*.h")

file(GLOB BOX2D_SOURCE_FILES "*.cpp" "./src/*/*.cpp")


add_library(box2d STATIC ${BOX2D_HEADER_FILES} ${BOX2D_SOURCE_FILES})

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${BOX2D_SOURCE_FILES} ${BOX2D_HEADER_FILES})