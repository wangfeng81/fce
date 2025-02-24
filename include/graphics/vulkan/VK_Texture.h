/*
**
**	VK_Texture.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_Texture2
	 */
	class FC_API VK_Texture2 final
		{
			/*	Friends
			*/
			friend Texture2;

		public:
			/*	Info
			*/
			using Info = Image::Info;

			/*	Get VkImageView
			*/
			[[nodiscard]] operator const VkImageView() const noexcept;

			[[nodiscard]] const VkImageView& GetHandle() const noexcept;

			[[nodiscard]] const VkSampler& GetSampler() const noexcept;

			/*	Update
			*/
			VkResult Update( const Image&,
							 const VkExtent2D&,
							 const VkOffset2D& src,
							 const VkOffset2D& dst );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Texture2 );

			~VK_Texture2() = default;

			VK_Texture2() noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const Info& );

			/*	Create image & view
			*/
			VkResult CreateView( const Info& );

			VkResult CreateImage( const Info& );

		private:
			/*	Image & view
			*/
			VkImage vkImage;

			VkImageView vkView;

			/*	Vma allocation
			*/
			VmaAllocation vmaAlloc;

			/*	VkSampler
			*/
			VkSampler vkSampler;

			/*	COLOR list
			*/
			static constexpr VkFormat COLOR_LIST[]
				{
					VK_FORMAT_UNDEFINED,
					VK_FORMAT_R8_UNORM,
					VK_FORMAT_B5G6R5_UNORM_PACK16,
					VK_FORMAT_R8G8B8_UNORM,
					VK_FORMAT_R8G8B8A8_UNORM,
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::texture
{
	/*
	 @	Texture2 vulkan impl
	 */
	using Impl = vulkan::VK_Texture2;
}



/**
 !	class VK_Texture2
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkImage
	 */
	FC_INLINE VK_Texture2::operator const VkImageView() const noexcept
		{
			return this->vkView;
		}

	FC_INLINE const VkImageView& VK_Texture2::GetHandle() const noexcept
		{
			return this->vkView;
		}

	FC_INLINE const VkSampler& VK_Texture2::GetSampler() const noexcept
		{
			return this->vkSampler;
		}
}