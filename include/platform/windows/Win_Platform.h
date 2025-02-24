/*
**
**	Platform windows.
**
		Copyright (c) WangFeng since 1999...
*/

/**
 !	Includes
 */
#ifndef NOMINMAX
	#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <windowsx.h>
#include <shellapi.h>

#if WINVER >= _WIN32_WINNT_VISTA
	#include <dwmapi.h>
	#pragma comment( lib, "Dwmapi.lib" )
#endif


/**
 !	Platform
 */
#define FC_WINDOWS 1


/**
 !	64-bit
 */
#if defined( _WIN64 )
	#define FC_64_BIT 64
/**
 !	32-bit
 */
#elif defined( _WIN32 )
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
#define FC_EXPORT __declspec( dllexport )
#define FC_IMPORT __declspec( dllimport )


/**
 !	Graphics library
 */
#define FC_VULKAN 1


/*
 !	Predefine
 */
namespace fce::inner::win
{
	class Win_Window;
	class Win_Clipboard;
	class Win_Application;
}