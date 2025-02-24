/*
**
**	VULKAN.
**
		Copyright (c) WangFeng since 2020...
*/

#ifdef FC_WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR
#elif defined  FC_ANDROID
	#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined  FC_LINUX
	#define VK_USE_PLATFORM_XLIB_KHR
#elif defined  FC_IOS
	#define VK_USE_PLATFORM_IOS_MVK
#elif defined  FC_MAC
	#define VK_USE_PLATFORM_MACOS_MVK
#endif


/**
 !	VULKAN is dynamically loaded by Volk
 */
#ifndef FC_DEBUG
	#define VK_NO_PROTOTYPES
#endif

#ifdef VK_NO_PROTOTYPES
	#include <volk/volk.h>
#else
	#include <vulkan/vulkan.h>
#endif


/**
 !	VMA is indirectly dynamically loaded by Volk

		#define VMA_VULKAN_VERSION 1001000
 */
#define VMA_STATIC_VULKAN_FUNCTIONS 1
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0

#include <vma/vk_mem_alloc.h>


/**
 !	Predefine
 */
namespace fce::inner::vulkan
{
	class VK_Graphics;
	class VK_Instance;
	class VK_Device;
	class VK_Command;
	class VK_SwapChain;
	class VK_WindowSurface;
	class VK_PipelineLayout;
	class VK_RenderPipeline;

	class VK_Texture2;
	class VK_RenderPass;
	class VK_FrameBuffer;
	class VK_IndexBuffer;
	class VK_VertexBuffer;
}


/*
 @	Vulkan error return
 */
#define VK_ERR_RETURN( expr )	res = expr;								\
																		\
								if( res != VK_SUCCESS )					\
									{									\
										return res;						\
									}


/*
 @	Vulkan error guard
 */
#define VK_ERR_GUARD( expr )	do {									\
									res = expr;							\
																		\
									if( res == VK_SUCCESS )				\
										{								\
											break;						\
										}								\
								}										\
								while( s_VK_Graphics.on_Error( res ) )