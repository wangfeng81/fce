/*
**
**	VK_Allocator.
**
		Vulkan allocator

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VK_Allocator::VK_Allocator() noexcept
		:
			deviceMemorySize( 0 ),

			// VmaAllocator
			vma( VK_NULL_HANDLE ),

			// VkAllocationCallbacks
			vkCallbacks
				{
					.pUserData		 = nullptr,
					.pfnAllocation	 = vkMalloc,
					.pfnReallocation = vkRealloc,
					.pfnFree		 = vkFree,

					// vk �ڲ������ͷ� ֪ͨ
					#ifdef FC_DEBUG

						.pfnInternalAllocation	= on_InternalAllocate,
						.pfnInternalFree		= on_InternalFree

					#else

						.pfnInternalAllocation	= nullptr,
						.pfnInternalFree		= nullptr
					
					#endif
				}
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VkResult VK_Allocator::Create() noexcept
		{
			// ���� VMA
			return this->CreateVMA();
		}

	/*
	 @	����
	 */
	FC_INLINE void VK_Allocator::Destroy() noexcept
		{
			// ���� VMA
			this->DestroyVMA();
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	vkFree
	 */
	FC_INLINE void VK_Allocator::vkFree( void* pUserData,
										 void* pMemory ) noexcept
		{
			s_App.Free( pMemory );
		}

	/*
	 @	vkMalloc
	 */
	FC_INLINE void* VK_Allocator::vkMalloc( void* pUserData,
											size_t size,
											size_t alignment,
											VkSystemAllocationScope scope ) noexcept
		{
			return s_App.Malloc( size, alignment );
		}

	/*
	 @	vkRealloc
	 */
	FC_INLINE void* VK_Allocator::vkRealloc( void* pUserData,
											 void* pOriginal,
											 size_t size,
											 size_t alignment,
											 VkSystemAllocationScope scope ) noexcept
		{
			return s_App.Realloc( pOriginal, size, alignment );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	���� VulkanMemoryAllocator
	 */
	FC_INLINE void VK_Allocator::DestroyVMA() noexcept
		{
			// ���� vam
			if( this->vma )
				{
					vmaDestroyAllocator( this->vma );
				}
		}


	/*
	 @	���� VulkanMemoryAllocator
	 */
	FC_INLINE VkResult VK_Allocator::CreateVMA() noexcept
		{
			/*
				vma �� vulkan functions Ԥ��ͨ�� volk ��̬����
				����Ϳ���ȡ�� ֱ���þ�̬��ȡ

				�Ժ���� volk & vma ����ر䶯
				����Ҫ�õ� VmaVulkanFunctions ����
				���磺volk ��������װ�� struct ��ȫ��
					  vma �����˾�̬��

				// ���� custom �Ķ�̬����
				VmaVulkanFunctions vmaFuncs
					{
						.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
						.vkGetDeviceProcAddr = vkGetDeviceProcAddr
					};
			*/

			// ���� device memory callbacks
			const VmaDeviceMemoryCallbacks vmaCallbacks
				{
					.pfnAllocate	= on_VmaAllocate,
					.pfnFree		= on_VmaFree,
					.pUserData		= nullptr
				};

			// ���� vma create info
			const VmaAllocatorCreateInfo createInfo
				{
					.flags						 = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT,// ע�� �˱�־Ϊ�Ż� �� ��Ҫͬ�����̷߳���
					.physicalDevice				 = s_VK_PhyDevice,
					.device						 = s_VK_Device,
					.preferredLargeHeapBlockSize = 0,
					.pAllocationCallbacks		 = &this->vkCallbacks,
					.pDeviceMemoryCallbacks		 = &vmaCallbacks,
					.pHeapSizeLimit				 = nullptr,
					.pVulkanFunctions			 = nullptr, // &vmaFuncs,
					.instance					 = s_VK_Instance,
					.vulkanApiVersion			 = VK_Instance::API_VERSION,

					#if VMA_EXTERNAL_MEMORY
						.pTypeExternalMemoryHandleTypes = nullptr
					#endif
				};

			// ���� vma
			return vmaCreateAllocator( &createInfo,
									   &this->vma );
		}


	/*
	 @	���� memory type index
	 */
	FC_INLINE uint32_t VK_Allocator::FindMemoryType( const uint32_t typeBits,
													 const VkMemoryPropertyFlags flags ) noexcept
		{
			// ��ȡ Memory properties
			VkPhysicalDeviceMemoryProperties properties;

			vkGetPhysicalDeviceMemoryProperties( s_VK_PhyDevice,
												 &properties );

			// �������� ƥ��� index
			for( uint32_t i{ 0 }; i < properties.memoryTypeCount; ++i )
				{
					if( typeBits & ( 1 << i )
					 && ( properties.memoryTypes[ i ].propertyFlags & flags ) == flags )
						{
							return i;
						}
				}

			// û��ƥ�������
			return UINT32_MAX;
		}


	/*
	 @	�ͷ� �Դ�
	 */
	void VK_Allocator::Free( VkDeviceMemory memory ) noexcept
		{
			if( memory )
				{
					vkFreeMemory( s_VK_Device,
								  memory,
								  s_VK_Allocator );
				}
		}


	/*
	 @	���� �Դ�
	 */
	VkResult VK_Allocator::Malloc( VkDeviceMemory& memory,
								   const VkMemoryRequirements& requirements,
								   const VkMemoryPropertyFlags flags ) noexcept
		{
			// ���� Memory type index
			const uint32_t typeIndex
				{
					FindMemoryType( requirements.memoryTypeBits,
									flags )
				};

			// ���� memory allocate info
			const VkMemoryAllocateInfo allocInfo
				{
					.sType			 = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					.pNext			 = nullptr,
					.allocationSize  = requirements.size,
					.memoryTypeIndex = typeIndex
				};

			// ���� memory
			return vkAllocateMemory( s_VK_Device,
									 &allocInfo,
									 s_VK_Allocator,
									 &memory );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	VK �ڲ��ͷ� ֪ͨ
	 */
	void VK_Allocator::on_InternalFree( void* pUserData,
										size_t size,
										VkInternalAllocationType type,
										VkSystemAllocationScope scope ) noexcept
		{
			FC_LOG( "Vulkan internal free: %d", size );
		}

	/*
	 @	VK �ڲ����� ֪ͨ
	 */
	void VK_Allocator::on_InternalAllocate( void* pUserData,
											size_t size,
											VkInternalAllocationType type,
											VkSystemAllocationScope scope ) noexcept
		{
			FC_LOG( "Vulkan internal allocate: %d", size );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	vma �Դ� �ͷ� ֪ͨ
	 */
	void VK_Allocator::on_VmaFree( VmaAllocator		allocator,
								   uint32_t			memoryType,
								   VkDeviceMemory	memory,
								   VkDeviceSize		size,
								   void*			pUserData ) noexcept
		{
			s_VK_Allocator.deviceMemorySize -= size;

			//FC_LOG( "Vulkan �Դ�:%d M  ���٣� %d M", s_VK_Allocator.deviceMemorySize >> 20, size >> 20 );
		}

	/*
	 @	vma �Դ� ���� ֪ͨ
	 */
	void VK_Allocator::on_VmaAllocate( VmaAllocator		allocator,
									   uint32_t			memoryType,
									   VkDeviceMemory	memory,
									   VkDeviceSize		size,
									   void*			pUserData ) noexcept
		{
			s_VK_Allocator.deviceMemorySize += size;

			//FC_LOG( "Vulkan �Դ�:%d M  ���ӣ� %d M", s_VK_Allocator.deviceMemorySize >> 20, size >> 20 );
		}
}