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
	 @	����
	 */
	FC_INLINE VK_PhysicalDevice::VK_PhysicalDevice() noexcept
		:
			vkPhysicalDevice( VK_NULL_HANDLE )
		{
		}

	/*
	 @	��ʼ��
	 */
	FC_INLINE VkResult VK_PhysicalDevice::Initialize() noexcept
		{
			// ��ȡ �Կ��б�
			PhyDeviceList listPhyDevices;

			VkResult res
				{
					GetPhyDeviceList( listPhyDevices )
				};

			if( res == VK_SUCCESS )
				{
					// ѡȡ �Կ�
					if( this->vkPhysicalDevice =
							this->PickPhyDevice( listPhyDevices ) )
						{
							// ƥ�� Queue family
							res = this->MatchQueueFamily();

							if( res == VK_SUCCESS )
								{
									// ƥ�� Sharing mode
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
	 @	ƥ�� Sharing mode
	 */
	FC_INLINE void VK_PhysicalDevice::MatchSharingMode() noexcept
		{
			// ��� graph �� present Ϊͬһ���дأ����� EXCLUSIVE
			this->config.sharingMode = this->config.queueFamilies.count == 1
									 ? VK_SHARING_MODE_EXCLUSIVE
									 : VK_SHARING_MODE_CONCURRENT;
		}


	/*
	 @	ƥ�� ������ ����

			֧�� graphics �� presentation
	 */
	FC_INLINE VkResult VK_PhysicalDevice::MatchQueueFamily() noexcept
		{
			// ��ȡ ������ �б�
			QueueFamilyList listQueueFamilies;

			VkResult res
				{
					GetQueueFamilyList( listQueueFamilies,
										this->vkPhysicalDevice )
				};

			if( res == VK_SUCCESS )
				{
					// ��ȡ ����������
					const uint32_t count
						{
							static_cast< uint32_t >
								(
									listQueueFamilies.size()
								)
						};

					// ���� �������б�
					for( uint32_t idx{ 0 }; idx < count; ++idx )
						{
							// ����Ƿ�֧�� presentation
							const VkBool32 presentation
								{
									this->CheckPresentation( idx )
								};

							// ����Ƿ�֧�� graphics
							const VkBool32 bGraph
								{
									listQueueFamilies[ idx ].queueFlags
									& VK_QUEUE_GRAPHICS_BIT
								};

							// ����Ѱ�� ���߶�֧�ֵ� ����
							if( bGraph && presentation )
								{
									this->config.queueFamilies.present = idx;
									this->config.queueFamilies.graph = idx;

									this->config.queueFamilies.count = 1;

									return VK_SUCCESS;
								}
							else if( presentation )
								{
									// ���� presentation ��������
									this->config.queueFamilies.present = idx;
								}
							else if( bGraph )
								{
									// ���� graphics ��������
									this->config.queueFamilies.graph = idx;
								}
						}
				}
			else
				{
					// û�п��õĶ���
					return res;
				}

			// �������зֱ�֧�� graphics �� presentation
			this->config.queueFamilies.count = 2;

			return res;
		}


	/*
	 @	ѡȡ �Կ�
	 */
	FC_INLINE VkPhysicalDevice VK_PhysicalDevice::PickPhyDevice( const PhyDeviceList& listPhyDevices ) const noexcept
		{
			// �����Կ��б�
			for( const VkPhysicalDevice phyDevice : listPhyDevices )
				{
					// ȡ��һ������Ҫ����豸
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
	 @	��� ������֧�� presentation
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

					// ��������
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
	 @	��� �Կ��Ƿ�����Ҫ��
	 */
	FC_INLINE VkBool32 VK_PhysicalDevice::CheckPhyDevice( const VkPhysicalDevice phyDevice ) const noexcept
		{
			// ��ȡ����
			VkPhysicalDeviceProperties deviceProperties;

			vkGetPhysicalDeviceProperties( phyDevice,
										   &deviceProperties );

			// ��ȡ����
			VkPhysicalDeviceFeatures deviceFeatures;

			vkGetPhysicalDeviceFeatures( phyDevice,
										 &deviceFeatures );

			// �������
			return deviceProperties.deviceType ==
					VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	��ȡ �Կ� �б�
	 */
	VkResult VK_PhysicalDevice::GetPhyDeviceList( PhyDeviceList& arrPhyDevices ) noexcept
		{
			// ��ȡ PhyDevice ����
			uint32_t count;

			VkResult res
				{
					vkEnumeratePhysicalDevices( s_VK_Instance,
												&count, nullptr )
				};

			// ��ȡ PhyDevice �б�
			if( res == VK_SUCCESS && count )
				{
					arrPhyDevices.resize( count );

					res = vkEnumeratePhysicalDevices( s_VK_Instance, &count,
													  arrPhyDevices.data() );
				}

			return res;
		}


	/*
	 @	��ȡ ������ �б�
	 */
	VkResult VK_PhysicalDevice::GetQueueFamilyList( QueueFamilyList& arrQueueFamilies,
													const VkPhysicalDevice phyDevice ) noexcept
		{
			// ��ȡ ����������
			uint32_t count;

			vkGetPhysicalDeviceQueueFamilyProperties( phyDevice,
													  &count, nullptr );

			if( count )
				{
					// ���� ������ �б�����
					arrQueueFamilies.resize( count );

					// ��ȡ �������б�
					vkGetPhysicalDeviceQueueFamilyProperties( phyDevice, &count,
															  arrQueueFamilies.data() );

					return VK_SUCCESS;
				}

			return VK_ERROR_UNKNOWN;
		}


	/*
	 @	��ȡ �����豸 ��չ��
	 */
	VkResult VK_PhysicalDevice::GetExtensionList( ExtensionList& arrDevicExtensions,
												  const VkPhysicalDevice phyDevice,
												  const char* const layerName ) noexcept
		{
			// ��ȡ extension ����
			uint32_t count;

			VkResult res
				{
					vkEnumerateDeviceExtensionProperties( phyDevice,
														  layerName,
														  &count, VK_NULL_HANDLE )
				};

			// ��ȡ extension �б�
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