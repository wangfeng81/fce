/*
**
**	VK_IndexBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_IndexBuffer
	 */
	class FC_API VK_IndexBuffer final
		{
			/*	Friends
			*/
			friend IndexBuffer;

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
			DISABLE_COPY_MOVE( VK_IndexBuffer );

			~VK_IndexBuffer() = default;

			VK_IndexBuffer( const VkIndexType ) noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const VkDeviceSize ) noexcept;

		private:
			/*	Choose type
			*/
			[[nodiscard]] static VkIndexType Convert( const TYPE ) noexcept;

		private:
			/*	VkBuffer
			*/
			VkBuffer vkBuffer;

			/*	VkIndexType
			*/
			VkIndexType vkType;

			/*	VmaAllocation
			*/
			VmaAllocation vmaAlloc;
		};
}



/*
 !	Implementation
 */
namespace fce::inner::indexBuffer
{
	/*
	 @	IndexBuffer vulkan impl
	 */
	using Impl = vulkan::VK_IndexBuffer;
}



/**
 !	class VK_IndexBuffer
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get vkBuffer
	 */
	FC_INLINE VK_IndexBuffer::operator const VkBuffer() const noexcept
		{
			return this->vkBuffer;
		}

	FC_INLINE const VkBuffer& VK_IndexBuffer::GetHandle() const noexcept
		{
			return this->vkBuffer;
		}

	/*
	 @	Use
	 */
	FC_INLINE void VK_IndexBuffer::Use( const VkCommandBuffer vkCommandBuffer,
										const VkDeviceSize offset ) const noexcept
		{
			vkCmdBindIndexBuffer( vkCommandBuffer,
								  this->vkBuffer,
								  offset,
								  this->vkType );
		}

	/*
	 @	Update
	 */
	FC_INLINE VkResult VK_IndexBuffer::Update( const VK_TempBuffer& tempBuffer,
											   const VkDeviceSize size,
											   const VkDeviceSize srcOffset,
											   const VkDeviceSize dstOffset ) NOEXCEPT
		{
			return tempBuffer.Copy( this->vkBuffer, size,
													srcOffset,
													dstOffset );
		}
}