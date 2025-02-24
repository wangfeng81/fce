/*
**
**	VK_Buffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	析构
	 */
	VK_Buffer::~VK_Buffer() noexcept
		{
			this->Release();
		}

	/*
	 @	构造 move
	 */
	VK_Buffer::VK_Buffer( VK_Buffer&& rv ) noexcept
		:
			vkBuffer( rv.vkBuffer ),
			vmaAlloc( rv.vmaAlloc )
		{
			rv.vkBuffer = VK_NULL_HANDLE;
			rv.vmaAlloc = VK_NULL_HANDLE;
		}

	/*
	 @	构造 size
	 */
	VK_Buffer::VK_Buffer( const VkDeviceSize size ) NOEXCEPT
		:
			vkBuffer( VK_NULL_HANDLE ),
			vmaAlloc( VK_NULL_HANDLE )
		{
			const VkResult res
				{
					this->Initialize( size )
				};

			FC_ASSERT( res == VK_SUCCESS );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	释放
	 */
	FC_INLINE void VK_Buffer::Release() noexcept
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
	 @	初始化 size
	 */
	FC_INLINE VkResult VK_Buffer::Initialize( const VkDeviceSize size ) noexcept
		{
			VkResult res;

			// 设置 VkBuffer create info
			const VkBufferCreateInfo vkBufferCreateInfo
				{
					.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.size					= size,
					.usage					= VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
					.sharingMode			= s_VK_PhyDevice->sharingMode,
					.queueFamilyIndexCount	= 0,
					.pQueueFamilyIndices	= nullptr,
				};
			
			// 设置 Vma allocation create info
			const VmaAllocationCreateInfo vmaAllocCreateInfo
				{
					.flags			= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
									| VMA_ALLOCATION_CREATE_MAPPED_BIT,
					.usage			= VMA_MEMORY_USAGE_AUTO,
					.requiredFlags	= 0,
					.preferredFlags	= 0,
					.memoryTypeBits	= 0,
					.pool			= nullptr,
					.pUserData		= nullptr,
					.priority		= 0.0f
				};
			
			// 创建 VkBuffer
			VmaAllocationInfo vmaAllocInfo{};

			VK_ERR_GUARD( vmaCreateBuffer( s_VK_Allocator,
										   &vkBufferCreateInfo,
										   &vmaAllocCreateInfo,
										   &this->vkBuffer,
										   &this->vmaAlloc,
										   &vmaAllocInfo ) );

			this->mappedData = vmaAllocInfo.pMappedData;

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	拷贝
	 */
	VkResult VK_Buffer::Copy( VkBuffer dstBuffer,
								  const VkDeviceSize size,
								  const VkDeviceSize srcOffset,
								  const VkDeviceSize dstOffset ) const NOEXCEPT
		{
			const VkBufferCopy region
				{
					.srcOffset	= srcOffset,
					.dstOffset	= dstOffset,
					.size		= size
				};

			return s_VK_Command.CopyBuffer( this->vkBuffer,
											dstBuffer,
											region );
		}
}