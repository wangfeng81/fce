/*
**
**	VK_PhysicalDevice.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_PhysicalDevice::VK_PhysicalDevice() noexcept
		:
			vkPhysicalDevice( VK_NULL_HANDLE )
		{
		}

	/*
	 @	初始化
	 */
	FC_INLINE VkResult VK_PhysicalDevice::Initialize() noexcept
		{
			// 获取 显卡列表
			PhyDeviceList listPhyDevices;

			VkResult res
				{
					GetPhyDeviceList( listPhyDevices )
				};

			if( res == VK_SUCCESS )
				{
					// 选取 显卡
					if( this->vkPhysicalDevice =
							this->PickPhyDevice( listPhyDevices ) )
						{
							// 匹配 Queue family
							res = this->MatchQueueFamily();

							if( res == VK_SUCCESS )
								{
									// 匹配 Sharing mode
									this->MatchSharingMode();
								}
						}
					else
						{
							res = VK_ERROR_UNKNOWN;
						}
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	匹配 Sharing mode
	 */
	FC_INLINE void VK_PhysicalDevice::MatchSharingMode() noexcept
		{
			// 如果 graph 和 present 为同一队列簇，则用 EXCLUSIVE
			this->config.sharingMode = this->config.queueFamilies.count == 1
									 ? VK_SHARING_MODE_EXCLUSIVE
									 : VK_SHARING_MODE_CONCURRENT;
		}


	/*
	 @	匹配 队列族 索引

			支持 graphics 和 presentation
	 */
	FC_INLINE VkResult VK_PhysicalDevice::MatchQueueFamily() noexcept
		{
			// 获取 队列族 列表
			QueueFamilyList listQueueFamilies;

			VkResult res
				{
					GetQueueFamilyList( listQueueFamilies,
										this->vkPhysicalDevice )
				};

			if( res == VK_SUCCESS )
				{
					// 获取 队列族数量
					const uint32_t count
						{
							static_cast< uint32_t >
								(
									listQueueFamilies.size()
								)
						};

					// 遍历 队列族列表
					for( uint32_t idx{ 0 }; idx < count; ++idx )
						{
							// 检测是否支持 presentation
							const VkBool32 presentation
								{
									this->CheckPresentation( idx )
								};

							// 检测是否支持 graphics
							const VkBool32 bGraph
								{
									listQueueFamilies[ idx ].queueFlags
									& VK_QUEUE_GRAPHICS_BIT
								};

							// 优先寻找 两者都支持的 队列
							if( bGraph && presentation )
								{
									this->config.queueFamilies.present = idx;
									this->config.queueFamilies.graph = idx;

									this->config.queueFamilies.count = 1;

									return VK_SUCCESS;
								}
							else if( presentation )
								{
									// 保存 presentation 队列索引
									this->config.queueFamilies.present = idx;
								}
							else if( bGraph )
								{
									// 保存 graphics 队列索引
									this->config.queueFamilies.graph = idx;
								}
						}
				}
			else
				{
					// 没有可用的队列
					return res;
				}

			// 两个队列分别支持 graphics 和 presentation
			this->config.queueFamilies.count = 2;

			return res;
		}


	/*
	 @	选取 显卡
	 */
	FC_INLINE VkPhysicalDevice VK_PhysicalDevice::PickPhyDevice( const PhyDeviceList& listPhyDevices ) const noexcept
		{
			// 遍历显卡列表
			for( const VkPhysicalDevice phyDevice : listPhyDevices )
				{
					// 取第一个满足要求的设备
					if( this->CheckPhyDevice( phyDevice ) )
						{
							return phyDevice;
						}
				}

			return nullptr;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	检测 队列族支持 presentation
	 */
	FC_INLINE VkBool32 VK_PhysicalDevice::CheckPresentation( const uint32_t queueFamilyIndex ) const noexcept
		{
			#ifdef FC_WINDOWS
				{
					return vkGetPhysicalDeviceWin32PresentationSupportKHR( this->vkPhysicalDevice,
																		   queueFamilyIndex );
				}
			#elif defined FC_LINUX
				{
					return vkGetPhysicalDeviceXcbPresentationSupportKHR( this->physicalDevice,
																		 queueFamilyIndex, 0, 0 );
				}
			#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )
				{
					return vkGetPhysicalDeviceWaylandPresentationSupportKHR( this->physicalDevice,
																			 queueFamilyIndex, nullptr );
				}
			#else

					// 单件窗口
					VkBool32 presentation;

					if( vkGetPhysicalDeviceSurfaceSupportKHR( this->physicalDevice,
															  queueFamilyIndex,
															  s_Window.surface.impl,
															  &presentation ) == VK_SUCCESS )
						{
							return presentation;
						}

					return VK_FALSE;

			#endif
		}


	/*
	 @	检测 显卡是否满足要求
	 */
	FC_INLINE VkBool32 VK_PhysicalDevice::CheckPhyDevice( const VkPhysicalDevice phyDevice ) const noexcept
		{
			// 获取属性
			VkPhysicalDeviceProperties deviceProperties;

			vkGetPhysicalDeviceProperties( phyDevice,
										   &deviceProperties );

			// 获取特征
			VkPhysicalDeviceFeatures deviceFeatures;

			vkGetPhysicalDeviceFeatures( phyDevice,
										 &deviceFeatures );

			// 检测需求
			return deviceProperties.deviceType ==
					VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	获取 显卡 列表
	 */
	VkResult VK_PhysicalDevice::GetPhyDeviceList( PhyDeviceList& arrPhyDevices ) noexcept
		{
			// 获取 PhyDevice 数量
			uint32_t count;

			VkResult res
				{
					vkEnumeratePhysicalDevices( s_VK_Instance,
												&count, nullptr )
				};

			// 获取 PhyDevice 列表
			if( res == VK_SUCCESS && count )
				{
					arrPhyDevices.resize( count );

					res = vkEnumeratePhysicalDevices( s_VK_Instance, &count,
													  arrPhyDevices.data() );
				}

			return res;
		}


	/*
	 @	获取 队列族 列表
	 */
	VkResult VK_PhysicalDevice::GetQueueFamilyList( QueueFamilyList& arrQueueFamilies,
													const VkPhysicalDevice phyDevice ) noexcept
		{
			// 获取 队列族数量
			uint32_t count;

			vkGetPhysicalDeviceQueueFamilyProperties( phyDevice,
													  &count, nullptr );

			if( count )
				{
					// 设置 队列族 列表数量
					arrQueueFamilies.resize( count );

					// 获取 队列族列表
					vkGetPhysicalDeviceQueueFamilyProperties( phyDevice, &count,
															  arrQueueFamilies.data() );

					return VK_SUCCESS;
				}

			return VK_ERROR_UNKNOWN;
		}


	/*
	 @	获取 物理设备 扩展表
	 */
	VkResult VK_PhysicalDevice::GetExtensionList( ExtensionList& arrDevicExtensions,
												  const VkPhysicalDevice phyDevice,
												  const char* const layerName ) noexcept
		{
			// 获取 extension 数量
			uint32_t count;

			VkResult res
				{
					vkEnumerateDeviceExtensionProperties( phyDevice,
														  layerName,
														  &count, VK_NULL_HANDLE )
				};

			// 获取 extension 列表
			if( res == VK_SUCCESS && count )
				{
					arrDevicExtensions.resize( count );

					res = vkEnumerateDeviceExtensionProperties( phyDevice,
																layerName, &count,
																arrDevicExtensions.data() );
				}

			return res;
		}
}