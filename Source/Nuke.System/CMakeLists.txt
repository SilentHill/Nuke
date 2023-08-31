
message(NOTICE "【Nuke.System】配置开始")

# 配置cmake版本
cmake_minimum_required (VERSION 3.8)

# Nuke预配置
include ("../Nuke.Cpp.cmake")


add_library(Nuke.System STATIC ${SRC_H} ${SRC_HH} ${SRC_HPP} ${SRC_CPP} ${SRC_CC})

target_precompile_headers(Nuke.System PUBLIC "_pch.h")
target_include_directories(Nuke.System PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_SYSTEM_NAME MATCHES "Linux")

elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "/EHsc")
    target_link_libraries(Nuke.System "Ws2_32.lib")
    target_link_libraries(Nuke.System "Mswsock.lib")
endif()

message(NOTICE "【Nuke.System】配置结束")