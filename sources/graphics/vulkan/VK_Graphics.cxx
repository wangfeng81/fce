/*
**
**	VK_Graphics
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

#ifdef VK_NO_PROTOTYPES
	#include <volk/volk.c>
#elif defined FC_WINDOWS
	#pragma comment( lib, "vulkan-1.lib" )
#endif

#include "VK_Allocator.cxx"
#include "VK_Instance.cxx"
#include "VK_PhysicalDevice.cxx"
#include "VK_Device.cxx"
#include "VK_Command.cxx"
#include "VK_TempBuffer.cxx"


namespace fce::inner::vulkan
{
	/*
	 @	创建
	 */
	FC_INLINE VkResult VK_Graphics::Create() noexcept
		{
			VkResult res;

			// 初始化 dynamic loader
			#ifdef VK_NO_PROTOTYPES

				VK_ERR_RETURN( volkInitialize() );

			#endif

			// 创建 instance
			res = this->instance.Create();

			if( res == VK_SUCCESS )
				{
					// 初始化 physical device
					res = this->phyDevice.Initialize();

					if( res == VK_SUCCESS )
						{
							// 创建 logical device
							res = this->device.Create();

							if( res == VK_SUCCESS )
								{
									// 创建 command
									res = this->command.Create();

									if( res == VK_SUCCESS )
										{
											// 创建 allocator
											res = this->allocator.Create();
										}
								}
						}
				}

			return res;
		}


	/*
	 @	清空 资源
	 */
	FC_INLINE void VK_Graphics::Clear() noexcept
		{
			// 析构 资源 池
			VK_PipelineLayout::DestructAll();
		}


	/*
	 @	销毁
	 */
	FC_INLINE void VK_Graphics::Destroy() noexcept
		{
			// 销毁 allocator
			this->allocator.Destroy();

			// 销毁 command
			this->command.Destroy();

			// 销毁 device
			this->device.Destroy();

			// 销毁 instance
			this->instance.Destroy();
		}


	/*
	 @	处理 错误
	 */
	bool VK_Graphics::on_Error( const VkResult res ) noexcept
		{
			// 处理 错误
			switch( res )
				{
					// 正确
					case VK_SUCCESS:
						{
							return true;
						}

					// 重建 surface，同时重建 swap chain
					case VK_ERROR_SURFACE_LOST_KHR:
					// 重建 swap chain
					case VK_SUBOPTIMAL_KHR:
					case VK_ERROR_OUT_OF_DATE_KHR:
					case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
						{
							return s_Window.Rebuild();
						}

					// 丢失 device
					case VK_ERROR_DEVICE_LOST:
						{
							return s_VK_Device.on_Lost();
						}

					// 清理 设备内存
					case VK_ERROR_OUT_OF_DEVICE_MEMORY:
						{
							return true;
						}

					// 清理 主机内存
					case VK_ERROR_OUT_OF_HOST_MEMORY:
					case VK_ERROR_FRAGMENTED_POOL:
						{
							return s_MemMgmt.Cleanup();
						}

					// 默认 处理
					default:
						{
						}
				}

			return false;
		}
}