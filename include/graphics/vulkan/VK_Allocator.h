/*
**
**	VK_Allocator.
**
		Vulkan allocator

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_Allocator
	 */
	class FC_API VK_Allocator final
		{
			/*	Friends
			*/
			friend VK_Graphics;

		public:
			/*	Free & Malloc device memory
			*/
			static void Free( VkDeviceMemory ) noexcept;

			static VkResult Malloc( VkDeviceMemory&,
									const VkMemoryRequirements&,
									const VkMemoryPropertyFlags ) noexcept;

			/*	VmaAllocator & VkAllocationCallbacks
			*/
			[[nodiscard]] operator const VmaAllocator() const noexcept;

			[[nodiscard]] operator const VkAllocationCallbacks*() const noexcept;

			/*	Get Device memory size
			*/
			[[nodiscard]] VkDeviceSize GetDeviceMemorySize() const noexcept;

			/*	Find memory type index
			*/
			[[nodiscard]] static uint32_t FindMemoryType( const uint32_t typeBits,
														  const VkMemoryPropertyFlags ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Allocator );

			~VK_Allocator() = default;

			VK_Allocator() noexcept;

			/*	Create & Destroy
			*/
			void Destroy() noexcept;

			VkResult Create() noexcept;

			/*	Create & Destroy VMA
			*/
			void DestroyVMA() noexcept;

			VkResult CreateVMA() noexcept;

			/*	Device memory allocation callbacks
			*/
			static void VKAPI_PTR on_VmaFree( VmaAllocator,
											  uint32_t,
											  VkDeviceMemory,
											  VkDeviceSize,
											  void* pUserData ) noexcept;

			static void VKAPI_PTR on_VmaAllocate( VmaAllocator,
												  uint32_t,
												  VkDeviceMemory,
												  VkDeviceSize,
												  void* pUserData ) noexcept;

			/*	Host memory allocation callbacks
			*/
			static void VKAPI_PTR vkFree( void* pUserData,
										  void* pMemory ) noexcept;

			static void* VKAPI_PTR vkMalloc( void* pUserData,
											 size_t size,
											 size_t alignment,
											 VkSystemAllocationScope ) noexcept;

			static void* VKAPI_PTR vkRealloc( void* pUserData,
											  void* pOriginal,
											  size_t size,
											  size_t alignment,
											  VkSystemAllocationScope ) noexcept;

			static void VKAPI_PTR on_InternalFree( void* pUserData,
												   size_t size,
												   VkInternalAllocationType,
												   VkSystemAllocationScope ) noexcept;

			static void VKAPI_PTR on_InternalAllocate( void* pUserData,
													   size_t size,
													   VkInternalAllocationType,
													   VkSystemAllocationScope ) noexcept;

		private:
			/*	VmaAllocator
			*/
			VmaAllocator vma;

			/*	Device memory size
			*/
			VkDeviceSize deviceMemorySize;

			/*	VkAllocationCallbacks
			*/
			const VkAllocationCallbacks vkCallbacks;
		};


	/*
	 @	VK_Allocator singleton shortcut
	 */
	#define s_VK_Allocator s_Graphics.impl.allocator
}



/**
 !	class VK_Allocator
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VmaAllocator
	 */
	FC_INLINE VK_Allocator::operator const VmaAllocator() const noexcept
		{
			return this->vma;
		}

	/*
	 @	Get VkAllocationCallbacks pointer
	 */
	FC_INLINE VK_Allocator::operator const VkAllocationCallbacks*() const noexcept
		{
			return &this->vkCallbacks;
		}

	/*
	 @	Get Device memory size
	 */
	FC_INLINE VkDeviceSize VK_Allocator::GetDeviceMemorySize() const noexcept
		{
			return this->deviceMemorySize;
		}
}