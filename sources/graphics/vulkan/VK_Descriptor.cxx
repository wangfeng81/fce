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
	 @	����
	 */
	FC_INLINE VK_DescriptorSet::VK_DescriptorSet( const VkDescriptorSet vkDescriptorSet ) noexcept
		:
			vkDescriptorSet( vkDescriptorSet )
		{
		}


	/*
	 @	�ͷ� ����Ϣ
	 */
	void VK_DescriptorSet::FreeInfos( const VkWriteDescriptorSet* vkWrites,
									  const uint32_t size ) noexcept
		{
			// ���� vkWrites
			for( uint32_t c{ 0 }; c < size; ++c )
				{
					// �ͷ� pBufferInfo
					if( vkWrites->pBufferInfo )
						{
							s_MemMgmt.Free( const_cast< VkDescriptorBufferInfo* >( vkWrites->pBufferInfo ),
																				   vkWrites->descriptorCount );
						}

					// �ͷ� pImageInfo
					if( vkWrites->pImageInfo )
						{
							s_MemMgmt.Free( const_cast< VkDescriptorImageInfo* >( vkWrites->pImageInfo ),
																				  vkWrites->descriptorCount );
						}

					// ��һ��
					++vkWrites;
				}
		}


	/*
	 @	���� ���� Texture2 ��

			arrayIndex ��ʼλ��
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

			// ���� vkWrites
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
	 @	���� ���� UniformBuffer ��

			arrayIndex ��ʼλ��
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

			// ���� vkWrites
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
	 @	���� Texture2 ���� ��

			arrayIndex ��ʼλ��
	 */
	VkWriteDescriptorSet VK_DescriptorSet::MakeWrites( const uint32_t binding,
													   const stl::vector< const Texture2* >& ilistTexture2 ) NOEXCEPT
		{
			// ���� ����
			const uint32_t size
				{
					static_cast< uint32_t >( ilistTexture2.size() )
				};

			// ���� vkWrites
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
	 @	���� UniformBuffer ���� ��

			arrayIndex ��ʼλ��
	 */
	VkWriteDescriptorSet VK_DescriptorSet::MakeWrites( const uint32_t binding,
													   const stl::vector< const UniformBuffer* >& ilistUBO ) NOEXCEPT
		{
			// Ubo ����
			const uint32_t size
				{
					static_cast< uint32_t >( ilistUBO.size() )
				};

			// ���� vkWrites
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
	 @	���� image info ��
	 */
	FC_INLINE const VkDescriptorImageInfo* VK_DescriptorSet::MakeInfos( const stl::vector< const Texture2* >& ilistTexture2,
																		const uint32_t size ) NOEXCEPT
		{
			// ���� ImageInfo ����
			VkDescriptorImageInfo* arrImageInfo
				{
					s_MemMgmt.Malloc< VkDescriptorImageInfo >( size )
				};

			// ���� ImageInfo ����
			VkDescriptorImageInfo* imageInfo{ arrImageInfo };

			for( const Texture2* texture2 : ilistTexture2 )
				{
					FC_ASSERT( texture2 );

					// ���� image info ����ʽ
					auto MakeImageInfo = []( const VK_Texture2& texture2 )
						{
							return VkDescriptorImageInfo
									{
										.sampler		= texture2.GetSampler(),
										.imageView		= texture2,
										.imageLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
									};
						};

					// ��ֵ image info
					*imageInfo++ = MakeImageInfo( texture2->impl );
				}

			return arrImageInfo;
		}


	/*
	 @	���� buffer info ��
	 */
	FC_INLINE const VkDescriptorBufferInfo* VK_DescriptorSet::MakeInfos( const stl::vector< const UniformBuffer* >& ilistUBO,
																		 const uint32_t size ) NOEXCEPT
		{
			// ���� BufferInfo ����
			VkDescriptorBufferInfo* arrBufferInfo
				{
					s_MemMgmt.Malloc< VkDescriptorBufferInfo >( size )
				};

			// ���� BufferInfo ����
			VkDescriptorBufferInfo* bufferInfo{ arrBufferInfo };

			for( const UniformBuffer* ubo : ilistUBO )
				{
					FC_ASSERT( ubo );

					// ���� buffer info ����ʽ
					auto MakeBufferInfo = []( const VK_UniformBuffer& ubo )
						{
							return VkDescriptorBufferInfo
									{
										.buffer	= ubo,
										.offset	= 0,
										.range	= ubo.GeSize()
									};
						};

					// ��ֵ buffer info
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
	 @	����
	 */
	FC_INLINE VK_DescriptorPool::VK_DescriptorPool() noexcept
		:
			vkDescriptorSetLayout( VK_NULL_HANDLE ),
			vkDescriptorPool( VK_NULL_HANDLE )
		{
		}

	/*
	 @	�ͷ�
	 */
	FC_INLINE void VK_DescriptorPool::Release() noexcept
		{
			// ���� VkDescriptorPool
			if( this->vkDescriptorPool )
				{
					vkDestroyDescriptorPool( s_VK_Device,
											 this->vkDescriptorPool,
											 s_VK_Allocator );
				}

			// ���� VkDescriptorSetLayout
			if( this->vkDescriptorSetLayout )
				{
					vkDestroyDescriptorSetLayout( s_VK_Device,
												  this->vkDescriptorSetLayout,
												  s_VK_Allocator );
				}
		}


	/*
	 @	��ʼ��

			ͨ�� ���� ������ ��
	 */
	FC_INLINE VkResult VK_DescriptorPool::Initialize( const Info& info ) noexcept
		{
			VkResult res;

			// ��ȡ Descriptor ����
			const uint32_t count
				{
					static_cast< uint32_t >( info.size() )
				};

			// ���� ����
			stl::vector< VkDescriptorSetLayoutBinding > arrBindings( count );

			// ���� ��
			stl::vector< VkDescriptorPoolSize > arrPoolSizes( count );

			// ת�� �б�
			for( uint32_t i{ 0 }; i < count; ++i )
				{
					// ���� ����
					Convert( i, info[ i ],
							 arrBindings[ i ] );

					// ���� ��
					Convert( arrBindings[ i ],
							 arrPoolSizes[ i ] );
				}

			// ���� ���� create info
			const VkDescriptorSetLayoutCreateInfo createInfo
				{
					.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
					.pNext			= nullptr,
					.flags			= 0,
					.bindingCount	= count,
					.pBindings		= arrBindings.data()
				};

			// ���� ����
			VK_ERR_GUARD( vkCreateDescriptorSetLayout( s_VK_Device,
													   &createInfo,
													   s_VK_Allocator,
													   &this->vkDescriptorSetLayout ) );

			// ��� count = 0 ������ ���ܴ�������
			if( res == VK_SUCCESS )
				{
					// ���� �� create info
					const VkDescriptorPoolCreateInfo createInfo
						{
							.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
							.pNext			= nullptr,
							.flags			= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT, // û����� �޷����� free ����������������ڿ��ܵ��Ż�
							.maxSets		= 0x3000,	// ������Ҫ�����Ż�
							.poolSizeCount	= count,
							.pPoolSizes		= arrPoolSizes.data()
						};

					// ���� ��
					VK_ERR_GUARD( vkCreateDescriptorPool( s_VK_Device,
														  &createInfo,
														  s_VK_Allocator,
														  &this->vkDescriptorPool ) );
				}

			return res;
		}


	/*
	 @	ת��
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
	 @	���� ��������
	 */
	FC_INLINE VkDescriptorSet VK_DescriptorPool::Allocate() noexcept
		{
			VkResult res;

			// ���� allocate info
			const VkDescriptorSetAllocateInfo allocateInfo
				{
					.sType				 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
					.pNext				 = nullptr,
					.descriptorPool		 = this->vkDescriptorPool,
					.descriptorSetCount	 = 1,
					.pSetLayouts		 = &this->vkDescriptorSetLayout
				};

			// ���� Descriptor set����ʱ��Ҫ�� ������ ����һ��
			VkDescriptorSet vkDescriptorSet{ VK_NULL_HANDLE };

			VK_ERR_GUARD( vkAllocateDescriptorSets( s_VK_Device,
													&allocateInfo,
													&vkDescriptorSet ) );

			return res == VK_SUCCESS
				   ? vkDescriptorSet
				   : VK_NULL_HANDLE;
		}


	/*
	 @	�ͷ� ����������
	 */
	FC_INLINE VkResult VK_DescriptorPool::Free( VkDescriptorSet vkDescriptorSet ) NOEXCEPT
		{
			return vkFreeDescriptorSets( s_VK_Device,
										 this->vkDescriptorPool,
										 1, &vkDescriptorSet );
		}
}