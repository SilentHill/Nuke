
message("【Nuke】配置开始")

# 配置cmake版本
cmake_minimum_required (VERSION 3.8)
project ("Nuke")

# 公共配置
include ("Nuke.Cpp.cmake")

# Nuke成员
add_subdirectory("Nuke.System")
add_subdirectory("Nuke.Test")

message("【Nuke】配置结束")
