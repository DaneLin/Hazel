#pragma once

#ifdef HZ_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif // HZ_BUILD_DLL
#else
#define HAZEL_API
#endif
#else
	#error Hazel only support Windows!

#endif // HZ_PLATFORM_WINDOWS

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if (!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define HZ_CORE_ASSERT(x, ...) { if (!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif // HZ_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

/*
使用std::bind()需要添加#include<functional>头文件
std::bind()接受一个可调用对象以及参数(如果有的话)，形成一个新的可调用对象
可调用对象：
	普通函数体，Lambda表达式，类成员函数，静态成员函数，仿函数等

std::placeholders代表占位符，表示在函数参数中占据的参数位置
std::placeholders::_1中的_1表示新的可调用对象中的第一个参数
*/
