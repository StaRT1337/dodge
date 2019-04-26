#pragma once
#include <Windows.h>

#include <d2d1.h>
#include <dwrite.h>

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <filesystem>
#include <codecvt>
#include <fstream>
#include <ctime>
#include <any>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "fmt/core.h"
#include "fmt/format.h"

using namespace std::literals;

#define PRINT(__FMT__, ...) fmt::print("INFO: " __FMT__ "\n", __VA_ARGS__)
