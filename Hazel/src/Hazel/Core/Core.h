#pragma once

#include <memory>

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

#ifdef HZ_DISABLE_ASSERTS
#define HZ_ASSERT(x, ...)
#define HZ_CORE_ASSERT(x, ...)
#else
#define HZ_ASSERT(x, ...) { if (!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define HZ_CORE_ASSERT(x, ...) { if (!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#endif // HZ_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

/*
ʹ��std::bind()��Ҫ����#include<functional>ͷ�ļ�
std::bind()����һ���ɵ��ö����Լ�����(����еĻ�)���γ�һ���µĿɵ��ö���
�ɵ��ö���
	��ͨ�����壬Lambda����ʽ�����Ա��������̬��Ա�������º�����

std::placeholders����ռλ������ʾ�ں���������ռ�ݵĲ���λ��
std::placeholders::_1�е�_1��ʾ�µĿɵ��ö����еĵ�һ������
*/


namespace Hazel
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}