/*
**
**	VK_TempBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	VK_TempBuffer::~VK_TempBuffer() noexcept
		{
			this->Release();
		}

	/*
	 @	���� move
	 */
	VK_TempBuffer::VK_TempBuffer( VK_TempBuffer&& rv ) noexcept
		:
			size		( rv.size ),
			vkBuffer	( rv.vkBuffer ),
			vmaAlloc	( rv.vmaAlloc ),
			mappedData	( rv.mappedData )
		{
			rv.vkBuffer = VK_NULL_HANDLE;
		}

	/*
	 @	���� copy
	 */
	VK_TempBuffer::VK_TempBuffer( const VK_TempBuffer& lv ) NOEXCEPT
		:
			VK_TempBuffer( lv.size )
		{
			FC_ASSERT( *this && lv );

			memcpy( this->mappedData,
					lv.mappedData,
					lv.size );
		}

	/*
	 @	���� size
	 */
	VK_TempBuffer::VK_TempBuffer( const VkDeviceSize size ) NOEXCEPT
		:
			size		( size ),
			vkBuffer	( VK_NULL_HANDLE ),
			vmaAlloc	( VK_NULL_HANDLE ),
			mappedData	( VK_NULL_HANDLE )
		{
			const VkResult res
				{
					this->Initialize( size )
				};

			FC_ASSERT( res == VK_SUCCESS || size == 0 );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	��ֵ �ƶ�
	 */
	VK_TempBuffer& VK_TempBuffer::operator = ( VK_TempBuffer&& rv ) noexcept
		{
			std::swap( this->size,		 rv.size );
			std::swap( this->vkBuffer,	 rv.vkBuffer );
			std::swap( this->vmaAlloc,	 rv.vmaAlloc );
			std::swap( this->mappedData, rv.mappedData );

			return *this;
		}

	/*
	 @	��ֵ ����
	 */
	VK_TempBuffer& VK_TempBuffer::operator = ( const VK_TempBuffer& lv ) NOEXCEPT
		{
			return *this = VK_TempBuffer( lv );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	void VK_TempBuffer::Resize( const VkDeviceSize size ) NOEXCEPT
		{
			*this = VK_TempBuffer( size );
		}

	/*
	 @	���л�
	 */
	Buffer& VK_TempBuffer::operator >> ( Buffer& buf ) const NOEXCEPT
		{
			return buf.Write( this->mappedData,
						UInt( this->size ) );
		}

	/*
	 @	����
	 */
	VkResult VK_TempBuffer::Copy( VkBuffer dstBuffer,
								  const VkDeviceSize size,
								  const VkDeviceSize srcOffset,
								  const VkDeviceSize dstOffset ) const NOEXCEPT
		{
			// ���� Copy region
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



namespace fce::inner::vulkan
{
	/*
	 @	�ͷ�
	 */
	FC_INLINE void VK_TempBuffer::Release() noexcept
		{
			// ���� vma buffer ���ͷ� vk buffer
			if( this->vkBuffer && this->vmaAlloc )
				{
					vmaDestroyBuffer( s_VK_Allocator,
									  this->vkBuffer,
									  this->vmaAlloc );
				}
		}


	/*
	 @	��ʼ�� size
	 */
	FC_INLINE VkResult VK_TempBuffer::Initialize( const VkDeviceSize size ) noexcept
		{
			VkResult res;

			// ���� VkBuffer create info
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
			
			// ���� Vma allocation create info
			const VmaAllocationCreateInfo vmaAllocCreateInfo
				{
					.flags			= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
									| VMA_ALLOCATION_CREATE_MAPPED_BIT,
					.usage			= VMA_MEMORY_USAGE_CPU_ONLY,
					.requiredFlags	= 0,
					.preferredFlags	= VK_MEMORY_PROPERTY_HOST_CACHED_BIT,	// ����������� ��ȡ��ǳ���
					.memoryTypeBits	= 0,
					.pool			= nullptr,
					.pUserData		= nullptr,
					.priority		= 0.0f
				};
			
			// ���� VkBuffer
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