/*
**
**	VK_Buffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_Buffer
	 */
	class FC_API VK_Buffer final
		{
			/*	Friends
			*/
			friend VK_Texture2;

			friend VK_IndexBuffer;

			friend VK_VertexBuffer;

		public:
			/*	Get base pointer
			*/
			template< typename T = Byte >

			[[nodiscard]] auto* Data( this auto& ) noexcept;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator bool() const noexcept;

			/*	Get VkBuffer
			*/
			[[nodiscard]] operator const VkBuffer() const noexcept;

			[[nodiscard]] const VkBuffer& GetHandle() const noexcept;

			/*	Copy buffer
			*/
			VkResult Copy( VkBuffer dstBuffer,
						   const VkDeviceSize size,
						   const VkDeviceSize srcOffset,
						   const VkDeviceSize dstOffset ) const NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Buffer );

			~VK_Buffer() noexcept;

			VK_Buffer( VK_Buffer&& ) noexcept;

			VK_Buffer( const VkDeviceSize ) NOEXCEPT;

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
		};
}



/**
 !	class VK_Buffer
 */
namespace fce::inner::vulkan
{
	/*
	 @	Check whether the buffer is valid
	 */
	FC_INLINE VK_Buffer::operator bool() const noexcept
		{
			return this->vkBuffer;
		}

	/*
	 @	Get vkBuffer
	 */
	FC_INLINE VK_Buffer::operator const VkBuffer() const noexcept
		{
			return this->vkBuffer;
		}

	FC_INLINE const VkBuffer& VK_Buffer::GetHandle() const noexcept
		{
			return this->vkBuffer;
		}
}