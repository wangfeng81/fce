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
	 @	优选的 present mode 顺序表

			FIFO	最丝滑 按队列先进先出
			MAILBOX	切换到最新 可能会丢帧

			FIFO		 & MAILBOX		都确保 垂直刷新 不会撕裂
			FIFO_RELAXED & IMMEDIATE	不确保 垂直刷新 可能撕裂
	 */
	constinit const VkPresentModeKHR VK_SwapChain::MODE_SEQUENCE[]
		{
			VK_PRESENT_MODE_FIFO_KHR,
			VK_PRESENT_MODE_MAILBOX_KHR,

			VK_PRESENT_MODE_FIFO_RELAXED_KHR,
			VK_PRESENT_MODE_IMMEDIATE_KHR
		};

	/*
	 @	优选的 surface format 顺序表
	 */
	constinit const VkSurfaceFormatKHR VK_SwapChain::FORMAT_SEQUENCE[]
		{
			{ VK_FORMAT_B8G8R8A8_SRGB,	VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },

			{ VK_FORMAT_B8G8R8A8_UNORM,	VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }
		};

	/*
	 @	预设的 composite alpha 组
	 */
	constinit const VkCompositeAlphaFlagBitsKHR VK_SwapChain::ALPHA_GROUP[]
		{
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
		};

	/*
	 @	预设的 surface transform 组
	 */
	constinit const VkSurfaceTransformFlagBitsKHR VK_SwapChain::TRANSFORM_GROUP[]
		{
			VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
		};
}



namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_SwapChain::VK_SwapChain() noexcept
		:
			config{},
			vkSwapchainKHR( VK_NULL_HANDLE )
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁
	 */
	FC_INLINE void VK_SwapChain::Destroy() noexcept
		{
			// 销毁 View list
			this->DestroyViewList();

			// 销毁 Swap chain
			if( this->vkSwapchainKHR )
				{
					vkDestroySwapchainKHR( s_VK_Device,
										   this->vkSwapchainKHR,
										   s_VK_Allocator );

					// 由于可能 Rebuild 所以赋空
					this->vkSwapchainKHR = VK_NULL_HANDLE;
				}
		}


	/*
	 @	创建
	 */
	FC_INLINE VkResult VK_SwapChain::Create( const VK_WindowSurface& windowSurface ) noexcept
		{
			// 获取 surface 特性
			VkSurfaceCapabilitiesKHR capabilities;

			VkResult res
				{
					vkGetPhysicalDeviceSurfaceCapabilitiesKHR( s_VK_PhyDevice,
															   windowSurface,
															   &capabilities )
				};

			if( res == VK_SUCCESS )
				{
					// 设置 config
					this->config =
						{
							.extent2D	= this->ChooseSize( windowSurface,
															capabilities ),
							.surface	= this->ChooseFormat( windowSurface )
						};

					// 设置 create info
					const VkSwapchainCreateInfoKHR createInfo
						{
							.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
							.pNext					= nullptr,
							.flags					= 0,
							.surface				= windowSurface,
							.minImageCount			= this->ChooseCount( capabilities ),
							.imageFormat			= this->config.surface.format,
							.imageColorSpace		= this->config.surface.colorSpace,
							.imageExtent			= this->config.extent2D,
							.imageArrayLayers		= 1,// 用于 VR 眼镜 之类的
							.imageUsage				= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
							.imageSharingMode		= s_VK_PhyDevice->sharingMode,
							.queueFamilyIndexCount	= s_VK_PhyDevice->queueFamilies.count,
							.pQueueFamilyIndices	= s_VK_PhyDevice->queueFamilies.data,
							.preTransform			= this->ChooseTransform( capabilities ),
							.compositeAlpha			= this->ChooseAlpha( capabilities ),
							.presentMode			= this->ChooseMode( windowSurface ),
							.clipped				= true,
							.oldSwapchain			= VK_NULL_HANDLE
						};

					// 创建 VkSwapchainKHR
					VK_ERR_GUARD( vkCreateSwapchainKHR( s_VK_Device,
														&createInfo,
														s_VK_Allocator,
														&this->vkSwapchainKHR ) );

					// 创建 view list
					if( res == VK_SUCCESS )
						{
							res = this->CreateViewList( this->config.surface.format );
						}
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁 View list
	 */
	FC_INLINE void VK_SwapChain::DestroyViewList() noexcept
		{
			// 销毁 视图列表
			for( VkImageView vkImageView : this->arrViews )
				{
					if( vkImageView )
						{
							vkDestroyImageView( s_VK_Device,
												vkImageView,
												s_VK_Allocator );
						}
				}
		}


	/*
	 @	创建 View list
	 */
	FC_INLINE VkResult VK_SwapChain::CreateViewList( const VkFormat vkFormat ) noexcept
		{
			// 获取 image 数组，在 lamble 内判断 res
			ImageList arrImages;

			VkResult res
				{
					this->GetImageList( arrImages )
				};

			// 设置 create info
			VkImageViewCreateInfo createInfo
				{
					.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.pNext				= nullptr,
					.flags				= 0,
					.image				= VK_NULL_HANDLE,
					.viewType			= VK_IMAGE_VIEW_TYPE_2D,
					.format				= vkFormat,
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

			/*
			 @	用 image 生成 view
			 */
			auto f = [ &res, &createInfo ]( const VkImage vkImage ) noexcept
				{
					// 初始化 view 空
					VkImageView vkImageView{ VK_NULL_HANDLE };

					if( res == VK_SUCCESS )
						{
							// 设置 vkImage
							createInfo.image = vkImage;

							// 创建 view
							VK_ERR_GUARD( vkCreateImageView( s_VK_Device,
															 &createInfo,
															 s_VK_Allocator,
															 &vkImageView ) );
						}

					return vkImageView;
				};

			// 通过 image list 生成 view list
			stl::transform( arrImages, this->arrViews, f );

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	选择 present mode
	 */
	FC_INLINE VkPresentModeKHR VK_SwapChain::ChooseMode( const VK_WindowSurface& windowSurface ) const noexcept
		{
			// 获取 surface 支持的 mode 数组
			VK_WindowSurface::ModeList arrSupportes;

			if( windowSurface.GetModeList( arrSupportes ) == VK_SUCCESS )
				{
					// 检查 支持是否空
					if( ! arrSupportes.empty() )
						{
							// 按优选顺序遍历 mode sequence
							for( const VkPresentModeKHR presentMode : MODE_SEQUENCE )
								{
									// 查找 mode 在支持的表内 则返回
									if( stl::find( arrSupportes,
												   presentMode ) != arrSupportes.end() )
										{
											return presentMode;
										}
								}

							// 次选第一个 支持的
							return arrSupportes.front();
						}
				}

			return MODE_SEQUENCE[ 0 ];
		}


	/*
	 @	选择 Surface format
	 */
	FC_INLINE VkSurfaceFormatKHR VK_SwapChain::ChooseFormat( const VK_WindowSurface& windowSurface ) const noexcept
		{
			// 获取 surface 支持的 format 数组
			VK_WindowSurface::FormatList arrSupportes;

			if( windowSurface.GetFormatList( arrSupportes ) == VK_SUCCESS )
				{
					// 检查 支持是否空
					if( ! arrSupportes.empty() )
						{
							// 获取 第一个支持
							const VkSurfaceFormatKHR& firstSupported
								{
									arrSupportes.front()
								};

							// 如果 surface 没有指定 则自定义最优格式
							if( firstSupported.format == VK_FORMAT_UNDEFINED )
								{
									return FORMAT_SEQUENCE[ 0 ];
								}

							// 按优选顺序遍历 format sequence
							for( const VkSurfaceFormatKHR& surfaceFormat : FORMAT_SEQUENCE )
								{
									/*
									 @	比较 format
									 */
									auto f = [ &surfaceFormat ]( const VkSurfaceFormatKHR& supported ) noexcept
										{
											return supported.format		== surfaceFormat.format
												&& supported.colorSpace == surfaceFormat.colorSpace;
										};

									// 查找 mode 在支持的表内 则返回
									if( stl::find_if( arrSupportes, f ) != arrSupportes.end() )
										{
											return surfaceFormat;
										}
								}

							// 次选第一个 支持的
							return firstSupported;
						}
				}

			return FORMAT_SEQUENCE[ 0 ];
		}


	/*
	 @	选择 交换链 尺寸
	 */
	FC_INLINE VkExtent2D VK_SwapChain::ChooseSize( const VK_WindowSurface& windowSurface,
												   const VkSurfaceCapabilitiesKHR& capabilities ) const noexcept
		{
			// 宽高之一设置成 0xFFFFFFFF 则未定义
			if( capabilities.currentExtent.width == UInt32_MAX )
				{
					// 初始化尺寸为 客户区显示尺寸
					VkExtent2D extent2D( windowSurface.superior.GetSize() );

					// 调节宽高 在支持的范围之内
					const VkExtent2D& minExtent
						{
							capabilities.minImageExtent
						};

					const VkExtent2D& maxExtent
						{
							capabilities.maxImageExtent
						};

					// 调节 宽度
					if( extent2D.width < minExtent.width )
						{
							extent2D.width = minExtent.width;
						}
					else if( extent2D.width > maxExtent.width )
						{
							extent2D.width = maxExtent.width;
						}

					// 调节 高度
					if( extent2D.height < minExtent.height )
						{
							extent2D.height = minExtent.height;
						}
					else if( extent2D.height > maxExtent.height )
						{
							extent2D.height = maxExtent.height;
						}

					return extent2D;
				}
			else
				{
					// 如果定义了 currentExtent 则直接使用
					return capabilities.currentExtent;
				}
		}


	/*
	 @	选择 交换链 图像数量
	 */
	FC_INLINE uint32_t VK_SwapChain::ChooseCount( const VkSurfaceCapabilitiesKHR& capabilities ) const noexcept
		{
			// 优选 三缓冲
			constexpr uint32_t count{ 3 };

			// 交换链 至少两个 Image
			const uint32_t minCount
				{
					capabilities.minImageCount
				};

			// maxImageCount 为 0 则没有数量限制
			const uint32_t maxCount
				{
					capabilities.maxImageCount
				};

			// 确保 小于等于 maxImageCount
			if( count > maxCount && maxCount > 0 )
				{
					return maxCount;
				}

			// 确保 大于等于 minImageCount
			if( count < minCount )
				{
					return minCount;
				}

			// 三缓冲
			return count;
		}


	/*
	 @	选择 和背景的混合方式
	 */
	FC_INLINE VkCompositeAlphaFlagBitsKHR VK_SwapChain::ChooseAlpha( const VkSurfaceCapabilitiesKHR& capabilities ) const noexcept
		{
			VkCompositeAlphaFlagsKHR compositeBits{};

			// 组合 composite alpha
			for( const VkCompositeAlphaFlagBitsKHR alphaFlag : ALPHA_GROUP )
				{
					if( capabilities.supportedCompositeAlpha & alphaFlag )
						{
							compositeBits |= alphaFlag;
						}
				}

			// 如果 优选不支持，则使用默认
			return static_cast< VkCompositeAlphaFlagBitsKHR >
					(
						compositeBits ? compositeBits
									  : capabilities.supportedCompositeAlpha
					);
		}


	/*
	 @	选择 变换方式
	 */
	FC_INLINE VkSurfaceTransformFlagBitsKHR VK_SwapChain::ChooseTransform( const VkSurfaceCapabilitiesKHR& capabilities ) const noexcept
		{
			VkSurfaceTransformFlagsKHR transformBits{};

			// 组合 surface transform
			for( const VkSurfaceTransformFlagBitsKHR transformFlag : TRANSFORM_GROUP )
				{
					if( capabilities.supportedTransforms & transformFlag )
						{
							transformBits |= transformFlag;
						}
				}

			// 如果 优选不支持，则使用默认
			return static_cast< VkSurfaceTransformFlagBitsKHR >
					(
						transformBits ? transformBits
									  : capabilities.currentTransform
					);
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	获取 image 列表
	 */
	VkResult VK_SwapChain::GetImageList( ImageList& arrImages ) const noexcept
		{
			// 获取 image 数量
			uint32_t count;

			VkResult res
				{
					vkGetSwapchainImagesKHR( s_VK_Device,
											 this->vkSwapchainKHR,
											 &count, nullptr )
				};

			// 获取 image 列表
			if( res == VK_SUCCESS && count )
				{
					arrImages.resize( count );

					res = vkGetSwapchainImagesKHR( s_VK_Device,
												   this->vkSwapchainKHR,
												   &count, arrImages.data() );
				}

			return res;
		}
}