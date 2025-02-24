/*
**
**	VK_SwapChain.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_SwapChain
	 */
	class FC_API VK_SwapChain final
		{
			/*	Friends
			*/
			friend VK_WindowSurface;

			/*	Config
			 */
			struct
				{
					VkExtent2D extent2D;

					VkSurfaceFormatKHR surface;
				}
			config;

		public:
			/*	List types
			*/
			using ImageList = stl::vector< VkImage >;

			using ViewList = stl::vector< VkImageView >;

			/*	Access image views
			*/
			[[nodiscard]] auto end() const noexcept;

			[[nodiscard]] auto begin() const noexcept;

			[[nodiscard]] uint32_t Count() const noexcept;

			/*	Visit config
			*/
			[[nodiscard]] const auto* operator -> () const noexcept;

			/*	Get VkSwapchainKHR
			*/
			[[nodiscard]] operator const VkSwapchainKHR() const noexcept;

			[[nodiscard]] const VkSwapchainKHR& GetHandle() const noexcept;

			/*	Get image list
			*/
			VkResult GetImageList( ImageList& ) const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_SwapChain );

			~VK_SwapChain() = default;

			VK_SwapChain() noexcept;

			/*	Create & Destroy
			*/
			void Destroy() noexcept;

			VkResult Create( const VK_WindowSurface& ) noexcept;

			/*	Create & Destroy view list
			*/
			void DestroyViewList() noexcept;

			VkResult CreateViewList( const VkFormat ) noexcept;

			/*	Choose config
			*/
			VkPresentModeKHR				ChooseMode( const VK_WindowSurface& ) const noexcept;

			VkSurfaceFormatKHR				ChooseFormat( const VK_WindowSurface& ) const noexcept;

			VkExtent2D						ChooseSize( const VK_WindowSurface&,
														const VkSurfaceCapabilitiesKHR& ) const noexcept;

			uint32_t						ChooseCount( const VkSurfaceCapabilitiesKHR& ) const noexcept;

			VkCompositeAlphaFlagBitsKHR		ChooseAlpha( const VkSurfaceCapabilitiesKHR& ) const noexcept;

			VkSurfaceTransformFlagBitsKHR	ChooseTransform( const VkSurfaceCapabilitiesKHR& ) const noexcept;

		private:
			/*	View list
			*/
			ViewList arrViews;

			/*	Vk swap chain
			*/
			VkSwapchainKHR vkSwapchainKHR;

			/*	Array for choosing
			*/
			constinit static const VkPresentModeKHR MODE_SEQUENCE[];

			constinit static const VkSurfaceFormatKHR FORMAT_SEQUENCE[];

			constinit static const VkCompositeAlphaFlagBitsKHR ALPHA_GROUP[];

			constinit static const VkSurfaceTransformFlagBitsKHR TRANSFORM_GROUP[];
		};


	/*
	 @	VK_SwapChain singleton shortcut
	 */
	#define s_VK_SwapChain s_Window.surface.impl.swapChain
}



/**
 !	class VK_SwapChain
 */
namespace fce::inner::vulkan
{
	/*
	 @	Access image views
	 */
	FC_INLINE auto VK_SwapChain::end() const noexcept
		{
			return this->arrViews.cend();
		}

	FC_INLINE auto VK_SwapChain::begin() const noexcept
		{
			return this->arrViews.cbegin();
		}

	FC_INLINE uint32_t VK_SwapChain::Count() const noexcept
		{
			return static_cast< uint32_t >
					(
						this->arrViews.size()
					);
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	Visit config
	 */
	FC_INLINE const auto* VK_SwapChain::operator -> () const noexcept
		{
			return &this->config;
		}

	/*
	 @	Get VkSwapchainKHR
	 */
	FC_INLINE VK_SwapChain::operator const VkSwapchainKHR() const noexcept
		{
			return this->vkSwapchainKHR;
		}

	FC_INLINE const VkSwapchainKHR& VK_SwapChain::GetHandle() const noexcept
		{
			return this->vkSwapchainKHR;
		}
}