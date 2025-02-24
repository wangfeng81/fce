/*
**
**	VK_VertexBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_VertexBuffer
	 */
	class FC_API VK_VertexBuffer final
		{
			/*	Friends
			*/
			friend VertexBuffer;

		public:
			/*	Get VkBuffer
			*/
			[[nodiscard]] operator const VkBuffer() const noexcept;

			[[nodiscard]] const VkBuffer& GetHandle() const noexcept;

			/*	Use
			*/
			void Use( const VkCommandBuffer,
					  const VkDeviceSize offset = 0 ) const noexcept;

			/*	Update
			*/
			VkResult Update( const VK_TempBuffer&,
							 const VkDeviceSize size,
							 const VkDeviceSize srcOffset,
							 const VkDeviceSize dstOffset ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_VertexBuffer );

			~VK_VertexBuffer() = default;

			VK_VertexBuffer() noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const VkDeviceSize ) noexcept;

		private:
			/*	VkBuffer
			*/
			VkBuffer vkBuffer;

			/*	VmaAllocation
			*/
			VmaAllocation vmaAlloc;

			/*	Primitive list
			*/
			static constexpr VkPrimitiveTopology PRIMITIVE_LIST[]
				{
					VK_PRIMITIVE_TOPOLOGY_POINT_LIST,

					VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
					VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,

					VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
					VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
					VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,

					VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
					VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
					VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
					VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
					VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
				};

			/*	Format list
			*/
			static constexpr VkFormat TYPE_LIST[]
				{
					VK_FORMAT_R8_UINT,

					VK_FORMAT_R8_UINT,
					VK_FORMAT_R16_UINT,
					VK_FORMAT_R8_UINT,
					VK_FORMAT_R16_UINT,
					VK_FORMAT_R32_UINT,

					VK_FORMAT_R8_SINT,
					VK_FORMAT_R8_UINT,
					VK_FORMAT_R16_SINT,
					VK_FORMAT_R16_UINT,
					VK_FORMAT_R32_SINT,
					VK_FORMAT_R32_UINT,
					VK_FORMAT_R64_SINT,
					VK_FORMAT_R64_UINT,
					VK_FORMAT_R32_SFLOAT,
					VK_FORMAT_R64_SFLOAT,

					VK_FORMAT_R8G8_UINT,
					VK_FORMAT_R8G8B8_UINT,
					VK_FORMAT_R8G8B8A8_UINT,

					VK_FORMAT_R8G8_SINT,
					VK_FORMAT_R8G8_UINT,
					VK_FORMAT_R16G16_SINT,
					VK_FORMAT_R16G16_UINT,
					VK_FORMAT_R32G32_SINT,
					VK_FORMAT_R32G32_UINT,
					VK_FORMAT_R64G64_SINT,
					VK_FORMAT_R64G64_UINT,
					VK_FORMAT_R32G32_SFLOAT,
					VK_FORMAT_R64G64_SFLOAT,

					VK_FORMAT_R8G8B8_SINT,
					VK_FORMAT_R8G8B8_UINT,
					VK_FORMAT_R16G16B16_SINT,
					VK_FORMAT_R16G16B16_UINT,
					VK_FORMAT_R32G32B32_SINT,
					VK_FORMAT_R32G32B32_UINT,
					VK_FORMAT_R64G64B64_SINT,
					VK_FORMAT_R64G64B64_UINT,
					VK_FORMAT_R32G32B32_SFLOAT,
					VK_FORMAT_R64G64B64_SFLOAT,

					VK_FORMAT_R8G8B8A8_SINT,
					VK_FORMAT_R8G8B8A8_UINT,
					VK_FORMAT_R16G16B16A16_SINT,
					VK_FORMAT_R16G16B16A16_UINT,
					VK_FORMAT_R32G32B32A32_SINT,
					VK_FORMAT_R32G32B32A32_UINT,
					VK_FORMAT_R64G64B64A64_SINT,
					VK_FORMAT_R64G64B64A64_UINT,
					VK_FORMAT_R32G32B32A32_SFLOAT,
					VK_FORMAT_R64G64B64A64_SFLOAT,

					VK_FORMAT_MAX_ENUM
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::vertexBuffer
{
	/*
	 @	VertexBuffer vulkan impl
	 */
	using Impl = vulkan::VK_VertexBuffer;
}



/**
 !	class VK_VertexBuffer
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get vkBuffer
	 */
	FC_INLINE VK_VertexBuffer::operator const VkBuffer() const noexcept
		{
			return this->vkBuffer;
		}

	FC_INLINE const VkBuffer& VK_VertexBuffer::GetHandle() const noexcept
		{
			return this->vkBuffer;
		}

	/*
	 @	Bind vertex buffer
	 */
	FC_INLINE void VK_VertexBuffer::Use( const VkCommandBuffer vkCommandBuffer,
										 const VkDeviceSize offset ) const noexcept
		{
			vkCmdBindVertexBuffers( vkCommandBuffer,
									0, 1,
									&this->vkBuffer,
									&offset );
		}

	/*
	 @	Update
	 */
	FC_INLINE VkResult VK_VertexBuffer::Update( const VK_TempBuffer& tempBuffer,
												const VkDeviceSize size,
												const VkDeviceSize srcOffset,
												const VkDeviceSize dstOffset ) NOEXCEPT
		{
			return tempBuffer.Copy( this->vkBuffer, size,
													srcOffset,
													dstOffset );
		}
}