#pragma once

#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdfloat>

#include <algorithm>
#include <array>
#include <bitset>
#include <charconv>
#include <chrono>
#include <concepts>
#include <execution>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <future>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <numbers>
#include <numeric>
#include <optional>
#include <print>
#include <random>
#include <ranges>
#include <source_location>
#include <span>
#include <string>
#include <string_view>
#include <sstream>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "fmt/ranges.h"
#include "fmt/compile.h"
#pragma comment(lib, "fmt.lib")

#include "glm/glm.hpp"

#include "toml.hpp"

#include "magic_enum.hpp"
using namespace magic_enum::bitwise_operators;

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")

#include <windows.h>
#include <tlhelp32.h>
#include <fcntl.h>
#include <io.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#include "detours.h"
#pragma comment(lib, "detours.lib")

#include "eq_macros.h"
