
message("【Nuke】配置开始")

# 配置cmake版本
cmake_minimum_required (VERSION 3.8)
project ("Nuke")

# 公共配置
include ("Source/Nuke.Cpp.cmake")

# gtest
add_subdirectory("Source/SubModules/googletest")

# Nuke成员
add_subdirectory("Source/Nuke.System")
add_subdirectory("Source/Nuke.Test")

message("【Nuke】配置结束")
