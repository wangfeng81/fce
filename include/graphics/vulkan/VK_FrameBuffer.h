/*
**
**	VK_FrameBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_FrameBuffer
	 */
	class FC_API VK_FrameBuffer final
		{
			/*	Friends
			*/
			friend class VK_FrameChain;

			/*	List types
			*/
			using ViewList = VK_SwapChain::ViewList;

		public:
			/*	Constructor & Destructor
			*/
			~VK_FrameBuffer() noexcept;

			VK_FrameBuffer( VkResult&,
							const ViewList&,
							const VkExtent2D&,
							const VkRenderPass ) noexcept;

			/*	Get VkFramebuffer
			*/
			[[nodiscard]] operator const VkFramebuffer() const noexcept;

		protected:
			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const ViewList&,
								 const VkExtent2D&,
								 const VkRenderPass ) noexcept;

		private:
			/*	VkFramebuffer
			*/
			VkFramebuffer vkFramebuffer;
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_FrameChain
	 */
	class FC_API VK_FrameChain final
		{
			/*	Friends
			*/
			friend VK_WindowSurface;

		public:
			/*	Visit VkFramebuffer array
			*/
			[[nodiscard]] const VkFramebuffer operator [] ( const uint32_t ) const NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_FrameChain );

			~VK_FrameChain() = default;

			VK_FrameChain() noexcept;

			/*	Create & Destroy
			*/
			void Destroy() noexcept;

			void DestroyDepthStencil() noexcept;

			void DestroyFrameBuffers() noexcept;

			VkResult CreateCommon( const VK_SwapChain& ) noexcept;

			VkResult CreateCompatible( const VK_SwapChain& ) noexcept;

		private:
			/*	RenderPass
			*/
			RenderPass* renderPass;

			/*	DepthStencil
			*/
			DepthStencil* depthStencil;

			/*	VkFramebuffer array
			*/
			stl::vector< VkFramebuffer > arrFrameBuffers;
		};
}



/**
 !	class VK_FrameBuffer
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkFramebuffer
	 */
	FC_INLINE VK_FrameBuffer::operator const VkFramebuffer() const noexcept
		{
			return this->vkFramebuffer;
		}

	/*
	 @	Visit VkFramebuffer array
	 */
	FC_INLINE const VkFramebuffer VK_FrameChain::operator [] ( const uint32_t idx ) const NOEXCEPT
		{
			return this->arrFrameBuffers[ idx ];
		}
}