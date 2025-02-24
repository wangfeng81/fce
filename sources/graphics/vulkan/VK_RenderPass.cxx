/*
**
**	VK_RenderPass.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

/**
 !	VK_Subpass	=>	��ͨ��
 */
namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VK_Subpass::VK_Subpass( const renderPass::Subpass& subpass )
		:
			// ת�� resolve & depthStencil attachment reference
			resolve
				(
					Convert( subpass.resolve )
				),
			depthStencil
				(
					Convert( subpass.depthStencil )
				),

			// ת�� Preserve attachment index array
			arrPreserves( subpass.arrPreserves ),

			// ת�� Input & Color attachment reference array
			arrInputs
				(
					stl::transform( subpass.arrInputs,
									Convert )
				),
			arrColors
				(
					stl::transform( subpass.arrColors,
									Convert )
				)
		{
		}

	/*
	 @	ת�� Subpass description
	 */
	FC_INLINE VK_Subpass::operator VkSubpassDescription() const noexcept
		{
			return VkSubpassDescription
					{
						.flags					 = 0,
						.pipelineBindPoint		 = VK_PIPELINE_BIND_POINT_GRAPHICS,
						.inputAttachmentCount	 = static_cast< uint32_t >( this->arrInputs.size() ),
						.pInputAttachments		 =							this->arrInputs.data(),
						.colorAttachmentCount	 = static_cast< uint32_t >( this->arrColors.size() ),
						.pColorAttachments		 =							this->arrColors.data(),
						.pResolveAttachments	 = Choose( this->resolve ),
						.pDepthStencilAttachment = Choose( this->depthStencil ),
						.preserveAttachmentCount = static_cast< uint32_t >( this->arrPreserves.size() ),
						.pPreserveAttachments	 =							this->arrPreserves.data()
					};
		}

	/*
	 @	ת�� Attachment reference
	 */
	FC_INLINE VkAttachmentReference VK_Subpass::Convert( const renderPass::Reference& reference )
		{
			return VkAttachmentReference
					{
						.attachment	= reference.index,
						.layout		= RenderPass::Convert( reference.layout )
					};
		}

	/*
	 @	���� Attachment reference
	 */
	FC_INLINE const VkAttachmentReference* VK_Subpass::Choose( const VkAttachmentReference& reference ) noexcept
		{
			// �� UINT32_MAX ����δʹ��
			return reference.attachment != UINT32_MAX
				   ? &reference : nullptr;
		}
}



/**
 !	VK_AttachmentGroup	=>	������
 */
namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VK_AttachmentGroup::VK_AttachmentGroup( const stl::vector< renderPass::Attachment >& arrAttachments )
		:
			// ת�� Attachment description array
			arrDescriptions
				(
					stl::transform( arrAttachments,
									Convert )
				)
		{
		}

	/*
	 @	ת�� Attachment description
	 */
	FC_INLINE VkAttachmentDescription VK_AttachmentGroup::Convert( const renderPass::Attachment& attachment )
		{
			return VkAttachmentDescription
					{
						.flags			= 0,
						.format			= VK_RenderPass::Convert( attachment.format ),
						.samples		= Multisample::Convert( attachment.sample ),
						.loadOp			= RenderPass::Convert( attachment.load ),
						.storeOp		= RenderPass::Convert( attachment.store ),
						.stencilLoadOp	= RenderPass::Convert( attachment.stencilLoad ),
						.stencilStoreOp	= RenderPass::Convert( attachment.stencilStore ),
						.initialLayout	= RenderPass::Convert( attachment.initialLayout ),
						.finalLayout	= RenderPass::Convert( attachment.finalLayout )
					};
		}

	/*
	 @	��ȡ Attachment description ����
	 */
	FC_INLINE const VkAttachmentDescription* VK_AttachmentGroup::Data() const noexcept
		{
			return this->arrDescriptions.data();
		}

	/*
	 @	��ȡ Attachment description ����
	 */
	FC_INLINE uint32_t VK_AttachmentGroup::Count() const noexcept
		{
			return static_cast< uint32_t >
					(
						this->arrDescriptions.size()
					);
		}
}



/**
 !	VK_DependencyGroup	=>	��ͨ��������
 */
namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VK_DependencyGroup::VK_DependencyGroup( const stl::vector< renderPass::Dependency >& arrDependencies )
		:
			// ת�� Subpass dependency array
			arrDescriptions
				(
					stl::transform( arrDependencies, Convert )
				)
		{
		}

	/*
	 @	ת�� Subpass dependency
	 */
	FC_INLINE VkSubpassDependency VK_DependencyGroup::Convert( const renderPass::Dependency& dependency )
		{
			return VkSubpassDependency
					{
						.srcSubpass		 = dependency.srcSubpass,
						.dstSubpass		 = dependency.dstSubpass,
						.srcStageMask	 = RenderPass::Combine( dependency.srcStages ),
						.dstStageMask	 = RenderPass::Combine( dependency.dstStages ),
						.srcAccessMask	 = RenderPass::Combine( dependency.srcAccesses ),
						.dstAccessMask	 = RenderPass::Combine( dependency.dstAccesses ),
						.dependencyFlags = RenderPass::Combine( dependency.arrTypes ),
					};
		}

	/*
	 @	��ȡ Subpass dependency ����
	 */
	FC_INLINE const VkSubpassDependency* VK_DependencyGroup::Data() const noexcept
		{
			return this->arrDescriptions.data();
		}

	/*
	 @	��ȡ Subpass dependency ����
	 */
	FC_INLINE uint32_t VK_DependencyGroup::Count() const noexcept
		{
			return static_cast< uint32_t >
					(
						this->arrDescriptions.size()
					);
		}
}



/**
 !	VK_SubpassGroup	=>	��ͨ����
 */
namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VK_SubpassGroup::VK_SubpassGroup( const stl::vector< renderPass::Subpass >& arrSubpasses )
		:
			// ת�� VK_Subpass array ������ת������ Attachment reference array
			vk_subpasses
				(
					stl::transform< decltype( vk_subpasses ) >( arrSubpasses )
				),

			// ת�� Subpass description array
			arrDescriptions
				(
					stl::transform< decltype( arrDescriptions ) >( vk_subpasses )
				)
		{
		}

	/*
	 @	��ȡ Subpass description ����
	 */
	FC_INLINE const VkSubpassDescription* VK_SubpassGroup::Data() const noexcept
		{
			return this->arrDescriptions.data();
		}

	/*
	 @	��ȡ Subpass description ����
	 */
	FC_INLINE uint32_t VK_SubpassGroup::Count() const noexcept
		{
			return static_cast< uint32_t >
					(
						this->arrDescriptions.size()
					);
		}
}



/**
 !	VK_RenderPass
 */
namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VK_RenderPass::VK_RenderPass() noexcept
		:
			vkRenderPass( VK_NULL_HANDLE )
		{
		}

	/*
	 @	�ͷ�
	 */
	FC_INLINE void VK_RenderPass::Release() noexcept
		{
			// ���� render pass
			if( this->vkRenderPass )
				{
					vkDestroyRenderPass( s_VK_Device,
										 this->vkRenderPass,
										 s_VK_Allocator );
				}
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	��ʼ�� Info
	 */
	FC_INLINE VkResult VK_RenderPass::Initialize( const renderPass::Info& info )
		{
			VkResult res;

			// ���� Groups
			const VK_SubpassGroup subpassGroup( info.arrSubpasses );

			const VK_AttachmentGroup attachmentGroup( info.arrAttachments );

			const VK_DependencyGroup dependencyGroup( info.arrDependencies );

			// ���� create info
			const VkRenderPassCreateInfo createInfo
				{
					.sType			 = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
					.pNext			 = nullptr,
					.flags			 = 0,
					.attachmentCount = attachmentGroup.Count(),
					.pAttachments	 = attachmentGroup.Data(),
					.subpassCount	 = subpassGroup.Count(),
					.pSubpasses		 = subpassGroup.Data(),
					.dependencyCount = dependencyGroup.Count(),
					.pDependencies	 = dependencyGroup.Data(),
				};

			// ���� Render pass
			VK_ERR_GUARD( vkCreateRenderPass( s_VK_Device,
											  &createInfo,
											  s_VK_Allocator,
											  &this->vkRenderPass ) );

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	ת�� VkFormat
	 */
	FC_INLINE VkFormat VK_RenderPass::Convert( const renderPass::FORMAT format )
		{
			switch( format )
				{
					// Swap chain surface
					case renderPass::FORMAT::SWAP_CHAIN:
						{
							return s_VK_SwapChain->surface.format;
						}

					// Depth stencil
					case renderPass::FORMAT::DEPTH:
					case renderPass::FORMAT::STENCIL:
					case renderPass::FORMAT::DEPTH_STENCIL:
						{
							VK_DepthStencil::Config cfg;

							const DepthStencil::TYPE type
								{
									DepthStencil::TYPE( UInt8( format )
													  - UInt8( renderPass::FORMAT::DEPTH ) )
								};

							VK_DepthStencil::ChooseConfig( cfg, type );

							return cfg.format;
						}

					// error
					default: FC_ASSERT( VK_FORMAT_UNDEFINED );
				}

			return VK_FORMAT_UNDEFINED;
		}
}