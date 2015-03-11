#pragma once
#define FCHART_COMPILE_PLATFORM_WIN_D2D1	1

#if _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT 
#endif