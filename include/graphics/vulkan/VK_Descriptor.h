/*
**
**	VK_Descriptor.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/


namespace fce::inner::vulkan
{
	/*
	 !	VK_Descriptor
	 */
	class FC_API VK_Descriptor final
		{
			/*	Friends
			*/
			friend Descriptor;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Descriptor );

			~VK_Descriptor() = default;

			VK_Descriptor() = default;

		private:
			/*	Descriptor type list
			*/
			static constexpr VkDescriptorType TYPE_LIST[]
				{
					VK_DESCRIPTOR_TYPE_SAMPLER,
					VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
					VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
					VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
					VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
					VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
					VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK,
					VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
					VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV,
					VK_DESCRIPTOR_TYPE_MUTABLE_VALVE,
					VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT
				};
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_DescriptorSet
	 */
	class FC_API VK_DescriptorSet final
		{
			/*	Friends
			*/
			friend DescriptorSet;

		public:
			/*	VkDescriptorSet
			*/
			[[nodiscard]] operator const VkDescriptorSet() const noexcept;

			/*	Use
			*/
			void Use( const VkCommandBuffer,
					  const VkPipelineLayout ) const noexcept;

			/*	Update one
			*/
			void Update( const VK_Texture2&,
						 const uint32_t binding,
						 const uint32_t arrayIndex ) NOEXCEPT;

			void Update( const VK_UniformBuffer&,
						 const uint32_t binding,
						 const uint32_t arrayIndex ) NOEXCEPT;

			/*	Update a binding group
			*/
			template< std::ranges::range Range >

			void Update( const Range&,
						 const uint32_t binding ) NOEXCEPT;

			/*	Update all
			*/
			template< std::size_t ...I,
					  std::ranges::range ...Args >

			void Update( const std::index_sequence< I... >,
						 const Args&... ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_DescriptorSet );

			~VK_DescriptorSet() = default;

			VK_DescriptorSet( const VkDescriptorSet ) noexcept;

		private:
			/*	Free infos
			*/
			void FreeInfos( const VkWriteDescriptorSet*,
							const uint32_t size ) noexcept;

			/*	Make Texture2 array writes
			*/
			VkWriteDescriptorSet MakeWrites( const uint32_t binding,
											 const stl::vector< const Texture2* >& ) NOEXCEPT;

			/*	Make UniformBuffer array writes
			*/
			VkWriteDescriptorSet MakeWrites( const uint32_t binding,
											 const stl::vector< const UniformBuffer* >& ) NOEXCEPT;

			/*	Make image infos
			*/
			const VkDescriptorImageInfo* MakeInfos( const stl::vector< const Texture2* >&,
													const uint32_t size ) NOEXCEPT;

			/*	Make buffer infos
			*/
			const VkDescriptorBufferInfo* MakeInfos( const stl::vector< const UniformBuffer* >&,
													 const uint32_t size ) NOEXCEPT;

		private:
			/*	Descriptor set
			*/
			VkDescriptorSet vkDescriptorSet;
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_DescriptorPool
	 */
	class FC_API VK_DescriptorPool final
		{
			/*	Friends
			*/
			friend DescriptorPool;

			using Info = stl::vector< descriptor::Info >;

		public:
			/*	VkDescriptorPool
			*/
			[[nodiscard]] operator const VkDescriptorPool() const noexcept;

			/*	Get VkDescriptorSetLayout
			*/
			[[nodiscard]] const VkDescriptorSetLayout& GetLayout() const noexcept;

			/*	Allocate & Free
			*/
			VkDescriptorSet Allocate() noexcept;

			VkResult Free( VkDescriptorSet ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_DescriptorPool );

			~VK_DescriptorPool() = default;

			VK_DescriptorPool() noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const Info& ) noexcept;

		private:
			/*	Convert
			*/
			static void Convert( const uint32_t,
								 const descriptor::Info&,
								 VkDescriptorSetLayoutBinding& ) noexcept;

			static void Convert( const VkDescriptorSetLayoutBinding&,
								 VkDescriptorPoolSize& ) noexcept;

		private:
			/*	VkDescriptorSetLayout
			*/
			VkDescriptorSetLayout vkDescriptorSetLayout;

			/*	VkDescriptorPool
			*/
			VkDescriptorPool vkDescriptorPool;
		};
}



/*
 !	Implementation
 */
namespace fce::inner::descriptor
{
	/*
	 @	VK_Descriptor vulkan impl
	 */
	using Impl = vulkan::VK_Descriptor;
}

namespace fce::inner::descriptorSet
{
	/*
	 @	VK_DescriptorSet vulkan impl
	 */
	using Impl = vulkan::VK_DescriptorSet;
}

namespace fce::inner::descriptorPool
{
	/*
	 @	VK_DescriptorPool vulkan impl
	 */
	using Impl = vulkan::VK_DescriptorPool;
}



/**
 !	class VK_DescriptorSet
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkDescriptorSet
	 */
	FC_INLINE VK_DescriptorSet::operator const VkDescriptorSet() const noexcept
		{
			return this->vkDescriptorSet;
		}

	/*
	 @	Bind
	 */
	FC_INLINE void VK_DescriptorSet::Use( const VkCommandBuffer vkCommandBuffer,
										  const VkPipelineLayout vkPipelineLayout ) const noexcept
		{
			vkCmdBindDescriptorSets( vkCommandBuffer,
									 VK_PIPELINE_BIND_POINT_GRAPHICS,
									 vkPipelineLayout,
									 0, 1,
									 &this->vkDescriptorSet,
									 0, nullptr );
		}
}



namespace fce::inner::vulkan
{
	/*	Update a binding group
	*/
	template< std::ranges::range Range >

	FC_INLINE void VK_DescriptorSet::Update( const Range& range,
											 const uint32_t binding ) NOEXCEPT
		{
			const VkWriteDescriptorSet vkWrites
				{
					this->MakeWrites( binding, range )
				};

			vkUpdateDescriptorSets( s_VK_Device,
									1,
									&vkWrites,
									0, nullptr );
		}

	/*	Update all
	*/
	template< std::size_t ...I,
			  std::ranges::range ...Args >

	FC_INLINE void VK_DescriptorSet::Update( const std::index_sequence< I... >,
											 const Args& ...args ) NOEXCEPT
		{
			constexpr uint32_t size
				{
					uint32_t( sizeof...( Args ) )
				};

			const VkWriteDescriptorSet vkWrites[ size ]
				{
					this->MakeWrites( I, args )...
				};

			vkUpdateDescriptorSets( s_VK_Device,
									size,
									vkWrites,
									0, nullptr );

			this->FreeInfos( vkWrites, size );
		}
}



/**
 !	class VK_DescriptorPool
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkDescriptorPool
	 */
	FC_INLINE VK_DescriptorPool::operator const VkDescriptorPool() const noexcept
		{
			return this->vkDescriptorPool;
		}

	/*
	 @	Get VkDescriptorSetLayout
	 */
	FC_INLINE const VkDescriptorSetLayout& VK_DescriptorPool::GetLayout() const noexcept
		{
			return this->vkDescriptorSetLayout;
		}
}