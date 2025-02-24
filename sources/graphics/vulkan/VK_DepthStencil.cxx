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
	 @	选取的 depth 顺序表
	 */
	constinit const VkFormat VK_DepthStencil::DEPTH_SEQUENCE[]
		{
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_X8_D24_UNORM_PACK32,
			VK_FORMAT_D16_UNORM,

			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT
		};

	/*
	 @	选取的 stencil 顺序表
	 */
	constinit const VkFormat VK_DepthStencil::STENCIL_SEQUENCE[]
		{
			VK_FORMAT_S8_UINT,

			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D32_SFLOAT_S8_UINT
		};

	/*
	 @	选取的 depth & stencil 顺序表
	 */
	constinit const VkFormat VK_DepthStencil::DEPTH_STENCIL_SEQUENCE[]
		{
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT
		};
}



namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_DepthStencil::VK_DepthStencil() noexcept
		:
			vkView( VK_NULL_HANDLE ),
			vkImage( VK_NULL_HANDLE ),
			vmaAlloc( VK_NULL_HANDLE )
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁
	 */
	FC_INLINE void VK_DepthStencil::Release() noexcept
		{
			// 销毁 view
			if( this->vkView )
				{
					vkDestroyImageView( s_VK_Device,
										this->vkView,
										s_VK_Allocator );
				}

			// 销毁 vma image 并释放 vk buffer
			if( this->vkImage && vmaAlloc )
				{
					vmaDestroyImage( s_VK_Allocator,
									 this->vkImage,
									 this->vmaAlloc );
				}
		}


	/*
	 @	创建
	 */
	FC_INLINE VkResult VK_DepthStencil::Initialize( const VkExtent2D& extent2D,
													const depthStencil::TYPE type ) noexcept
		{
			// 选取 配置
			Config cfg;

			VkResult res
				{
					ChooseConfig( cfg, type )
				};

			if( res == VK_SUCCESS )
				{
					// 创建 image
					res = this->CreateImage( cfg,
											 extent2D );

					if( res == VK_SUCCESS )
						{
							// 创建 view
							res = this->CreateView( cfg );
						}
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	创建 image
	 */
	FC_INLINE VkResult VK_DepthStencil::CreateImage( const Config& cfg,
													 const VkExtent2D& extent2D ) noexcept
		{
			VkResult res;

			// 设置 Vk image create info
			const VkImageCreateInfo vkImageCreateInfo
				{
					.sType					= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.imageType				= VK_IMAGE_TYPE_2D,
					.format					= cfg.format,
					.extent
					{
						.width				= extent2D.width,
						.height				= extent2D.height,
						.depth				= 1
					},
					.mipLevels				= 1,
					.arrayLayers			= 1,
					.samples				= VK_SAMPLE_COUNT_1_BIT,
					.tiling					= cfg.tiling,
					.usage					= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
					.sharingMode			= s_VK_PhyDevice->sharingMode,
					.queueFamilyIndexCount	= 0,
					.pQueueFamilyIndices	= nullptr,
					.initialLayout			= VK_IMAGE_LAYOUT_UNDEFINED
				};

			// 设置 Vma allocation create info
			const VmaAllocationCreateInfo vmaAllocCreateInfo
				{
					.flags			= 0,
					.usage			= VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
					.requiredFlags	= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					.preferredFlags	= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					.memoryTypeBits	= 0,
					.pool			= nullptr,
					.pUserData		= nullptr,
					.priority		= 0.0f
				};
			
			// 创建 vma image
			VK_ERR_GUARD( vmaCreateImage( s_VK_Allocator,
										  &vkImageCreateInfo,
										  &vmaAllocCreateInfo,
										  &this->vkImage,
										  &this->vmaAlloc,
										  nullptr ) );

			return res;
		}


	/*
	 @	创建 view
	 */
	FC_INLINE VkResult VK_DepthStencil::CreateView( const Config& cfg ) noexcept
		{
			VkResult res;

			// 设置 create info
			const VkImageViewCreateInfo createInfo
				{
					.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.pNext				= nullptr,
					.flags				= 0,
					.image				= this->vkImage,
					.viewType			= VK_IMAGE_VIEW_TYPE_2D,
					.format				= cfg.format,
					.components
					{
						.r				= VK_COMPONENT_SWIZZLE_IDENTITY,
						.g				= VK_COMPONENT_SWIZZLE_IDENTITY,
						.b				= VK_COMPONENT_SWIZZLE_IDENTITY,
						.a				= VK_COMPONENT_SWIZZLE_IDENTITY
					},
					.subresourceRange
					{
						.aspectMask		= cfg.aspect,
						.baseMipLevel	= 0,
						.levelCount		= 1,
						.baseArrayLayer	= 0,
						.layerCount		= 1
					}
				};

			// 创建 view
			VK_ERR_GUARD( vkCreateImageView( s_VK_Device,
											 &createInfo,
											 s_VK_Allocator,
											 &this->vkView ) );

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	选取 config
	 */
	VkResult VK_DepthStencil::ChooseConfig( Config& cfg,
											const depthStencil::TYPE type ) noexcept
		{
			std::size_t formatCount;

			const VkFormat* formatSequence;

			switch( type )
				{
					case depthStencil::TYPE::DEPTH:
						{
							formatSequence = DEPTH_SEQUENCE;
							formatCount = std::ranges::size( DEPTH_SEQUENCE );

							cfg.aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
						}
						break;

					case depthStencil::TYPE::STENCIL:
						{
							formatSequence = STENCIL_SEQUENCE;
							formatCount = std::ranges::size( STENCIL_SEQUENCE );

							cfg.aspect = VK_IMAGE_ASPECT_STENCIL_BIT;
						}
						break;

					case depthStencil::TYPE::DEPTH_STENCIL:
						{
							formatSequence = DEPTH_STENCIL_SEQUENCE;
							formatCount = std::ranges::size( DEPTH_STENCIL_SEQUENCE );

							cfg.aspect = VK_IMAGE_ASPECT_DEPTH_BIT
									   | VK_IMAGE_ASPECT_STENCIL_BIT;
						}
						break;

					default: return VK_ERROR_FORMAT_NOT_SUPPORTED;
				}

			return ChooseFormat( cfg.format,
								 cfg.tiling,
								 formatCount,
								 formatSequence );
		}


	/*
	 @	选取 format
	 */
	FC_INLINE VkResult VK_DepthStencil::ChooseFormat( VkFormat& format,
													  VkImageTiling& tiling,
													  const std::size_t formatCount,
													  const VkFormat* const formatSequence ) noexcept
		{
			// 按优选顺序遍历 format list
			for( std::size_t i{ 0 }; i < formatCount; ++i )
				{
					format = formatSequence[ i ];

					// 选取 tiling
					tiling = ChooseTiling( format );

					// 检测是否支持
					if( tiling != VK_IMAGE_TILING_MAX_ENUM )
						{
							return VK_SUCCESS;
						}
				}

			// 没有合适的 format
			return VK_ERROR_FORMAT_NOT_SUPPORTED;
		}


	/*
	 @	选取 tiling
	 */
	FC_INLINE VkImageTiling VK_DepthStencil::ChooseTiling( const VkFormat format ) noexcept
		{
			// 获取 format properties
			VkFormatProperties properties;

			vkGetPhysicalDeviceFormatProperties( s_VK_PhyDevice,
												 format,
												 &properties );

			// 优选 optimal 效率高
			if( properties.optimalTilingFeatures
			  & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT )
				{
					return VK_IMAGE_TILING_OPTIMAL;
				}
			// 次选 linear
			else if( properties.linearTilingFeatures
				   & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT )
				{
					return VK_IMAGE_TILING_LINEAR;
				}

			// 不支持
			return VK_IMAGE_TILING_MAX_ENUM;
		}
}