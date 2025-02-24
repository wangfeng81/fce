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
	 @	��ѡ�� present mode ˳���

			FIFO	��˿�� �������Ƚ��ȳ�
			MAILBOX	�л������� ���ܻᶪ֡

			FIFO		 & MAILBOX		��ȷ�� ��ֱˢ�� ����˺��
			FIFO_RELAXED & IMMEDIATE	��ȷ�� ��ֱˢ�� ����˺��
	 */
	constinit const VkPresentModeKHR VK_SwapChain::MODE_SEQUENCE[]
		{
			VK_PRESENT_MODE_FIFO_KHR,
			VK_PRESENT_MODE_MAILBOX_KHR,

			VK_PRESENT_MODE_FIFO_RELAXED_KHR,
			VK_PRESENT_MODE_IMMEDIATE_KHR
		};

	/*
	 @	��ѡ�� surface format ˳���
	 */
	constinit const VkSurfaceFormatKHR VK_SwapChain::FORMAT_SEQUENCE[]
		{
			{ VK_FORMAT_B8G8R8A8_SRGB,	VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },

			{ VK_FORMAT_B8G8R8A8_UNORM,	VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }
		};

	/*
	 @	Ԥ��� composite alpha ��
	 */
	constinit const VkCompositeAlphaFlagBitsKHR VK_SwapChain::ALPHA_GROUP[]
		{
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
		};

	/*
	 @	Ԥ��� surface transform ��
	 */
	constinit const VkSurfaceTransformFlagBitsKHR VK_SwapChain::TRANSFORM_GROUP[]
		{
			VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
		};
}



namespace fce::inner::vulkan
{
	/*
	 @	����
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
	 @	����
	 */
	FC_INLINE void VK_SwapChain::Destroy() noexcept
		{
			// ���� View list
			this->DestroyViewList();

			// ���� Swap chain
			if( this->vkSwapchainKHR )
				{
					vkDestroySwapchainKHR( s_VK_Device,
										   this->vkSwapchainKHR,
										   s_VK_Allocator );

					// ���ڿ��� Rebuild ���Ը���
					this->vkSwapchainKHR = VK_NULL_HANDLE;
				}
		}


	/*
	 @	����
	 */
	FC_INLINE VkResult VK_SwapChain::Create( const VK_WindowSurface& windowSurface ) noexcept
		{
			// ��ȡ surface ����
			VkSurfaceCapabilitiesKHR capabilities;

			VkResult res
				{
					vkGetPhysicalDeviceSurfaceCapabilitiesKHR( s_VK_PhyDevice,
															   windowSurface,
															   &capabilities )
				};

			if( res == VK_SUCCESS )
				{
					// ���� config
					this->config =
						{
							.extent2D	= this->ChooseSize( windowSurface,
															capabilities ),
							.surface	= this->ChooseFormat( windowSurface )
						};

					// ���� create info
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
							.imageArrayLayers		= 1,// ���� VR �۾� ֮���
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

					// ���� VkSwapchainKHR
					VK_ERR_GUARD( vkCreateSwapchainKHR( s_VK_Device,
														&createInfo,
														s_VK_Allocator,
														&this->vkSwapchainKHR ) );

					// ���� view list
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
	 @	���� View list
	 */
	FC_INLINE void VK_SwapChain::DestroyViewList() noexcept
		{
			// ���� ��ͼ�б�
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
	 @	���� View list
	 */
	FC_INLINE VkResult VK_SwapChain::CreateViewList( const VkFormat vkFormat ) noexcept
		{
			// ��ȡ image ���飬�� lamble ���ж� res
			ImageList arrImages;

			VkResult res
				{
					this->GetImageList( arrImages )
				};

			// ���� create info
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
			 @	�� image ���� view
			 */
			auto f = [ &res, &createInfo ]( const VkImage vkImage ) noexcept
				{
					// ��ʼ�� view ��
					VkImageView vkImageView{ VK_NULL_HANDLE };

					if( res == VK_SUCCESS )
						{
							// ���� vkImage
							createInfo.image = vkImage;

							// ���� view
							VK_ERR_GUARD( vkCreateImageView( s_VK_Device,
															 &createInfo,
															 s_VK_Allocator,
															 &vkImageView ) );
						}

					return vkImageView;
				};

			// ͨ�� image list ���� view list
			stl::transform( arrImages, this->arrViews, f );

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	ѡ�� present mode
	 */
	FC_INLINE VkPresentModeKHR VK_SwapChain::ChooseMode( const VK_WindowSurface& windowSurface ) const noexcept
		{
			// ��ȡ surface ֧�ֵ� mode ����
			VK_WindowSurface::ModeList arrSupportes;

			if( windowSurface.GetModeList( arrSupportes ) == VK_SUCCESS )
				{
					// ��� ֧���Ƿ��
					if( ! arrSupportes.empty() )
						{
							// ����ѡ˳����� mode sequence
							for( const VkPresentModeKHR presentMode : MODE_SEQUENCE )
								{
									// ���� mode ��֧�ֵı��� �򷵻�
									if( stl::find( arrSupportes,
												   presentMode ) != arrSupportes.end() )
										{
											return presentMode;
										}
								}

							// ��ѡ��һ�� ֧�ֵ�
							return arrSupportes.front();
						}
				}

			return MODE_SEQUENCE[ 0 ];
		}


	/*
	 @	ѡ�� Surface format
	 */
	FC_INLINE VkSurfaceFormatKHR VK_SwapChain::ChooseFormat( const VK_WindowSurface& windowSurface ) const noexcept
		{
			// ��ȡ surface ֧�ֵ� format ����
			VK_WindowSurface::FormatList arrSupportes;

			if( windowSurface.GetFormatList( arrSupportes ) == VK_SUCCESS )
				{
					// ��� ֧���Ƿ��
					if( ! arrSupportes.empty() )
						{
							// ��ȡ ��һ��֧��
							const VkSurfaceFormatKHR& firstSupported
								{
									arrSupportes.front()
								};

							// ��� surface û��ָ�� ���Զ������Ÿ�ʽ
							if( firstSupported.format == VK_FORMAT_UNDEFINED )
								{
									return FORMAT_SEQUENCE[ 0 ];
								}

							// ����ѡ˳����� format sequence
							for( const VkSurfaceFormatKHR& surfaceFormat : FORMAT_SEQUENCE )
								{
									/*
									 @	�Ƚ� format
									 */
									auto f = [ &surfaceFormat ]( const VkSurfaceFormatKHR& supported ) noexcept
										{
											return supported.format		== surfaceFormat.format
												&& supported.colorSpace == surfaceFormat.colorSpace;
										};

									// ���� mode ��֧�ֵı��� �򷵻�
									if( stl::find_if( arrSupportes, f ) != arrSupportes.end() )
										{
											return surfaceFormat;
										}
								}

							// ��ѡ��һ�� ֧�ֵ�
							return firstSupported;
						}
				}

			return FORMAT_SEQUENCE[ 0 ];
		}


	/*
	 @	ѡ�� ������ �ߴ�
	 */
	FC_INLINE VkExtent2D VK_SwapChain::ChooseSize( const VK_WindowSurface& windowSurface,
												   const VkSurfaceCapabilitiesKHR& capabilities ) const noexcept
		{
			// ���֮һ���ó� 0xFFFFFFFF ��δ����
			if( capabilities.currentExtent.width == UInt32_MAX )
				{
					// ��ʼ���ߴ�Ϊ �ͻ�����ʾ�ߴ�
					VkExtent2D extent2D( windowSurface.superior.GetSize() );

					// ���ڿ�� ��֧�ֵķ�Χ֮��
					const VkExtent2D& minExtent
						{
							capabilities.minImageExtent
						};

					const VkExtent2D& maxExtent
						{
							capabilities.maxImageExtent
						};

					// ���� ���
					if( extent2D.width < minExtent.width )
						{
							extent2D.width = minExtent.width;
						}
					else if( extent2D.width > maxExtent.width )
						{
							extent2D.width = maxExtent.width;
						}

					// ���� �߶�
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
					// ��������� currentExtent ��ֱ��ʹ��
					return capabilities.currentExtent;
				}
		}


	/*
	 @	ѡ�� ������ ͼ������
	 */
	FC_INLINE uint32_t VK_SwapChain::ChooseCount( const VkSurfaceCapabilitiesKHR& capabilities ) const noexcept
		{
			// ��ѡ ������
			constexpr uint32_t count{ 3 };

			// ������ �������� Image
			const uint32_t minCount
				{
					capabilities.minImageCount
				};

			// maxImageCount Ϊ 0 ��û����������
			const uint32_t maxCount
				{
					capabilities.maxImageCount
				};

			// ȷ�� С�ڵ��� maxImageCount
			if( count > maxCount && maxCount > 0 )
				{
					return maxCount;
				}

			// ȷ�� ���ڵ��� minImageCount
			if( count < minCount )
				{
					return minCount;
				}

			// ������
			return count;
		}


	/*
	 @	ѡ�� �ͱ����Ļ�Ϸ�ʽ
	 */
	FC_INLINE VkCompositeAlphaFlagBitsKHR VK_SwapChain::ChooseAlpha( const VkSurfaceCapabilitiesKHR& capabilities ) const noexcept
		{
			VkCompositeAlphaFlagsKHR compositeBits{};

			// ��� composite alpha
			for( const VkCompositeAlphaFlagBitsKHR alphaFlag : ALPHA_GROUP )
				{
					if( capabilities.supportedCompositeAlpha & alphaFlag )
						{
							compositeBits |= alphaFlag;
						}
				}

			// ��� ��ѡ��֧�֣���ʹ��Ĭ��
			return static_cast< VkCompositeAlphaFlagBitsKHR >
					(
						compositeBits ? compositeBits
									  : capabilities.supportedCompositeAlpha
					);
		}


	/*
	 @	ѡ�� �任��ʽ
	 */
	FC_INLINE VkSurfaceTransformFlagBitsKHR VK_SwapChain::ChooseTransform( const VkSurfaceCapabilitiesKHR& capabilities ) const noexcept
		{
			VkSurfaceTransformFlagsKHR transformBits{};

			// ��� surface transform
			for( const VkSurfaceTransformFlagBitsKHR transformFlag : TRANSFORM_GROUP )
				{
					if( capabilities.supportedTransforms & transformFlag )
						{
							transformBits |= transformFlag;
						}
				}

			// ��� ��ѡ��֧�֣���ʹ��Ĭ��
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
	 @	��ȡ image �б�
	 */
	VkResult VK_SwapChain::GetImageList( ImageList& arrImages ) const noexcept
		{
			// ��ȡ image ����
			uint32_t count;

			VkResult res
				{
					vkGetSwapchainImagesKHR( s_VK_Device,
											 this->vkSwapchainKHR,
											 &count, nullptr )
				};

			// ��ȡ image �б�
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