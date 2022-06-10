
# 环境信息
message ("目标操作系统 - " ${CMAKE_SYSTEM_NAME})
message ("使用C++编译器 - " ${CMAKE_CXX_COMPILER_ID} " v" ${CMAKE_CXX_COMPILER_VERSION})

# 配置编译器参数
set(CMAKE_CXX_STANDARD 20)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wno-c++17-extensions)
    add_definitions(-Wno-deprecated-declarations)
    add_definitions(-Wno-c++98-compat)
    add_definitions(-Wno-microsoft-include)

    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
        #add_definitions(-fpermissive)
    elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
        add_compile_definitions(__STDC_NO_ATOMICS__)
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
        add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    endif ()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions(-fpermissive)
    add_definitions(-Wno-c++17-extensions)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
endif ()

message ("使用C++标准 - " "C++" ${CMAKE_CXX_STANDARD})


# Android编译环境变量
# ANDROID_HOME=C:\\Microsoft\AndroidSDK\25
# ANT_HOME=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Apps\apache-ant-1.9.3
# JAVA_HOME=C:\Program Files\Android\jdk\jdk-8.0.302.8-hotspot\jdk8u302-b08
# NDK_ROOT=C:\Microsoft\AndroidNDK\android-ndk-r21e