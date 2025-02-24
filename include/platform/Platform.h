/*
**
**	Platform.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once


/**
 !	Common includes
 */
#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif

#include <cassert>
#include <numbers>
#include <numeric>
#include <variant>
#include <typeindex>

#include <set>
#include <map>
#include <array>
#include <stack>
#include <queue>
#include <unordered_set>

#include <future>
#include <semaphore>
#include <shared_mutex>

#include <regex>
#include <fstream>
#include <filesystem>

#include <source_location>


/**
 !	using namespace
 */
using namespace std::chrono_literals;

/**
 !	windows
 */
#if defined( _WIN32 ) || defined( _WIN64 )
	#include "windows/Win_Platform.h"
/**
 !	android
 */
#elif defined( ANDROID )
	#include "android/Android_Platform.h"
/**
 !	linux
 */
#elif defined( LINUX )
	#include "linux/Linux_Platform.h"
/**
 !	APPLE
 */
#elif defined( __APPLE__ )
	#include <TargetConditionals.h>
	/**
	 !	ios
	 */
	#if defined( TARGET_OS_IPHONE )
		#include "ios/Ios_Platform.h"
	/**
	 !	mac
	 */
	#elif defined( TARGET_OS_MAC )
		#include "mac/Mac_Platform.h"
	 /**
	  !	unknown
	  */
	#else
		#error Unsupported apple platform
	#endif
 /**
  !	unknown
  */
#else
	#error Unsupported platform
#endif


/**
 !	Cross graphics library
 */
#ifdef FC_VULKAN
	#include "../graphics/vulkan/vk.h"
#elif defined  FC_DIRECTX
	#include "../graphics/directx/dx.h"
#elif defined  FC_METAL
	#include "../graphics/metal/mt.h"
#elif defined  FC_GLES
	#include "../graphics/gles/es.h"
#endif


/**
 !	MSVC
 */
#if _MSC_VER
	#define FC_INLINE	__forceinline
/**
 !	MINGW
 */
#elif __MINGW32__ || __MINGW64__
	#define FC_INLINE	__attribute__((always_inline)) 
/**
 !	GCC
 */
#elif __GNUC__
	#define FC_INLINE	__attribute__((always_inline)) 
/**
 !	Default
 */
#else
	#define FC_INLINE	inline
#endif


/**
 !	FC_RELEASE
 */
#ifndef FC_DEBUG
	#define FC_NDEBUG

	#ifndef FC_SECURITY
		#define FC_RELEASE
	#endif
#endif


/**
 !	Export or Import
 */
#ifdef FC_EXPORTS
	#define FC_API	FC_EXPORT
#elif defined FC_IMPORTS
	#define FC_API	FC_IMPORT
#else
	#define FC_API
#endif


/**
 !	noexcept
 */
#ifdef FC_RELEASE
	#define NOEXCEPT	noexcept
#else
	#define NOEXCEPT
#endif


/**
 !	constexpr
 */
#ifdef FC_RELEASE
	#define CONSTEXPR	constexpr
#else
	#define CONSTEXPR
#endif



/**
 !	fce name
 */
#define FCE_NAME TXT( "fce" )