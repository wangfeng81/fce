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
	 @	����
	 */
	FC_INLINE VkResult VK_Graphics::Create() noexcept
		{
			VkResult res;

			// ��ʼ�� dynamic loader
			#ifdef VK_NO_PROTOTYPES

				VK_ERR_RETURN( volkInitialize() );

			#endif

			// ���� instance
			res = this->instance.Create();

			if( res == VK_SUCCESS )
				{
					// ��ʼ�� physical device
					res = this->phyDevice.Initialize();

					if( res == VK_SUCCESS )
						{
							// ���� logical device
							res = this->device.Create();

							if( res == VK_SUCCESS )
								{
									// ���� command
									res = this->command.Create();

									if( res == VK_SUCCESS )
										{
											// ���� allocator
											res = this->allocator.Create();
										}
								}
						}
				}

			return res;
		}


	/*
	 @	��� ��Դ
	 */
	FC_INLINE void VK_Graphics::Clear() noexcept
		{
			// ���� ��Դ ��
			VK_PipelineLayout::DestructAll();
		}


	/*
	 @	����
	 */
	FC_INLINE void VK_Graphics::Destroy() noexcept
		{
			// ���� allocator
			this->allocator.Destroy();

			// ���� command
			this->command.Destroy();

			// ���� device
			this->device.Destroy();

			// ���� instance
			this->instance.Destroy();
		}


	/*
	 @	���� ����
	 */
	bool VK_Graphics::on_Error( const VkResult res ) noexcept
		{
			// ���� ����
			switch( res )
				{
					// ��ȷ
					case VK_SUCCESS:
						{
							return true;
						}

					// �ؽ� surface��ͬʱ�ؽ� swap chain
					case VK_ERROR_SURFACE_LOST_KHR:
					// �ؽ� swap chain
					case VK_SUBOPTIMAL_KHR:
					case VK_ERROR_OUT_OF_DATE_KHR:
					case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
						{
							return s_Window.Rebuild();
						}

					// ��ʧ device
					case VK_ERROR_DEVICE_LOST:
						{
							return s_VK_Device.on_Lost();
						}

					// ���� �豸�ڴ�
					case VK_ERROR_OUT_OF_DEVICE_MEMORY:
						{
							return true;
						}

					// ���� �����ڴ�
					case VK_ERROR_OUT_OF_HOST_MEMORY:
					case VK_ERROR_FRAGMENTED_POOL:
						{
							return s_MemMgmt.Cleanup();
						}

					// Ĭ�� ����
					default:
						{
						}
				}

			return false;
		}
}