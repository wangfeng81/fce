/*
**
**	VK_FrameBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	VK_FrameBuffer::~VK_FrameBuffer() noexcept
		{
			this->Release();
		}

	/*
	 @	����
	 */
	VK_FrameBuffer::VK_FrameBuffer( VkResult& res,
									const ViewList& arrViews,
									const VkExtent2D& extent2D,
									const VkRenderPass vkRenderPass ) noexcept
		:
			vkFramebuffer( VK_NULL_HANDLE )
		{
			res = this->Initialize( arrViews, extent2D, vkRenderPass );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	�ͷ�
	 */
	FC_INLINE void VK_FrameBuffer::Release() noexcept
		{
			// ���� frame buffer
			if( this->vkFramebuffer )
				{
					vkDestroyFramebuffer( s_VK_Device,
										  this->vkFramebuffer,
										  s_VK_Allocator );
				}
		}

	/*
	 @	��ʼ��
	 */
	FC_INLINE VkResult VK_FrameBuffer::Initialize( const ViewList& arrViews,
												   const VkExtent2D& extent2D,
												   const VkRenderPass vkRenderPass ) noexcept
		{
			VkResult res;

			// ���� create info
			const VkFramebufferCreateInfo createInfo
				{
					.sType			 = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
					.pNext			 = nullptr,
					.flags			 = 0,
					.renderPass		 = vkRenderPass,
					.attachmentCount = static_cast< uint32_t >( arrViews.size() ),
					.pAttachments	 =							arrViews.data(),
					.width			 = extent2D.width,
					.height			 = extent2D.height,
					.layers			 = 1
				};

			// ���� Frame buffer
			VK_ERR_GUARD( vkCreateFramebuffer( s_VK_Device,
											   &createInfo,
											   s_VK_Allocator,
											   &this->vkFramebuffer ) );

			return res;
		}
}



/**
 !	Frame buffer chain
 */
namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VK_FrameChain::VK_FrameChain() noexcept
		:
			renderPass( VK_NULL_HANDLE ),
			depthStencil( VK_NULL_HANDLE )
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE void VK_FrameChain::Destroy() noexcept
		{
			// ���� frame buffer array
			this->DestroyFrameBuffers();

			// ���� depth stencil
			this->DestroyDepthStencil();
		}

	/*
	 @	���� depth stencil
	 */
	FC_INLINE void VK_FrameChain::DestroyDepthStencil() noexcept
		{
			if( this->depthStencil )
				{
					this->depthStencil->Destroy();
				}
		}

	/*
	 @	���� frame buffer array
	 */
	FC_INLINE void VK_FrameChain::DestroyFrameBuffers() noexcept
		{
			for( VkFramebuffer vkFramebuffer : this->arrFrameBuffers )
				{
					if( vkFramebuffer )
						{
							vkDestroyFramebuffer( s_VK_Device,
												  vkFramebuffer,
												  s_VK_Allocator );
						}
				}
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	���� Common
	 */
	FC_INLINE VkResult VK_FrameChain::CreateCommon( const VK_SwapChain& swapChain ) noexcept
		{
			VkResult res{ VK_SUCCESS };

			// ���� RenderPass
			this->renderPass = RenderPass::Load( "renderPass/Common.rp" );

			if( ! renderPass )
				{
					return VK_ERROR_INITIALIZATION_FAILED;
				}

			// ���� create info
			VkFramebufferCreateInfo createInfo
				{
					.sType			 = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
					.pNext			 = nullptr,
					.flags			 = 0,
					.renderPass		 = renderPass->impl,
					.attachmentCount = 1,
					.pAttachments	 = VK_NULL_HANDLE,
					.width			 = swapChain->extent2D.width,
					.height			 = swapChain->extent2D.height,
					.layers			 = 1
				};

			/*
			 @	�� image view ���� frame buffer
			 */
			auto f = [ &res, &createInfo ]( const VkImageView vkImageView ) noexcept
				{
					// ��ʼ�� frame buffer ��
					VkFramebuffer vkFramebuffer{ VK_NULL_HANDLE };

					if( res == VK_SUCCESS )
						{
							// ���� swap chain view
							createInfo.pAttachments = &vkImageView;

							// ���� frame buffer
							VK_ERR_GUARD( vkCreateFramebuffer( s_VK_Device,
															   &createInfo,
															   s_VK_Allocator,
															   &vkFramebuffer ) );
						}

					return vkFramebuffer;
				};

			// ͨ�� swap chain ���� frame buffer array
			stl::transform( swapChain, this->arrFrameBuffers, f );

			return res;
		}


	/*
	 @	���� Compatible
	 */
	FC_INLINE VkResult VK_FrameChain::CreateCompatible( const VK_SwapChain& swapChain ) noexcept
		{
			VkResult res{ VK_SUCCESS };

			// ������ extent2D
			const VkExtent2D& extent2D
				{
					swapChain->extent2D
				};

			// ���� render pass
			this->renderPass = RenderPass::Load( "renderPass/Compatible.rp" );

			// ��� depth stencil
			this->depthStencil = DepthStencil::Generate( depthStencil::TYPE::DEPTH,
														 extent2D );

			if( ! this->renderPass || ! this->depthStencil )
				{
					// �༭�� ģʽ�� �Զ���ȫ�ļ�
					#ifdef FC_EDITOR

						RenderPass::Info info;

						Xml xml( "renderPass/Compatible.xml" );

						xml >> info;

						Buffer buf( info );

						buf.Save( "renderPass/Compatible.rp" );

						this->renderPass = RenderPass::Load( "renderPass/Compatible.rp" );

					#else

						return VK_ERROR_INITIALIZATION_FAILED;

					#endif
				}

			// ��װ View array
			VkImageView arrViews[]
				{
					VK_NULL_HANDLE,
					this->depthStencil->impl
				};

			// ���� create info
			const VkFramebufferCreateInfo createInfo
				{
					.sType			 = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
					.pNext			 = nullptr,
					.flags			 = 0,
					.renderPass		 = renderPass->impl,
					.attachmentCount = 2,
					.pAttachments	 = arrViews,
					.width			 = extent2D.width,
					.height			 = extent2D.height,
					.layers			 = 1
				};

			/*
			 @	�� image view ���� frame buffer
			 */
			auto f = [ &res, &createInfo, &arrViews ]( const VkImageView vkImageView ) noexcept
				{
					// ��ʼ�� frame buffer ��
					VkFramebuffer vkFramebuffer{ VK_NULL_HANDLE };

					if( res == VK_SUCCESS )
						{
							// ���� swap chain view
							arrViews[ 0 ] = vkImageView;

							// ���� frame buffer
							VK_ERR_GUARD( vkCreateFramebuffer( s_VK_Device,
															   &createInfo,
															   s_VK_Allocator,
															   &vkFramebuffer ) );
						}

					return vkFramebuffer;
				};

			// ͨ�� swap chain ���� frame buffer array
			stl::transform( swapChain, this->arrFrameBuffers, f );

			return res;
		}
}