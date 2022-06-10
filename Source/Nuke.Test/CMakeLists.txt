
message("【Nuke.Test】配置开始")

# 配置cmake版本
cmake_minimum_required (VERSION 3.8)

# Nuke预配置
include ("../Nuke.Cpp.cmake")

# 编译文件
add_executable (Nuke.Test
"_pch.h"
"Tests/DateTimeTest.h"
"Tests/TimerTest.h"


"Nuke.Test.cpp" 
)

# 依赖链接
target_link_libraries(Nuke.Test "Nuke.System" "gtest" "Ths.DomainResolver")
target_precompile_headers(Nuke.Test PRIVATE _pch.h)

message(NOTICE "【Nuke.Test】配置结束")