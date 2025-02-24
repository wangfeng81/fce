/*
**
**	VK_PresentChain.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_PresentChain::VK_PresentChain() noexcept
		:
			curSyncObjects( VK_NULL_HANDLE ),
			endSyncObjects( VK_NULL_HANDLE )
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁
	 */
	FC_INLINE void VK_PresentChain::Destroy() noexcept
		{
			// 销毁 同步对象 数组
			this->DestroySyncObjects();

			// 释放 命令缓冲 数组
			this->FreeCommandBuffers();
		}

	/*
	 @	创建
	 */
	FC_INLINE VkResult VK_PresentChain::Create( const VK_SwapChain& swapChain ) noexcept
		{
			// 获取 swap chain 数量
			const uint32_t count
				{
					swapChain.Count()
				};

			// 分配 command buffer 数组
			VkResult res
				{
					this->AllocateCommandBuffers( count )
				};

			if( res == VK_SUCCESS )
				{
					// 创建 sync object 数组
					res = this->CreateSyncObjects( count );
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	释放 command buffer 数组
	 */
	FC_INLINE void VK_PresentChain::FreeCommandBuffers() noexcept
		{
			// 获取 count
			const uint32_t count
				{
					static_cast< uint32_t >
					( this->arrCommandBuffers.size() )
				};

			// 有 buffer 才释放
			if( count )
				{
					vkFreeCommandBuffers( s_VK_Device,
										  s_VK_Command,
										  count,
										  this->arrCommandBuffers.data() );
				}
		}


	/*
	 @	分配 command buffer 数组
	 */
	FC_INLINE VkResult VK_PresentChain::AllocateCommandBuffers( const uint32_t count ) noexcept
		{
			VkResult res;

			// 设置 Command buffer 数量
			this->arrCommandBuffers.resize( count );

			// 设置 create info
			const VkCommandBufferAllocateInfo createInfo
				{
					.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
					.pNext				= nullptr,
					.commandPool		= s_VK_Command,
					.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY,
					.commandBufferCount	= count
				};

			// 分配 Command buffer 数组
			VK_ERR_GUARD( vkAllocateCommandBuffers( s_VK_Device,
													&createInfo,
													this->arrCommandBuffers.data() ) );

			// 如果失败 则清空
			if( res != VK_SUCCESS )
				{
					this->arrCommandBuffers.clear();
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁 同步对象 数组
	 */
	FC_INLINE void VK_PresentChain::DestroySyncObjects() noexcept
		{
			for( SyncObjects& syncObj : this->arrSyncObjects )
				{
					if( syncObj.queueSubmited )
						{
							vkDestroyFence( s_VK_Device,
											syncObj.queueSubmited,
											s_VK_Allocator );
						}

					if( syncObj.imageAcquired )
						{
							vkDestroySemaphore( s_VK_Device,
												syncObj.imageAcquired,
												s_VK_Allocator );
						}

					if( syncObj.renderFinished )
						{
							vkDestroySemaphore( s_VK_Device,
												syncObj.renderFinished,
												s_VK_Allocator );
						}
				}

			// 由于可能 Rebuild 所以清空
			this->arrSyncObjects.clear();
		}


	/*
	 @	创建 同步对象 数组
	 */
	FC_INLINE VkResult VK_PresentChain::CreateSyncObjects( const uint32_t count ) noexcept
		{
			VkResult res;

			// 设置 sync object 数量，并初始化 circle iterator
			this->arrSyncObjects.resize( count );

			this->curSyncObjects = this->arrSyncObjects.data();

			this->endSyncObjects = this->curSyncObjects + ( count - 1 );

			// 设置 fence create info 初始 有信号
			const VkFenceCreateInfo fenceCreateInfo
				{
					.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
					.pNext = nullptr,
					.flags = VK_FENCE_CREATE_SIGNALED_BIT
				};

			// 设置 semaphore create info
			const VkSemaphoreCreateInfo semCreateInfo
				{
					.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0
				};

			// 创建 sync object 数组
			for( SyncObjects& syncObj : this->arrSyncObjects )
				{
					// Fence
					VK_ERR_GUARD( vkCreateFence( s_VK_Device,
												 &fenceCreateInfo,
												 s_VK_Allocator,
												 &syncObj.queueSubmited ) );
					if( res != VK_SUCCESS )
						{
							break;
						}

					// Semaphore image acquired
					VK_ERR_GUARD( vkCreateSemaphore( s_VK_Device,
													 &semCreateInfo,
													 s_VK_Allocator,
													 &syncObj.imageAcquired ) );
					if( res != VK_SUCCESS )
						{
							break;
						}

					// Semaphore render finished
					VK_ERR_GUARD( vkCreateSemaphore( s_VK_Device,
													 &semCreateInfo,
													 s_VK_Allocator,
													 &syncObj.renderFinished ) );
					if( res != VK_SUCCESS )
						{
							break;
						}
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	下一个 sync object
	 */
	FC_INLINE const VK_PresentChain::SyncObjects& VK_PresentChain::LoopSyncObjects() noexcept
		{
			// 移至下一帧，若到尾部 则折回
			if( this->curSyncObjects == this->endSyncObjects )
				{
					this->curSyncObjects = this->arrSyncObjects.data();
				}
			else
				{
					++this->curSyncObjects;
				}

			// 返回 sync object 引用
			return *this->curSyncObjects;
		}
}