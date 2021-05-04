#pragma once
#include <memory>

//TODO: add support for mac and linux!
#ifdef COCO_PLATFORM_WINDOWS
	#if COCO_DLL
		#ifdef COCO_BUILD_DLL
			#define COCO_API __declspec(dllexport)
		#else
			#define COCO_API __declspec(dllimport)
		#endif
	#else
		#define COCO_API
	#endif
#endif

#if COCO_ASSERTS
	#define ASSERT(x, ...) {if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define ASSERT_CORE(x, ...) {if(!(x)) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define ASSERT(x, ...)
	#define ASSERT_CORE(x, ...)
#endif

namespace Coco
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

	template<typename T>
	using WeakRef = std::weak_ptr<T>;
}