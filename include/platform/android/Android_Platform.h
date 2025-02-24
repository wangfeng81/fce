/*
**
**	Platform android.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once


/**
 !	Includes
 */
#include <jni.h>
#include <poll.h>
#include <sched.h>
#include <dlfcn.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include <android/looper.h>
#include <android/window.h>
#include <android/api-level.h>
#include <android/configuration.h>
#include <android/native_activity.h>


/**
 !	Platform
 */
#define  FC_ANDROID 1


/**
 !	64-bit
 */
#if defined( __x86_64__ )
	#define FC_64_BIT 64
/**
 !	32-bit
 */
#elif defined( __i386__ )
	#define FC_32_BIT 32
/**
 !	Unknown
 */
#else
	#error Unknown x-bit
#endif


/**
 !	Debug
 */
#ifdef _DEBUG
	#define FC_DEBUG
#endif


/**
 !	EXPORT and IMPORT
 */
#define FC_EXPORT	__attribute__ ( ( visibility( "default" ) ) )
#define FC_IMPORT


/**
 !	Graphics library
 */
#define FC_VULKAN 1


/*
 !	Predefine
 */
namespace fce::inner::android
{
	class Android_Application;
}