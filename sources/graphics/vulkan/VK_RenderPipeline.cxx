/*
**
**	VK_RenderPipeline.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

#include "VK_Multisample.cxx"
#include "VK_Rasterization.cxx"
#include "VK_ColorBlend.cxx"


/**
 !	VK_ShaderProgram	=>	着色 程序组
 */
namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_ShaderProgram::VK_ShaderProgram( const stl::vector< shader::Info >& arrShaders )
		:
			// 转换 shader group
			arrCreateInfos
				(
					stl::transform( arrShaders, Convert )
				)
		{
		}

	/*
	 @	通过 Shader::Info 设置 create info
	 */
	FC_INLINE VkPipelineShaderStageCreateInfo VK_ShaderProgram::Convert( const shader::Info& info )
		{
			// 加载 Shader
			const Shader* const shader
				{
					Shader::LoadAsset( info.name )
				};

			FC_ASSERT( shader );

			// 设置 create Info
			return VkPipelineShaderStageCreateInfo
					{
						.sType				 = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
						.pNext				 = nullptr,
						.flags				 = 0,
						.stage				 = Shader::Convert( info.type ),
						.module				 = shader->impl,
						.pName				 = info.entry.c_str(),
						.pSpecializationInfo = nullptr
					};
		}

	/*
	 @	获取 shader 数据
	 */
	FC_INLINE const VkPipelineShaderStageCreateInfo* VK_ShaderProgram::Data() const noexcept
		{
			return this->arrCreateInfos.data();
		}

	/*
	 @	获取 shader 数量
	 */
	FC_INLINE uint32_t VK_ShaderProgram::Count() const noexcept
		{
			return static_cast< uint32_t >
					(
						this->arrCreateInfos.size()
					);
		}
}



/**
 !	VK_VertexDescribe	=>	顶点描述
 */
namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_VertexDescribe::VK_VertexDescribe( const vertex::Info& info )
		:
			// 设置 Binding create info
			bindingCreateInfo
				{
					this->SetCreateInfo( info )
				},

			// 设置 Assembly create Info
			assemblyCreateInfo
				{
					.sType					= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.topology				= VertexBuffer::Convert( info.primitive ),
					.primitiveRestartEnable	= info.restart
				}
		{
		}


	/*
	 @	设置 Binding create info
	 */
	FC_INLINE VkPipelineVertexInputStateCreateInfo VK_VertexDescribe::SetCreateInfo( const vertex::Info& info )
		{
			// 预分配 Binding & Attribute 数量
			this->arrBindings.reserve( info.Count() );

			this->arrAttributes.reserve( info.CountElements() );

			// 添加 Binding group
			this->AddBindings( info.arrBindings );

			// 设置 Binding create info
			return VkPipelineVertexInputStateCreateInfo
					{
						.sType							 = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
						.pNext							 = nullptr,
						.flags							 = 0,
						.vertexBindingDescriptionCount	 = static_cast< uint32_t >( this->arrBindings.size() ),
						.pVertexBindingDescriptions		 =							this->arrBindings.data(),
						.vertexAttributeDescriptionCount = static_cast< uint32_t >( this->arrAttributes.size() ),
						.pVertexAttributeDescriptions	 =							this->arrAttributes.data()
					};
		}


	/*
	 @	添加 Binding group
	 */
	FC_INLINE void VK_VertexDescribe::AddBindings( const stl::vector< vertex::Binding >& arrBindings )
		{
			// Binding 数量
			const uint32_t count
				{
					static_cast< uint32_t >( arrBindings.size() )
				};

			// 遍历 Binding group
			for( uint32_t i{ 0 }; i < count; ++i )
				{
					// stride
					const uint32_t stride
						{
							// 添加 这个 Binding 下的 Attribute group
							this->AddAttributes( i, arrBindings[ i ] )
						};

					// 添加 Binding 到 array
					this->arrBindings.emplace_back
						(
							VkVertexInputBindingDescription
								{
									.binding	= i,
									.stride		= stride,
									.inputRate	= VK_VERTEX_INPUT_RATE_VERTEX
								}
						);
				}
		}


	/*
	 @	添加 Attribute group
	 */
	FC_INLINE uint32_t VK_VertexDescribe::AddAttributes( const uint32_t idx,
														 const vertex::Binding& bingding )
		{
			uint32_t offset{ 0 };

			// 元素数量
			const uint32_t count
				{
					static_cast< uint32_t >( bingding.size() )
				};

			// 遍历 所有元素
			for( uint32_t i{ 0 }; i < count; ++i )
				{
					// 获取 TYPE
					const TYPE type
						{
							bingding[ i ]
						};

					// 添加 attribute
					this->arrAttributes.emplace_back
						(
							VkVertexInputAttributeDescription
								{
									.location	= i,
									.binding	= idx,
									.format		= VertexBuffer::Convert( type ),
									.offset		= offset,
								}
						);

					// 推移 offset
					offset += sizeOf[ type ];
				}

			// 返回作为 stride
			return offset;
		}
}



/**
 !	VK_Viewport		=>	视口 & 裁剪
 */
namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_Viewport::VK_Viewport( const stl::vector< Scissor >& arrScissors,
										const stl::vector< Viewport >& arrViewports ) noexcept
		:
			// 先转换 vkScissors 和 vkViewports
			vkScissors
				(
					stl::transform< decltype( vkScissors ) >( arrScissors )
				),

			vkViewports
				(
					stl::transform< decltype( vkViewports ) >( arrViewports )
				),

			// 再设置 create info
			createInfo
				{
					.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
					.pNext			= nullptr,
					.flags			= 0,
					.viewportCount	= static_cast< uint32_t >( this->vkViewports.size() ),
					.pViewports		=						   this->vkViewports.data(),
					.scissorCount	= static_cast< uint32_t >( this->vkScissors.size() ),
					.pScissors		=						   this->vkScissors.data()
				}
		{
		}
}



/**
 !	VK_DynamicState		=>	动态状态
 */
namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_DynamicState::VK_DynamicState( const stl::vector< renderPipeline::DYNAMIC_STATE >& arrDynStates )
		:
			// 先转换 vkDynamicStates
			vkDynamicStates
				(
					stl::transform( arrDynStates, RenderPipeline::Convert )
				),

			// 再设置 create info
			createInfo
				{
					.sType				= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
					.pNext				= nullptr,
					.flags				= 0,
					.dynamicStateCount	= static_cast< uint32_t >( this->vkDynamicStates.size() ),
					.pDynamicStates		=						   this->vkDynamicStates.data()
				}
		{
		}
}



/**
 !	VK_DepthStencil		=>	深度模板
 */
namespace fce::inner::depthStencil
{
	/*
	 @	转换
	 */
	FC_INLINE Stencil::operator VkStencilOpState() const
		{
			return VkStencilOpState
					{
						.failOp			= DepthStencil::Convert( this->failOp ),
						.passOp			= DepthStencil::Convert( this->passOp ),
						.depthFailOp	= DepthStencil::Convert( this->depthFailOp ),
						.compareOp		= Graphics::Convert( this->compareOp ),
						.compareMask	= this->compareMask,
						.writeMask		= this->writeMask,
						.reference		= this->reference
					};
		}

	/*
	 @	转换
	 */
	FC_INLINE Info::operator VkPipelineDepthStencilStateCreateInfo() const
		{
			return VkPipelineDepthStencilStateCreateInfo
					{
						.sType					= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
						.pNext					= nullptr,
						.flags					= 0,
						.depthTestEnable		= this->enableDepth,
						.depthWriteEnable		= this->enableWrite,
						.depthCompareOp			= Graphics::Convert( this->compare ),
						.depthBoundsTestEnable	= this->enableBounds,
						.stencilTestEnable		= this->enableStencil,
						.front					= this->front,
						.back					= this->back,
						.minDepthBounds			= this->minDepth,
						.maxDepthBounds			= this->maxDepth
					};
		}
}



/**
 !	VK_PipelineLayout	=>	管道布局
 */
namespace fce::inner::vulkan
{
	/*
	 @	析构
	 */
	VK_PipelineLayout::~VK_PipelineLayout() noexcept
		{
			// 销毁 layout
			if( this->vkPipelineLayout )
				{
					vkDestroyPipelineLayout( s_VK_Device,
											 this->vkPipelineLayout,
											 s_VK_Allocator );
				}
		}

	/*
	 @	构造
	 */
	VK_PipelineLayout::VK_PipelineLayout( RESULT& result,
										  const VkDescriptorSetLayout vkDescriptorSetLayout,
										  const stl::vector< pushConstant::Info >& arrPushConstants )
		:
			vkPipelineLayout( VK_NULL_HANDLE )
		{
			// 转换 VkPushConstantRange 数组
			stl::vector< VkPushConstantRange > arrPushConstantRanges;

			Convert( arrPushConstants, arrPushConstantRanges );

			// 设置 create info
			const VkPipelineLayoutCreateInfo createInfo
				{
					.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.setLayoutCount			= 1,
					.pSetLayouts			= &vkDescriptorSetLayout,
					.pushConstantRangeCount	= static_cast< uint32_t >( arrPushConstantRanges.size() ),
					.pPushConstantRanges	=						   arrPushConstantRanges.data(),
				};

			// 创建 Pipeline layout
			VkResult res;

			VK_ERR_GUARD( vkCreatePipelineLayout( s_VK_Device,
												  &createInfo,
												  s_VK_Allocator,
												  &this->vkPipelineLayout ) );

			result = s_VK_Graphics.Convert( res );
		}


	/*
	 @	转换 PushConstant list
	 */
	FC_INLINE void VK_PipelineLayout::Convert( const stl::vector< pushConstant::Info >& arrPushConstants,
													 stl::vector< VkPushConstantRange >& arrPushConstantRanges )
		{
			uint32_t offset{ 0 };

			// 转换 pushConstant::Info 到 VkPushConstantRange
			auto f = [ &offset ] ( const pushConstant::Info& info )
				{
					const VkPushConstantRange vkPushConstantRange
						{
							.stageFlags = info.GetStage(),
							.offset = offset,
							.size = info.size
						};

					// 推移 offset
					offset += info.size;

					return vkPushConstantRange;
				};

			stl::transform( arrPushConstants,
							arrPushConstantRanges, f );
		}


	/*
	 @	Get native handle
	 */
	FC_INLINE const VkPipelineLayout VK_PipelineLayout::GetHandle() const noexcept
		{
			return this->vkPipelineLayout;
		}
}



/**
 !	VK_RenderPipeline	=>	渲染管道
 */
namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_RenderPipeline::VK_RenderPipeline() noexcept
		:
			vkPipeline( VK_NULL_HANDLE ),
			vkPipelineLayout( VK_NULL_HANDLE )
		{
		}

	/*
	 @	释放
	 */
	FC_INLINE void VK_RenderPipeline::Release() noexcept
		{
			// 销毁 pipe line
			if( this->vkPipeline )
				{
					vkDestroyPipeline( s_VK_Device,
									   this->vkPipeline,
									   s_VK_Allocator );
				}
		}


	/*
	 @	初始化
	 */
	FC_INLINE VkResult VK_RenderPipeline::Initialize( const renderPipeline::Info& info,
													  const RenderPipeline& superior )
		{
			VkResult res;

			// 设置 infos
			const VK_ShaderProgram	vk_shaderProgram	( info.arrShaders		);
			const VK_VertexDescribe	vk_vertexDescribe	( info.vertexDescribe	);
			const VK_ColorBlend		vk_colorBlend		( info.colorBlend		);
			const VK_Multisample	vk_multisample		( info.multisample		);
			const VK_Rasterization	vk_rasterization	( info.rasterization	);
			const VK_Viewport		vk_viewport			( info.arrScissors,
														  info.arrViewports		);

			// 动态 infos
			const Raw_DynamicState	raw_dynamicState	( info.arrDynamicStates );
			const Raw_DepthStencil	raw_depthStencil	( info.depthStencil		);

			// 设置 create info
			const VkGraphicsPipelineCreateInfo createInfo
				{
					.sType				 = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
					.pNext				 = nullptr,
					.flags				 = 0,
					.stageCount			 = vk_shaderProgram.Count(),
					.pStages			 = vk_shaderProgram.Data(),
					.pVertexInputState	 = &vk_vertexDescribe.bindingCreateInfo,
					.pInputAssemblyState = &vk_vertexDescribe.assemblyCreateInfo,
					.pTessellationState	 = nullptr,
					.pViewportState		 = &vk_viewport.createInfo,
					.pRasterizationState = &vk_rasterization.createInfo,
					.pMultisampleState	 = &vk_multisample.createInfo,
					.pDepthStencilState	 = raw_depthStencil.Get(),
					.pColorBlendState	 = &vk_colorBlend.createInfo,
					.pDynamicState		 = raw_dynamicState ? &raw_dynamicState->createInfo : nullptr,
					.layout				 = this->RequestLayout( info, superior ),
					.renderPass			 = superior.renderPass->impl,
					.subpass			 = 0,
					.basePipelineHandle	 = VK_NULL_HANDLE,
					.basePipelineIndex	 = -1
				};

			// 创建 graphics pipelines
			VK_ERR_GUARD( vkCreateGraphicsPipelines( s_VK_Device,
													 VK_NULL_HANDLE,
													 1, &createInfo,
													 s_VK_Allocator,
													 &this->vkPipeline ) );

			return res;
		}


	/*
	 @	获取 VkPipelineLayout
	 */
	FC_INLINE VkPipelineLayout VK_RenderPipeline::RequestLayout( const renderPipeline::Info& info,
																 const RenderPipeline& superior )
		{
			// 请求 VK_PipelineLayout
			const VK_PipelineLayout* vk_pipelineLayout
				{
					VK_PipelineLayout::Request( superior.descriptorPool->impl.GetLayout(),
												info.arrPushConstants )
				};

			// 保存 layout 句柄
			FC_ASSERT( vk_pipelineLayout );

			return this->vkPipelineLayout = vk_pipelineLayout->GetHandle();
		}
}