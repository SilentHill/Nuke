
message(NOTICE "【Nuke.System】配置开始")

# 配置cmake版本
cmake_minimum_required (VERSION 3.8)

# Nuke预配置
include ("../Nuke.Cpp.cmake")

add_library(Nuke.System STATIC 
"_pch.h"

"Nuke.System.cpp" "Nuke.System.h"
"CrossPlatform/FileSystemApi.h" "CrossPlatform/FileSystemApi.cpp"
"CrossPlatform/SocketApiTypes.h"
"CrossPlatform/SocketApi.h" "CrossPlatform/SocketApi.cpp"
"CrossPlatform/ProcessApi.h" 
"CrossPlatform/ProcessApi.Windows.cpp"
"CrossPlatform/ProcessApi.Linux.cpp"
"CrossPlatform/ProcessApi.MacOS.cpp"
"CrossPlatform/NetworkApi.h" "CrossPlatform/NetworkApi.cpp"
"CrossPlatform/DnsApi.h" "CrossPlatform/DnsApi.cpp"
"CrossPlatform/TimeZoneApi.h" 
"CrossPlatform/TimeZoneApi.Windows.cpp"
"CrossPlatform/TimeZoneApi.Linux.cpp"
"CrossPlatform/TimeZoneApi.MacOS.cpp"
"CrossPlatform/MiscApi.h" "CrossPlatform/MiscApi.cpp"

"System/Event.h"
"System/Property.h"
"System/BitConverter.h" "System/BitConverter.cpp"
"System/Numerics/BitOperations.h" "System/Numerics/BitOperations.cpp"

"System/String.h" "System/String.cpp"
"System/Version.h" "System/Version.cpp"
"System/SafeHandle.h" "System/SafeHandle.cpp"
"System/OperatingSystem.h" "System/OperatingSystem.cpp"

"System/Generate.h" "System/Generate.cpp" 
"System/Math.h" "System/Math.cpp" 
"System/DateTime.h" "System/DateTime.cpp" 
"System/TimeZoneInfo.h"  "System/TimeZoneInfo.cpp" 
"System/TimeSpan.h"  "System/TimeSpan.cpp" 
"System/Guid.cpp" "System/Guid.h" 
"System/Environment.cpp" "System/Environment.h" 


"System/Buffers/Binary/BinaryPrimitives.h" "System/Buffers/Binary/BinaryPrimitives.cpp"

"System/Diagnostics/Stopwatch.h" "System/Diagnostics/Stopwatch.cpp"
"System/Diagnostics/Process.h" "System/Diagnostics/Process.cpp"
"System/Diagnostics/Metrics/Instrument.h" "System/Diagnostics/Metrics/Instrument.cpp"
"System/Diagnostics/Metrics/Meter.h" "System/Diagnostics/Metrics/Meter.cpp"

"System/Collections/Concurrent/ConcurrentQueue.h"


"System/IO/Path.h" "System/IO/Path.cpp"
"System/IO/File.h" "System/IO/File.cpp"
"System/IO/Directory.h" "System/IO/Directory.cpp"
"System/IO/FileSystemEntry.h" "System/IO/FileSystemEntry.cpp"
"System/IO/FileSystemInfo.h" "System/IO/FileSystemInfo.cpp"
"System/IO/FileInfo.h" "System/IO/FileInfo.cpp"
"System/IO/DirectoryInfo.h" "System/IO/DirectoryInfo.cpp"

"System/Data/DataTable.h" "System/Data/DataTable.cpp"

"System/Timers/Timer.h" "System/Timers/Timer.cpp"

"System/Net/Sockets/AddressFamily.h"
"System/Net/IPAddress.h" "System/Net/IPAddress.cpp"
"System/Net/SocketAddress.h" "System/Net/SocketAddress.cpp"
"System/Net/EndPoint.h" "System/Net/EndPoint.cpp"
"System/Net/IPEndPoint.h" "System/Net/IPEndPoint.cpp"
"System/Net/DnsEndPoint.h" "System/Net/DnsEndPoint.cpp"
"System/Net/Dns.h" "System/Net/Dns.cpp"
"System/Net/NetworkInformation/NetworkInterfaceType.h"
"System/Net/NetworkInformation/IPInterfaceProperties.h" "System/Net/NetworkInformation/IPInterfaceProperties.cpp"
"System/Net/NetworkInformation/NetworkInterface.h" "System/Net/NetworkInformation/NetworkInterface.cpp"
"System/Net/Sockets/TcpClient.h" "System/Net/Sockets/TcpClient.cpp"

"System/Threading/Tasks/Task.h" "System/Threading/Tasks/Task.cpp"
"System/Threading/CancellationToken.h" "System/Threading/CancellationToken.cpp"

"System/ServiceModel/ICommunicationObject.h"
"System/ServiceModel/CommunicationState.h"
)

target_precompile_headers(Nuke.System PRIVATE _pch.h)
target_include_directories(Nuke.System PUBLIC "")

# MSQUIC
set(MSQUIC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR})
if (CMAKE_SYSTEM_NAME MATCHES "Linux")

elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "/EHsc")
    target_link_libraries(Nuke.System "Ws2_32.lib")
    target_link_libraries(Nuke.System "Mswsock.lib")
    target_link_libraries(Nuke.System "AdvApi32.lib")
endif()

message(NOTICE "【Nuke.System】配置结束")