/*
**
**	VK_DepthStencil.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_DepthStencil
	 */
	class FC_API VK_DepthStencil final
		{
			/*	Friends
			*/
			friend DepthStencil;

		public:
			/*	Config
			*/
			struct Config final
				{
					VkFormat format;

					VkImageTiling tiling;

					VkImageAspectFlags aspect;
				};

			/*	Get VkImageView
			*/
			[[nodiscard]] operator const VkImageView() const noexcept;

			[[nodiscard]] const VkImageView& GetHandle() const noexcept;

			/*	Choose config & format & tiling
			*/
			static VkResult ChooseFormat( VkFormat&,
										  VkImageTiling&,
										  const std::size_t,
										  const VkFormat* const ) noexcept;

			static VkResult ChooseConfig( Config&,
										  const depthStencil::TYPE ) noexcept;

			[[nodiscard]] static VkImageTiling ChooseTiling( const VkFormat ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_DepthStencil );

			~VK_DepthStencil() = default;

			VK_DepthStencil() noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const VkExtent2D&,
								 const depthStencil::TYPE ) noexcept;

			/*	Create image & view
			*/
			VkResult CreateView( const Config& ) noexcept;

			VkResult CreateImage( const Config&,
								  const VkExtent2D& ) noexcept;

		private:
			/*	Image & view
			*/
			VkImage vkImage;

			VkImageView vkView;

			/*	Vma allocation
			*/
			VmaAllocation vmaAlloc;

			/*	Array for choosing
			*/
			constinit static const VkFormat DEPTH_SEQUENCE[];

			constinit static const VkFormat STENCIL_SEQUENCE[];

			constinit static const VkFormat DEPTH_STENCIL_SEQUENCE[];

			/*	VkStencilOp list
			*/
			static constexpr VkStencilOp STENCIL_LIST[]
				{
					VK_STENCIL_OP_KEEP,
					VK_STENCIL_OP_ZERO,
					VK_STENCIL_OP_REPLACE,
					VK_STENCIL_OP_INCREMENT_AND_CLAMP,
					VK_STENCIL_OP_DECREMENT_AND_CLAMP,
					VK_STENCIL_OP_INVERT,
					VK_STENCIL_OP_INCREMENT_AND_WRAP,
					VK_STENCIL_OP_DECREMENT_AND_WRAP
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::depthStencil
{
	/*
	 @	DepthStencil vulkan impl
	 */
	using Impl = vulkan::VK_DepthStencil;
}



/**
 !	class VK_Texture2
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkImage
	 */
	FC_INLINE VK_DepthStencil::operator const VkImageView() const noexcept
		{
			return this->vkView;
		}

	FC_INLINE const VkImageView& VK_DepthStencil::GetHandle() const noexcept
		{
			return this->vkView;
		}
}