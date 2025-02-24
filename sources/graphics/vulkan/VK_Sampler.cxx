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
	 @	构造
	 */
	FC_INLINE VK_Sampler::VK_Sampler() noexcept
		:
			vkSampler( VK_NULL_HANDLE )
		{
		}

	/*
	 @	释放
	 */
	FC_INLINE void VK_Sampler::Release() noexcept
		{
			// 销毁 sampler
			if( this->vkSampler )
				{
					vkDestroySampler( s_VK_Device,
									  this->vkSampler,
									  s_VK_Allocator );
				}
		}


	/*
	 @	初始化 Info
	 */
	FC_INLINE VkResult VK_Sampler::Initialize( const sampler::Info& info )
		{
			VkResult res;

			// 设置 create info
			const VkSamplerCreateInfo createInfo
				{
					.sType					 = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
					.pNext					 = nullptr,
					.flags					 = 0,
					.magFilter				 = Sampler::Convert( info.filterMag ),
					.minFilter				 = Sampler::Convert( info.filterMin ),
					.mipmapMode				 = Sampler::Convert( info.mipmap ),
					.addressModeU			 = Sampler::Convert( info.addressU ),
					.addressModeV			 = Sampler::Convert( info.addressV ),
					.addressModeW			 = Sampler::Convert( info.addressW ),
					.mipLodBias				 = 0.0f,
					.anisotropyEnable		 = VK_TRUE,
					.maxAnisotropy			 = 16,
					.compareEnable			 = info.compareEnable,
					.compareOp				 = Graphics::Convert( info.compare ),
					.minLod					 = 0.0f,
					.maxLod					 = 0.0f,
					.borderColor			 = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
					.unnormalizedCoordinates = VK_FALSE
				};

			// 创建 sampler
			VK_ERR_GUARD( vkCreateSampler( s_VK_Device,
										   &createInfo,
										   s_VK_Allocator,
										   &this->vkSampler ) );

			return res;
		}
}