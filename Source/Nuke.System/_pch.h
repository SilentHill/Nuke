
#pragma once

// CRT
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstring>

// STL
#include <string>
#include <iostream>
#include <random>
#include <queue>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <chrono>
#include <future>
#include <bitset>
#include <regex>
#include <fstream>
#include <sstream>
#include <span>

#ifdef _MSC_VER 
    #define DEV_WARNING(warningMessage) __pragma(message(warningMessage))
#else 
    //#define DEV_WARNING(warningMessage) _Pragma("GCC warning " warningMessage )
    #define DEV_WARNING(warningMessage) 
#endif 
