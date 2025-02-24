/*
**
**	VK_Command.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_Command::VK_Command() noexcept
		:
			vkCommandPool( VK_NULL_HANDLE ),

			// transfer
			transSubmitInfo
				{
					.sType				  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
					.pNext				  = nullptr,
					.waitSemaphoreCount	  = 0,
					.pWaitSemaphores	  = nullptr,
					.pWaitDstStageMask	  = nullptr,
					.commandBufferCount	  = 1,
					.pCommandBuffers	  = &this->transCmdBuf,
					.signalSemaphoreCount = 0,
					.pSignalSemaphores	  = nullptr
				},

			// VkCommandBufferBeginInfo
			transCmdBufBegInfo
				{
					.sType			  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
					.pNext			  = nullptr,
					.flags			  = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
					.pInheritanceInfo = nullptr
				}
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁
	 */
	FC_INLINE void VK_Command::Destroy() noexcept
		{
			// 销毁 Command pool
			if( this->vkCommandPool )
				{
					vkDestroyCommandPool( s_VK_Device,
										  this->vkCommandPool,
										  s_VK_Allocator );
				}
		}


	/*
	 @	创建
	 */
	FC_INLINE VkResult VK_Command::Create() noexcept
		{
			// 设置 create info
			const VkCommandPoolCreateInfo createInfo
				{
					.sType			  = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
					.pNext			  = nullptr,
					.flags			  = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
									  | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
					.queueFamilyIndex = s_VK_PhyDevice->queueFamilies.graph
				};

			// 创建 Command pool
			VkResult res
				{
					vkCreateCommandPool( s_VK_Device,
										 &createInfo,
										 s_VK_Allocator,
										 &this->vkCommandPool )
				};

			if( res == VK_SUCCESS )
				{
					// 分配 transmission command buffer
					res = this->AllocateTransBuffer();
				}

			return res;
		}


	/*
	 @	分配 transmission buffer
	 */
	FC_INLINE VkResult VK_Command::AllocateTransBuffer() noexcept
		{
			// 设置 allocate info
			const VkCommandBufferAllocateInfo allocateInfo
				{
					.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
					.pNext				= nullptr,
					.commandPool		= this->vkCommandPool,
					.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY,
					.commandBufferCount	= 1
				};

			// 分配 transmission buffer
			return vkAllocateCommandBuffers( s_VK_Device,
											&allocateInfo,
											&this->transCmdBuf );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	开始 传送
	 */
	VkResult VK_Command::BeginTransmission() noexcept
		{
			VkResult res;

			// 开始记录 传送指令
			VK_ERR_GUARD( vkBeginCommandBuffer( this->transCmdBuf,
												&this->transCmdBufBegInfo ) );

			return res;
		}


	/*
	 @	结束 传送
	 */
	VkResult VK_Command::EndTransmission() noexcept
		{
			VkResult res;

			// 结束记录 传送指令
			VK_ERR_GUARD( vkEndCommandBuffer( this->transCmdBuf ) );

			// 提交 传送指令
			VK_ERR_GUARD( vkQueueSubmit( s_VK_Device->queues.graph,
										 1, &this->transSubmitInfo,
										 VK_NULL_HANDLE ) );

			// 等待 队列空闲
			VK_ERR_GUARD( vkQueueWaitIdle( s_VK_Device->queues.graph ) );

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	拷贝 buffer
	 */
	VkResult VK_Command::CopyBuffer( VkBuffer srcBuffer,
									 VkBuffer dstBuffer,
									 const VkBufferCopy& bufferCopy ) noexcept
		{
			// 开始 传送
			const VkResult res
				{
					this->BeginTransmission()
				};

			if( res == VK_SUCCESS )
				{
					// 拷贝 srcBuffer 到 dstBuffer
					vkCmdCopyBuffer( this->transCmdBuf,
									 srcBuffer,
									 dstBuffer,
									 1, &bufferCopy );

					// 结束 传送
					return this->EndTransmission();
				}

			return res;
		}


	/*
	 @	拷贝 image
	 */
	VkResult VK_Command::CopyImage( VkBuffer srcBuffer,
									VkImage dstImage,
									const VkBufferImageCopy& region ) noexcept
		{
			// 开始 传送
			const VkResult res
				{
					this->BeginTransmission()
				};

			if( res == VK_SUCCESS )
				{
					// 布局转换
					VkImageMemoryBarrier imgMemBarrier
						{
							.sType					= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
							.pNext					= nullptr,
							.srcAccessMask			= 0,
							.dstAccessMask			= VK_ACCESS_TRANSFER_WRITE_BIT,
							.oldLayout				= VK_IMAGE_LAYOUT_UNDEFINED,
							.newLayout				= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
							.srcQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED,
							.dstQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED,
							.image					= dstImage,
							.subresourceRange
							{
								.aspectMask			= VK_IMAGE_ASPECT_COLOR_BIT,
								.baseMipLevel		= 0,
								.levelCount			= 1,
								.baseArrayLayer		= 0,
								.layerCount			= 1
							}
						};

					vkCmdPipelineBarrier( this->transCmdBuf,
										  VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
										  VK_PIPELINE_STAGE_TRANSFER_BIT,
										  0,
										  0, nullptr,
										  0, nullptr,
										  1, &imgMemBarrier );

					vkCmdCopyBufferToImage( this->transCmdBuf,
											srcBuffer, dstImage,
											VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
											1, &region );

					// 布局转换
					imgMemBarrier.image			= dstImage;
					imgMemBarrier.oldLayout		= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					imgMemBarrier.newLayout		= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imgMemBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					imgMemBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier( this->transCmdBuf,
										  VK_PIPELINE_STAGE_TRANSFER_BIT,
										  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
										  0,
										  0, nullptr,
										  0, nullptr,
										  1, &imgMemBarrier );

					// 结束 传送
					return this->EndTransmission();
				}

			return res;
		}
}