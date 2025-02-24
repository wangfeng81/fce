/*
**
**	VK_Sampler.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_Sampler
	 */
	class FC_API VK_Sampler final
		{
			/*	Friends
			*/
			friend Sampler;

		public:
			/*	Get VkSampler
			*/
			[[nodiscard]] operator const VkSampler() const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Sampler );

			~VK_Sampler() = default;

			VK_Sampler() noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const sampler::Info& );

		private:
			/*	VkSampler
			*/
			VkSampler vkSampler;

			/*	VkFilter list
			*/
			static constexpr VkFilter FILTER_LIST[]
				{
					VK_FILTER_NEAREST,
					VK_FILTER_LINEAR,

					VK_FILTER_CUBIC_EXT
				};

			/*	VkSamplerMipmapMode list
			*/
			static constexpr VkSamplerMipmapMode MIPMAP_LIST[]
				{
					VK_SAMPLER_MIPMAP_MODE_NEAREST,
					VK_SAMPLER_MIPMAP_MODE_LINEAR
				};

			/*	VkSamplerAddressMode list
			*/
			static constexpr VkSamplerAddressMode ADDRESS_LIST[]
				{
					VK_SAMPLER_ADDRESS_MODE_REPEAT,
					VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
					VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
					VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
					VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::sampler
{
	/*
	 @	Sampler vulkan impl
	 */
	using Impl = vulkan::VK_Sampler;
}



/**
 !	class VK_Sampler
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkSampler
	 */
	FC_INLINE VK_Sampler::operator const VkSampler() const noexcept
		{
			return this->vkSampler;
		}
}