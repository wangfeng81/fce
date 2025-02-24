/*
**
**	VK_UniformBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_UniformBuffer
	 */
	class FC_API VK_UniformBuffer final
		{
			/*	Friends
			*/
			friend UniformBuffer;

		public:
			/*	Get VkBuffer
			*/
			[[nodiscard]] operator const VkBuffer() const noexcept;

			[[nodiscard]] const VkBuffer& GetHandle() const noexcept;

			/*	Get size
			*/
			[[nodiscard]] VkDeviceSize GeSize() const noexcept;

			/*	Update
			*/
			void Update( const Void* ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_UniformBuffer );

			~VK_UniformBuffer() = default;

			VK_UniformBuffer( VkDeviceSize ) noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize() noexcept;

		private:
			/*	VkBuffer
			*/
			VkBuffer vkBuffer;

			/*	VmaAllocation
			*/
			VmaAllocation vmaAlloc;

			/*	VkDeviceSize
			*/
			VkDeviceSize vkDeviceSize;
		};
}



/*
 !	Implementation
 */
namespace fce::inner::uniformBuffer
{
	/*
	 @	UniformBuffer vulkan impl
	 */
	using Impl = vulkan::VK_UniformBuffer;
}



/**
 !	class VK_UniformBuffer
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get vkBuffer
	 */
	FC_INLINE VK_UniformBuffer::operator const VkBuffer() const noexcept
		{
			return this->vkBuffer;
		}

	FC_INLINE const VkBuffer& VK_UniformBuffer::GetHandle() const noexcept
		{
			return this->vkBuffer;
		}

	/*
	 @	Get size
	 */
	FC_INLINE VkDeviceSize VK_UniformBuffer::GeSize() const noexcept
		{
			return this->vkDeviceSize;
		}
}