#pragma once

#include <Windows.h>
#include <array>
#include <string>
#include <cstdint>
#include <memory>
#include <span>
#include <unordered_map>
#include <vector>
#include <variant>

#define UNIMPLEMENTED_VOID(Name)                                             \
	void Name()                                                              \
	{                                                                        \
		MessageBoxA(nullptr, __FUNCTION__, "Unimplemented function", MB_OK); \
		__debugbreak();                                                      \
	}
