/*
**
**	VK_Descriptor.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/


/**
 !	VK_DescriptorSet
 */
namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_DescriptorSet::VK_DescriptorSet( const VkDescriptorSet vkDescriptorSet ) noexcept
		:
			vkDescriptorSet( vkDescriptorSet )
		{
		}


	/*
	 @	释放 表单信息
	 */
	void VK_DescriptorSet::FreeInfos( const VkWriteDescriptorSet* vkWrites,
									  const uint32_t size ) noexcept
		{
			// 遍历 vkWrites
			for( uint32_t c{ 0 }; c < size; ++c )
				{
					// 释放 pBufferInfo
					if( vkWrites->pBufferInfo )
						{
							s_MemMgmt.Free( const_cast< VkDescriptorBufferInfo* >( vkWrites->pBufferInfo ),
																				   vkWrites->descriptorCount );
						}

					// 释放 pImageInfo
					if( vkWrites->pImageInfo )
						{
							s_MemMgmt.Free( const_cast< VkDescriptorImageInfo* >( vkWrites->pImageInfo ),
																				  vkWrites->descriptorCount );
						}

					// 下一个
					++vkWrites;
				}
		}


	/*
	 @	更新 单个 Texture2 绑定

			arrayIndex 起始位置
	 */
	void VK_DescriptorSet::Update( const VK_Texture2& texture2 ,
								   const uint32_t binding,
								   const uint32_t arrayIndex ) NOEXCEPT
		{
			const VkDescriptorImageInfo imageInfo
				{
					.sampler		= texture2.GetSampler(),
					.imageView		= texture2,
					.imageLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				};

			// 设置 vkWrites
			const VkWriteDescriptorSet vkWrites
				{
					.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.pNext				= nullptr,
					.dstSet				= this->vkDescriptorSet,
					.dstBinding			= binding,
					.dstArrayElement	= arrayIndex,
					.descriptorCount	= 1,
					.descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.pImageInfo			= &imageInfo,
					.pBufferInfo		= nullptr,
					.pTexelBufferView	= nullptr
				};

			vkUpdateDescriptorSets( s_VK_Device,
									1,
									&vkWrites,
									0, nullptr );
		}


	/*
	 @	更新 单个 UniformBuffer 绑定

			arrayIndex 起始位置
	 */
	void VK_DescriptorSet::Update( const VK_UniformBuffer& ubo ,
								   const uint32_t binding,
								   const uint32_t arrayIndex ) NOEXCEPT
		{
			const VkDescriptorBufferInfo bufferInfo
				{
					.buffer	= ubo,
					.offset	= 0,
					.range	= ubo.GeSize()
				};

			// 设置 vkWrites
			const VkWriteDescriptorSet vkWrites
				{
					.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.pNext				= nullptr,
					.dstSet				= this->vkDescriptorSet,
					.dstBinding			= binding,
					.dstArrayElement	= arrayIndex,
					.descriptorCount	= 1,
					.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					.pImageInfo			= nullptr,
					.pBufferInfo		= &bufferInfo,
					.pTexelBufferView	= nullptr
				};

			vkUpdateDescriptorSets( s_VK_Device,
									1,
									&vkWrites,
									0, nullptr );
		}


	/*
	 @	制作 Texture2 绑定组 表单

			arrayIndex 起始位置
	 */
	VkWriteDescriptorSet VK_DescriptorSet::MakeWrites( const uint32_t binding,
													   const stl::vector< const Texture2* >& ilistTexture2 ) NOEXCEPT
		{
			// 纹理 数量
			const uint32_t size
				{
					static_cast< uint32_t >( ilistTexture2.size() )
				};

			// 设置 vkWrites
			return VkWriteDescriptorSet
					{
						.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						.pNext				= nullptr,
						.dstSet				= this->vkDescriptorSet,
						.dstBinding			= binding,
						.dstArrayElement	= 0,
						.descriptorCount	= size,
						.descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
						.pImageInfo			= this->MakeInfos( ilistTexture2, size ),
						.pBufferInfo		= nullptr,
						.pTexelBufferView	= nullptr
					};
		}


	/*
	 @	制作 UniformBuffer 绑定组 表单

			arrayIndex 起始位置
	 */
	VkWriteDescriptorSet VK_DescriptorSet::MakeWrites( const uint32_t binding,
													   const stl::vector< const UniformBuffer* >& ilistUBO ) NOEXCEPT
		{
			// Ubo 数量
			const uint32_t size
				{
					static_cast< uint32_t >( ilistUBO.size() )
				};

			// 设置 vkWrites
			return VkWriteDescriptorSet
					{
						.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
						.pNext				= nullptr,
						.dstSet				= this->vkDescriptorSet,
						.dstBinding			= binding,
						.dstArrayElement	= 0,
						.descriptorCount	= size,
						.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
						.pImageInfo			= nullptr,
						.pBufferInfo		= this->MakeInfos( ilistUBO, size ),
						.pTexelBufferView	= nullptr
					};
		}


	/*
	 @	制作 image info 表单
	 */
	FC_INLINE const VkDescriptorImageInfo* VK_DescriptorSet::MakeInfos( const stl::vector< const Texture2* >& ilistTexture2,
																		const uint32_t size ) NOEXCEPT
		{
			// 分配 ImageInfo 数组
			VkDescriptorImageInfo* arrImageInfo
				{
					s_MemMgmt.Malloc< VkDescriptorImageInfo >( size )
				};

			// 设置 ImageInfo 数组
			VkDescriptorImageInfo* imageInfo{ arrImageInfo };

			for( const Texture2* texture2 : ilistTexture2 )
				{
					FC_ASSERT( texture2 );

					// 设置 image info 函数式
					auto MakeImageInfo = []( const VK_Texture2& texture2 )
						{
							return VkDescriptorImageInfo
									{
										.sampler		= texture2.GetSampler(),
										.imageView		= texture2,
										.imageLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
									};
						};

					// 赋值 image info
					*imageInfo++ = MakeImageInfo( texture2->impl );
				}

			return arrImageInfo;
		}


	/*
	 @	制作 buffer info 表单
	 */
	FC_INLINE const VkDescriptorBufferInfo* VK_DescriptorSet::MakeInfos( const stl::vector< const UniformBuffer* >& ilistUBO,
																		 const uint32_t size ) NOEXCEPT
		{
			// 分配 BufferInfo 数组
			VkDescriptorBufferInfo* arrBufferInfo
				{
					s_MemMgmt.Malloc< VkDescriptorBufferInfo >( size )
				};

			// 设置 BufferInfo 数组
			VkDescriptorBufferInfo* bufferInfo{ arrBufferInfo };

			for( const UniformBuffer* ubo : ilistUBO )
				{
					FC_ASSERT( ubo );

					// 设置 buffer info 函数式
					auto MakeBufferInfo = []( const VK_UniformBuffer& ubo )
						{
							return VkDescriptorBufferInfo
									{
										.buffer	= ubo,
										.offset	= 0,
										.range	= ubo.GeSize()
									};
						};

					// 赋值 buffer info
					*bufferInfo++ = MakeBufferInfo( ubo->impl );
				}

			return arrBufferInfo;
		}
}



/**
 !	VK_DescriptorPool
 */
namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_DescriptorPool::VK_DescriptorPool() noexcept
		:
			vkDescriptorSetLayout( VK_NULL_HANDLE ),
			vkDescriptorPool( VK_NULL_HANDLE )
		{
		}

	/*
	 @	释放
	 */
	FC_INLINE void VK_DescriptorPool::Release() noexcept
		{
			// 销毁 VkDescriptorPool
			if( this->vkDescriptorPool )
				{
					vkDestroyDescriptorPool( s_VK_Device,
											 this->vkDescriptorPool,
											 s_VK_Allocator );
				}

			// 销毁 VkDescriptorSetLayout
			if( this->vkDescriptorSetLayout )
				{
					vkDestroyDescriptorSetLayout( s_VK_Device,
												  this->vkDescriptorSetLayout,
												  s_VK_Allocator );
				}
		}


	/*
	 @	初始化

			通过 布局 来创建 池
	 */
	FC_INLINE VkResult VK_DescriptorPool::Initialize( const Info& info ) noexcept
		{
			VkResult res;

			// 获取 Descriptor 数量
			const uint32_t count
				{
					static_cast< uint32_t >( info.size() )
				};

			// 用于 布局
			stl::vector< VkDescriptorSetLayoutBinding > arrBindings( count );

			// 用于 池
			stl::vector< VkDescriptorPoolSize > arrPoolSizes( count );

			// 转换 列表
			for( uint32_t i{ 0 }; i < count; ++i )
				{
					// 用于 布局
					Convert( i, info[ i ],
							 arrBindings[ i ] );

					// 用于 池
					Convert( arrBindings[ i ],
							 arrPoolSizes[ i ] );
				}

			// 设置 布局 create info
			const VkDescriptorSetLayoutCreateInfo createInfo
				{
					.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
					.pNext			= nullptr,
					.flags			= 0,
					.bindingCount	= count,
					.pBindings		= arrBindings.data()
				};

			// 创建 布局
			VK_ERR_GUARD( vkCreateDescriptorSetLayout( s_VK_Device,
													   &createInfo,
													   s_VK_Allocator,
													   &this->vkDescriptorSetLayout ) );

			// 如果 count = 0 创建池 可能存在问题
			if( res == VK_SUCCESS )
				{
					// 设置 池 create info
					const VkDescriptorPoolCreateInfo createInfo
						{
							.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
							.pNext			= nullptr,
							.flags			= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT, // 没有这个 无法单独 free 描述符集，这里存在可能的优化
							.maxSets		= 0x3000,	// 这里需要考虑优化
							.poolSizeCount	= count,
							.pPoolSizes		= arrPoolSizes.data()
						};

					// 创建 池
					VK_ERR_GUARD( vkCreateDescriptorPool( s_VK_Device,
														  &createInfo,
														  s_VK_Allocator,
														  &this->vkDescriptorPool ) );
				}

			return res;
		}


	/*
	 @	转换
	 */
	FC_INLINE void VK_DescriptorPool::Convert( const uint32_t index,
											   const descriptor::Info& descriptor,
											   VkDescriptorSetLayoutBinding& layoutBinding ) noexcept
		{
			layoutBinding =
				{
					.binding			= index,
					.descriptorType		= Descriptor::Convert( descriptor.type ),
					.descriptorCount	= descriptor.count,
					.stageFlags			= descriptor.GetStage(),
					.pImmutableSamplers	= nullptr
				};
		}

	FC_INLINE void VK_DescriptorPool::Convert( const VkDescriptorSetLayoutBinding& layoutBinding,
											   VkDescriptorPoolSize& poolSize ) noexcept
		{
			poolSize =
				{
					.type			 = layoutBinding.descriptorType,
					.descriptorCount = layoutBinding.descriptorCount
				};
		}


	/*
	 @	分配 描述符集
	 */
	FC_INLINE VkDescriptorSet VK_DescriptorPool::Allocate() noexcept
		{
			VkResult res;

			// 设置 allocate info
			const VkDescriptorSetAllocateInfo allocateInfo
				{
					.sType				 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
					.pNext				 = nullptr,
					.descriptorPool		 = this->vkDescriptorPool,
					.descriptorSetCount	 = 1,
					.pSetLayouts		 = &this->vkDescriptorSetLayout
				};

			// 分配 Descriptor set，有时需要和 交换链 数量一致
			VkDescriptorSet vkDescriptorSet{ VK_NULL_HANDLE };

			VK_ERR_GUARD( vkAllocateDescriptorSets( s_VK_Device,
													&allocateInfo,
													&vkDescriptorSet ) );

			return res == VK_SUCCESS
				   ? vkDescriptorSet
				   : VK_NULL_HANDLE;
		}


	/*
	 @	释放 描述符集合
	 */
	FC_INLINE VkResult VK_DescriptorPool::Free( VkDescriptorSet vkDescriptorSet ) NOEXCEPT
		{
			return vkFreeDescriptorSets( s_VK_Device,
										 this->vkDescriptorPool,
										 1, &vkDescriptorSet );
		}
}