/*
**
**	VK_UniformBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_UniformBuffer::VK_UniformBuffer( VkDeviceSize vkDeviceSize ) noexcept
		:
			vkDeviceSize( vkDeviceSize ),
			vkBuffer( VK_NULL_HANDLE ),
			vmaAlloc( VK_NULL_HANDLE )
		{
		}

	/*
	 @	释放
	 */
	FC_INLINE void VK_UniformBuffer::Release() noexcept
		{
			// 销毁 vma buffer 并释放 vk buffer
			if( this->vkBuffer && this->vmaAlloc )
				{
					vmaDestroyBuffer( s_VK_Allocator,
									  this->vkBuffer,
									  this->vmaAlloc );
				}
		}


	/*
	 @	初始化
	 */
	FC_INLINE VkResult VK_UniformBuffer::Initialize() noexcept
		{
			VkResult res;

			// 设置 VkBuffer create info
			const VkBufferCreateInfo bufInfo
				{
					.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.size					= this->vkDeviceSize,
					.usage					= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
											| VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					.sharingMode			= s_VK_PhyDevice->sharingMode,
					.queueFamilyIndexCount	= 0,
					.pQueueFamilyIndices	= nullptr,
				};
			
			// 设置 Vma allocation create info
			const VmaAllocationCreateInfo vmaAllocCreateInfo
				{
					.flags			= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
									| VMA_ALLOCATION_CREATE_MAPPED_BIT,
					.usage			= VMA_MEMORY_USAGE_CPU_TO_GPU,
					.requiredFlags	= 0,
					.preferredFlags	= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					.memoryTypeBits	= 0,
					.pool			= nullptr,
					.pUserData		= nullptr,
					.priority		= 0.0f
				};
			
			// 创建 Buffer
			VK_ERR_GUARD( vmaCreateBuffer( s_VK_Allocator,
										   &bufInfo,
										   &vmaAllocCreateInfo,
										   &this->vkBuffer,
										   &this->vmaAlloc,
										   nullptr ) );

			return res;
		}


	/*
	 @	Update
	 */
	void VK_UniformBuffer::Update( const Void* p ) NOEXCEPT
		{
			// 打开 映射
			void* data;

			if( vmaMapMemory( s_VK_Allocator,
							  this->vmaAlloc,
							  &data ) == VK_SUCCESS )
				{
					// 拷贝 数据
					memcpy( data, p,
							size_t( this->vkDeviceSize ) );

					// 关闭 映射
					vmaUnmapMemory( s_VK_Allocator,
									this->vmaAlloc );
				};
		}
}