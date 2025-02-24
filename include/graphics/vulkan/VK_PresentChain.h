/*
**
**	VK_PresentChain.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_PresentChain
	 */
	class FC_API VK_PresentChain final
		{
			/*	Friends
			*/
			friend VK_WindowSurface;

			/*	Sync objects
			 */
			struct SyncObjects final
				{
					VkFence queueSubmited;

					VkSemaphore imageAcquired,
								renderFinished;
				};

		public:
			/*	Visit VkCommandBuffer array
			*/
			[[nodiscard]] const VkCommandBuffer operator [] ( const uint32_t ) const NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_PresentChain );

			~VK_PresentChain() = default;

			VK_PresentChain() noexcept;

			/*	Create & Destroy
			*/
			void Destroy() noexcept;

			VkResult Create( const VK_SwapChain& ) noexcept;

			/*	Create & Destroy sync object array
			*/
			void DestroySyncObjects() noexcept;

			VkResult CreateSyncObjects( const uint32_t ) noexcept;

			/*	Allocate & Free command buffer array
			*/
			void FreeCommandBuffers() noexcept;

			VkResult AllocateCommandBuffers( const uint32_t ) noexcept;

			/*	Loop sync objects
			*/
			[[nodiscard]] const SyncObjects& LoopSyncObjects() noexcept;

		private:
			/*	Circle iterator Of sync objects
			*/
			SyncObjects* curSyncObjects;

			SyncObjects* endSyncObjects;

			/*	Sync objects array
			*/
			stl::vector< SyncObjects > arrSyncObjects;

			/*	Command buffer array
			*/
			stl::vector< VkCommandBuffer > arrCommandBuffers;
		};
}



/**
 !	class VK_PresentChain
 */
namespace fce::inner::vulkan
{
	/*
	 @	Visit VkCommandBuffer array
	 */
	FC_INLINE const VkCommandBuffer VK_PresentChain::operator [] ( const uint32_t idx ) const NOEXCEPT
		{
			return this->arrCommandBuffers[ idx ];
		}
}