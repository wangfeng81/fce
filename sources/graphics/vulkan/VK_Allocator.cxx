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
	 @	构造
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

					// vk 内部分配释放 通知
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
	 @	创建
	 */
	FC_INLINE VkResult VK_Allocator::Create() noexcept
		{
			// 创建 VMA
			return this->CreateVMA();
		}

	/*
	 @	销毁
	 */
	FC_INLINE void VK_Allocator::Destroy() noexcept
		{
			// 销毁 VMA
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
	 @	销毁 VulkanMemoryAllocator
	 */
	FC_INLINE void VK_Allocator::DestroyVMA() noexcept
		{
			// 销毁 vam
			if( this->vma )
				{
					vmaDestroyAllocator( this->vma );
				}
		}


	/*
	 @	创建 VulkanMemoryAllocator
	 */
	FC_INLINE VkResult VK_Allocator::CreateVMA() noexcept
		{
			/*
				vma 的 vulkan functions 预先通过 volk 动态加载
				这里就可以取巧 直接用静态获取

				以后如果 volk & vma 有相关变动
				则需要用到 VmaVulkanFunctions 设置
				比如：volk 将函数封装到 struct 非全局
					  vma 链接了静态库

				// 设置 custom 的动态加载
				VmaVulkanFunctions vmaFuncs
					{
						.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
						.vkGetDeviceProcAddr = vkGetDeviceProcAddr
					};
			*/

			// 设置 device memory callbacks
			const VmaDeviceMemoryCallbacks vmaCallbacks
				{
					.pfnAllocate	= on_VmaAllocate,
					.pfnFree		= on_VmaFree,
					.pUserData		= nullptr
				};

			// 设置 vma create info
			const VmaAllocatorCreateInfo createInfo
				{
					.flags						 = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT,// 注意 此标志为优化 而 需要同步多线程分配
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

			// 创建 vma
			return vmaCreateAllocator( &createInfo,
									   &this->vma );
		}


	/*
	 @	查找 memory type index
	 */
	FC_INLINE uint32_t VK_Allocator::FindMemoryType( const uint32_t typeBits,
													 const VkMemoryPropertyFlags flags ) noexcept
		{
			// 获取 Memory properties
			VkPhysicalDeviceMemoryProperties properties;

			vkGetPhysicalDeviceMemoryProperties( s_VK_PhyDevice,
												 &properties );

			// 遍历查找 匹配的 index
			for( uint32_t i{ 0 }; i < properties.memoryTypeCount; ++i )
				{
					if( typeBits & ( 1 << i )
					 && ( properties.memoryTypes[ i ].propertyFlags & flags ) == flags )
						{
							return i;
						}
				}

			// 没有匹配的类型
			return UINT32_MAX;
		}


	/*
	 @	释放 显存
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
	 @	分配 显存
	 */
	VkResult VK_Allocator::Malloc( VkDeviceMemory& memory,
								   const VkMemoryRequirements& requirements,
								   const VkMemoryPropertyFlags flags ) noexcept
		{
			// 查找 Memory type index
			const uint32_t typeIndex
				{
					FindMemoryType( requirements.memoryTypeBits,
									flags )
				};

			// 设置 memory allocate info
			const VkMemoryAllocateInfo allocInfo
				{
					.sType			 = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					.pNext			 = nullptr,
					.allocationSize  = requirements.size,
					.memoryTypeIndex = typeIndex
				};

			// 分配 memory
			return vkAllocateMemory( s_VK_Device,
									 &allocInfo,
									 s_VK_Allocator,
									 &memory );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	VK 内部释放 通知
	 */
	void VK_Allocator::on_InternalFree( void* pUserData,
										size_t size,
										VkInternalAllocationType type,
										VkSystemAllocationScope scope ) noexcept
		{
			FC_LOG( "Vulkan internal free: %d", size );
		}

	/*
	 @	VK 内部分配 通知
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
	 @	vma 显存 释放 通知
	 */
	void VK_Allocator::on_VmaFree( VmaAllocator		allocator,
								   uint32_t			memoryType,
								   VkDeviceMemory	memory,
								   VkDeviceSize		size,
								   void*			pUserData ) noexcept
		{
			s_VK_Allocator.deviceMemorySize -= size;

			//FC_LOG( "Vulkan 显存:%d M  减少： %d M", s_VK_Allocator.deviceMemorySize >> 20, size >> 20 );
		}

	/*
	 @	vma 显存 分配 通知
	 */
	void VK_Allocator::on_VmaAllocate( VmaAllocator		allocator,
									   uint32_t			memoryType,
									   VkDeviceMemory	memory,
									   VkDeviceSize		size,
									   void*			pUserData ) noexcept
		{
			s_VK_Allocator.deviceMemorySize += size;

			//FC_LOG( "Vulkan 显存:%d M  增加： %d M", s_VK_Allocator.deviceMemorySize >> 20, size >> 20 );
		}
}