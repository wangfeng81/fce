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
	 @	构造
	 */
	FC_INLINE VK_Texture2::VK_Texture2() noexcept
		:
			vkView( VK_NULL_HANDLE ),
			vkImage( VK_NULL_HANDLE ),
			vmaAlloc( VK_NULL_HANDLE ),
			vkSampler( Sampler::Default()->impl )
		{
		}

	/*
	 @	释放
	 */
	FC_INLINE void VK_Texture2::Release() noexcept
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
}



namespace fce::inner::vulkan
{
	/*
	 @	初始化 Info
	 */
	FC_INLINE VkResult VK_Texture2::Initialize( const Info& info )
		{
			// 创建 vkImage
			VkResult res
				{
					this->CreateImage( info )
				};

			if( res == VK_SUCCESS )
				{
					// 创建 vkView
					res = this->CreateView( info );
				}

			return res;
		}


	/*
	 @	更新 Image
	 */
	FC_INLINE VkResult VK_Texture2::Update( const Image& image,
											const VkExtent2D& extent2D,
											const VkOffset2D& srcOffset,
											const VkOffset2D& dstOffset )
		{
			// 设置 Copy region
			const VkBufferImageCopy region
				{
					.bufferOffset		= VkDeviceSize( image.Offset( srcOffset ) ),
					.bufferRowLength	= static_cast< uint32_t >( image.Width() ),
					.bufferImageHeight	= static_cast< uint32_t >( image.Height() ),
					.imageSubresource
					{
						.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT,
						.mipLevel		= 0,
						.baseArrayLayer	= 0,
						.layerCount		= 1
					},
					.imageOffset
					{
						.x				= dstOffset.x,
						.y				= dstOffset.y,
						.z				= 0
					},
					.imageExtent
					{
						.width			= extent2D.width,
						.height			= extent2D.height,
						.depth			= 1
					}
				};

			// 拷贝 tempBuffer 到 vkImage
			return s_VK_Command.CopyImage( *image,
											this->vkImage,
											region );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	创建 vkView

			相当于 Image::Info 信息
	 */
	FC_INLINE VkResult VK_Texture2::CreateView( const Info& info )
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
					.format				= Texture2::Convert( info.color ),
					.components
					{
						.r				= VK_COMPONENT_SWIZZLE_R,
						.g				= VK_COMPONENT_SWIZZLE_G,
						.b				= VK_COMPONENT_SWIZZLE_B,
						.a				= VK_COMPONENT_SWIZZLE_A
					},
					.subresourceRange
					{
						.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT,
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


	/*
	 @	创建 vkImage
	 */
	FC_INLINE VkResult VK_Texture2::CreateImage( const Info& info )
		{
			VkResult res;

			// 设置 Vk image create info
			const VkImageCreateInfo vkImageCreateInfo
				{
					.sType					= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.imageType				= VK_IMAGE_TYPE_2D,
					.format					= Texture2::Convert( info.color ),
					.extent
					{
						.width				= static_cast< uint32_t >( info.width ),
						.height				= static_cast< uint32_t >( info.height ),
						.depth				= 1
					},
					.mipLevels				= 1,
					.arrayLayers			= 1,
					.samples				= VK_SAMPLE_COUNT_1_BIT,
					.tiling					= VK_IMAGE_TILING_OPTIMAL,
					.usage					= VK_IMAGE_USAGE_TRANSFER_DST_BIT
											| VK_IMAGE_USAGE_SAMPLED_BIT,
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
					.requiredFlags	= 0,
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
}