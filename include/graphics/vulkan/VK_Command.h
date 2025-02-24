/*
**
**	VK_Command.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_Command
	 */
	class FC_API VK_Command final
		{
			/*	Friends
			*/
			friend VK_Graphics;

		public:
			/*	Copy buffer
			*/
			VkResult CopyBuffer( VkBuffer src,
								 VkBuffer dst,
								 const VkBufferCopy& ) noexcept;

			/*	Copy image
			*/
			VkResult CopyImage( VkBuffer src,
								VkImage dst,
								const VkBufferImageCopy& ) noexcept;

			/*	VkCommandPool
			*/
			[[nodiscard]] operator const VkCommandPool() const noexcept;

			[[nodiscard]] operator const VkCommandBuffer() const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Command );

			~VK_Command() = default;

			VK_Command() noexcept;

			/*	Create & Destroy
			*/
			void Destroy() noexcept;

			VkResult Create() noexcept;

			/*	Begin & End transmission
			*/
			VkResult EndTransmission() noexcept;

			VkResult BeginTransmission() noexcept;

			/*	Allocate transmission command buffer
			*/
			VkResult AllocateTransBuffer() noexcept;

		private:
			/*	Command pool
			*/
			VkCommandPool vkCommandPool;

			/*	transfer
			*/
			VkCommandBuffer transCmdBuf;

			VkSubmitInfo transSubmitInfo;

			VkCommandBufferBeginInfo transCmdBufBegInfo;
		};


	/*
	 @	VK_Command singleton shortcut
	 */
	#define s_VK_Command s_Graphics.impl.command
}



/**
 !	class VK_Command
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkCommandPool
	 */
	FC_INLINE VK_Command::operator const VkCommandPool() const noexcept
		{
			return this->vkCommandPool;
		}

	/*
	 @	Get VkCommandPool
	 */
	FC_INLINE VK_Command::operator const VkCommandBuffer() const noexcept
		{
			return this->transCmdBuf;
		}
}