/*
**
**	VK_IndexBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_IndexBuffer::VK_IndexBuffer( const VkIndexType type ) noexcept
		:
			vkType( type ),
			vkBuffer( VK_NULL_HANDLE ),
			vmaAlloc( VK_NULL_HANDLE )
		{
		}

	/*
	 @	释放
	 */
	FC_INLINE void VK_IndexBuffer::Release() noexcept
		{
			// 销毁 vma buffer 并释放 vk buffer
			if( this->vkBuffer && this->vmaAlloc )
				{
					vmaDestroyBuffer( s_VK_Allocator,
									  this->vkBuffer,
									  this->vmaAlloc );
				}
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	初始化 size
	 */
	FC_INLINE VkResult VK_IndexBuffer::Initialize( const VkDeviceSize size ) noexcept
		{
			VkResult res;

			// 设置 VkBuffer create info
			const VkBufferCreateInfo vkBufferCreateInfo
				{
					.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.size					= size,
					.usage					= VK_BUFFER_USAGE_INDEX_BUFFER_BIT
											| VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					.sharingMode			= s_VK_PhyDevice->sharingMode,
					.queueFamilyIndexCount	= 0,
					.pQueueFamilyIndices	= nullptr,
				};
			
			// 设置 Vma allocation create info
			const VmaAllocationCreateInfo vmaAllocCreateInfo
				{
					.flags			= 0,
					.usage			= VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
					.requiredFlags	= 0,
					.preferredFlags	= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					.memoryTypeBits	= 0,
					.pool			= nullptr,
					.pUserData		= nullptr,
					.priority		= 0.0f
				};
			
			// 创建 显存 vma buffer
			VK_ERR_GUARD( vmaCreateBuffer( s_VK_Allocator,
										   &vkBufferCreateInfo,
										   &vmaAllocCreateInfo,
										   &this->vkBuffer,
										   &this->vmaAlloc,
										   nullptr ) );

			return res;
		}


	/*
	 @	选择 索引位数
	 */
	FC_INLINE VkIndexType VK_IndexBuffer::Convert( const TYPE type ) noexcept
		{
			return type == TYPE::UINT16
				 ? VK_INDEX_TYPE_UINT16
				 : VK_INDEX_TYPE_UINT32;
		}
}