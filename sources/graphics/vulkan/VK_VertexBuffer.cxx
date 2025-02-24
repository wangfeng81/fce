/*
**
**	VK_VertexBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VK_VertexBuffer::VK_VertexBuffer() noexcept
		:
			vkBuffer( VK_NULL_HANDLE ),
			vmaAlloc( VK_NULL_HANDLE )
		{
		}

	/*
	 @	�ͷ�
	 */
	FC_INLINE void VK_VertexBuffer::Release() noexcept
		{
			// ���� vma buffer ���ͷ� vk buffer
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
	 @	��ʼ�� size
	 */
	FC_INLINE VkResult VK_VertexBuffer::Initialize( const VkDeviceSize size ) noexcept
		{
			VkResult res;

			// ���� VkBuffer create info
			const VkBufferCreateInfo vkBufferCreateInfo
				{
					.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.size					= size,
					.usage					= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
											| VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					.sharingMode			= s_VK_PhyDevice->sharingMode,
					.queueFamilyIndexCount	= 0,
					.pQueueFamilyIndices	= nullptr,
				};
			
			// ���� Vma allocation create info
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
			
			// ���� �Դ� vma buffer
			VK_ERR_GUARD( vmaCreateBuffer( s_VK_Allocator,
										   &vkBufferCreateInfo,
										   &vmaAllocCreateInfo,
										   &this->vkBuffer,
										   &this->vmaAlloc,
										   nullptr ) );

			return res;
		}


	/*
	 @	����� memory
	 */
	//FC_INLINE Bool VK_VertexBuffer::BindMemory()
	//	{
	//		// ��ȡ memory requirements
	//		VkMemoryRequirements requirements;

	//		vkGetBufferMemoryRequirements( s_VK_Device,
	//									   this->vb,
	//									   &requirements );

	//		// ���� memory
	//		if( s_VK_Allocator.Malloc( this->memory,
	//								   requirements,
	//								   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
	//								 | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
	//			{
	//				// �� memory
	//				return vkBindBufferMemory( s_VK_Device,
	//										   this->vb,
	//										   this->memory, 0 ) == VK_SUCCESS;
	//			}

	//		return false;
	//	}


	/*
	 @	���� ����
	 */
	//FC_INLINE Bool VK_VertexBuffer::Copy( const Void* src,
	//									  const UInt size )
	//	{
	//		// �� ӳ��
	//		void* data;

	//		if( vkMapMemory( s_VK_Device,
	//						 this->memory,
	//						 0, size,
	//						 0, &data ) == VK_SUCCESS )
	//			{
	//				// ���� ����
	//				memcpy( data, src, size );

	//				// �ر� ӳ��
	//				vkUnmapMemory( s_VK_Device,
	//							   this->memory );

	//				return true;
	//			};

	//		return false;
	//	}
}