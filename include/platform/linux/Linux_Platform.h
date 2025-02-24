/*
**
**	Platform linux.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once


/**
 !	Platform
 */
#define  FC_LINUX 1


/**
 !	64-bit or 32-bit
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
