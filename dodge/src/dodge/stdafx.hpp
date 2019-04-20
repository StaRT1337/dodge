#pragma once
#include <Windows.h>

#include <d2d1.h>
#include <dwrite.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <codecvt>
#include <memory>
#include <functional>
#include <fstream>
#include <filesystem>
#include <random>

#include "fmt/core.h"
#include "fmt/format.h"

using namespace std::literals;

#define PRINT(__FMT__, ...) fmt::print("INFO: " __FMT__ "\n", __VA_ARGS__)
