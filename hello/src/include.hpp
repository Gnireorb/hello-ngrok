#pragma once

#include <Windows.h>
#include <future>
#include <string>
#include <iostream>
#include <fstream>
#include <tchar.h>

#define FMT_HEADER_ONLY
#include "../../dependencies/fmtlib/include/fmt/core.h"
#include "../../dependencies/fmtlib/include/fmt/chrono.h"
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_dx9.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"
#include "../../dependencies/imgui/imgui_rubik.h"
#include "../../dependencies/rapidjson/document.h"
#include "../../dependencies/rapidjson/stringbuffer.h"
#include "../../dependencies/rapidjson/writer.h"

using namespace rapidjson;
