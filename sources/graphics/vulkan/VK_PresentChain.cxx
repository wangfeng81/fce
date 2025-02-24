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
	 @	����
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
	 @	����
	 */
	FC_INLINE void VK_PresentChain::Destroy() noexcept
		{
			// ���� ͬ������ ����
			this->DestroySyncObjects();

			// �ͷ� ����� ����
			this->FreeCommandBuffers();
		}

	/*
	 @	����
	 */
	FC_INLINE VkResult VK_PresentChain::Create( const VK_SwapChain& swapChain ) noexcept
		{
			// ��ȡ swap chain ����
			const uint32_t count
				{
					swapChain.Count()
				};

			// ���� command buffer ����
			VkResult res
				{
					this->AllocateCommandBuffers( count )
				};

			if( res == VK_SUCCESS )
				{
					// ���� sync object ����
					res = this->CreateSyncObjects( count );
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	�ͷ� command buffer ����
	 */
	FC_INLINE void VK_PresentChain::FreeCommandBuffers() noexcept
		{
			// ��ȡ count
			const uint32_t count
				{
					static_cast< uint32_t >
					( this->arrCommandBuffers.size() )
				};

			// �� buffer ���ͷ�
			if( count )
				{
					vkFreeCommandBuffers( s_VK_Device,
										  s_VK_Command,
										  count,
										  this->arrCommandBuffers.data() );
				}
		}


	/*
	 @	���� command buffer ����
	 */
	FC_INLINE VkResult VK_PresentChain::AllocateCommandBuffers( const uint32_t count ) noexcept
		{
			VkResult res;

			// ���� Command buffer ����
			this->arrCommandBuffers.resize( count );

			// ���� create info
			const VkCommandBufferAllocateInfo createInfo
				{
					.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
					.pNext				= nullptr,
					.commandPool		= s_VK_Command,
					.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY,
					.commandBufferCount	= count
				};

			// ���� Command buffer ����
			VK_ERR_GUARD( vkAllocateCommandBuffers( s_VK_Device,
													&createInfo,
													this->arrCommandBuffers.data() ) );

			// ���ʧ�� �����
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
	 @	���� ͬ������ ����
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

			// ���ڿ��� Rebuild �������
			this->arrSyncObjects.clear();
		}


	/*
	 @	���� ͬ������ ����
	 */
	FC_INLINE VkResult VK_PresentChain::CreateSyncObjects( const uint32_t count ) noexcept
		{
			VkResult res;

			// ���� sync object ����������ʼ�� circle iterator
			this->arrSyncObjects.resize( count );

			this->curSyncObjects = this->arrSyncObjects.data();

			this->endSyncObjects = this->curSyncObjects + ( count - 1 );

			// ���� fence create info ��ʼ ���ź�
			const VkFenceCreateInfo fenceCreateInfo
				{
					.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
					.pNext = nullptr,
					.flags = VK_FENCE_CREATE_SIGNALED_BIT
				};

			// ���� semaphore create info
			const VkSemaphoreCreateInfo semCreateInfo
				{
					.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0
				};

			// ���� sync object ����
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
	 @	��һ�� sync object
	 */
	FC_INLINE const VK_PresentChain::SyncObjects& VK_PresentChain::LoopSyncObjects() noexcept
		{
			// ������һ֡������β�� ���ۻ�
			if( this->curSyncObjects == this->endSyncObjects )
				{
					this->curSyncObjects = this->arrSyncObjects.data();
				}
			else
				{
					++this->curSyncObjects;
				}

			// ���� sync object ����
			return *this->curSyncObjects;
		}
}