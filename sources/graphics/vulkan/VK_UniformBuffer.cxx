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
	 @	����
	 */
	FC_INLINE VK_UniformBuffer::VK_UniformBuffer( VkDeviceSize vkDeviceSize ) noexcept
		:
			vkDeviceSize( vkDeviceSize ),
			vkBuffer( VK_NULL_HANDLE ),
			vmaAlloc( VK_NULL_HANDLE )
		{
		}

	/*
	 @	�ͷ�
	 */
	FC_INLINE void VK_UniformBuffer::Release() noexcept
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
	 @	��ʼ��
	 */
	FC_INLINE VkResult VK_UniformBuffer::Initialize() noexcept
		{
			VkResult res;

			// ���� VkBuffer create info
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
			
			// ���� Vma allocation create info
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
			
			// ���� Buffer
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
			// �� ӳ��
			void* data;

			if( vmaMapMemory( s_VK_Allocator,
							  this->vmaAlloc,
							  &data ) == VK_SUCCESS )
				{
					// ���� ����
					memcpy( data, p,
							size_t( this->vkDeviceSize ) );

					// �ر� ӳ��
					vmaUnmapMemory( s_VK_Allocator,
									this->vmaAlloc );
				};
		}
}