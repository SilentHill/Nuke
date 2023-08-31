
message("【Nuke.Test】配置开始")

# 配置cmake版本
cmake_minimum_required (VERSION 3.16)

# Nuke预配置
include ("../Nuke.Cpp.cmake")

#enable_testing()
# find_package(GTest REQUIRED)

# 编译文件
add_executable (Nuke.Test
"_pch.h"
"Tests/DateTimeTest.h"
"Tests/TimerTest.h"
"Tests/StringTest.h"

"Nuke.Test.cpp" 
)

# 依赖链接
add_subdirectory("googletest")
include_directories("googletest/googletest/include")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
target_precompile_headers(Nuke.Test PRIVATE _pch.h)
target_link_libraries(Nuke.Test "Nuke.System" "gtest")

message(NOTICE "【Nuke.Test】配置结束")
