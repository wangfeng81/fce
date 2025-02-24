/*
**
**	VK_Device.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	启用的 device 扩展列表
	 */
	constinit const char* const VK_Device::EXTENSION_GROUP[]
		{
			// 交换链
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
}



namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_Device::VK_Device() noexcept
		:
			vkDevice( VK_NULL_HANDLE )
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	Wait idle
	 */
	VkResult VK_Device::WaitIdle() const noexcept
		{
			VkResult res
				{
					VK_ERROR_DEVICE_LOST
				};

			if( this->vkDevice )
				{
					VK_ERR_GUARD( vkDeviceWaitIdle( this->vkDevice ) );
				}

			return res;
		}


	/*
	 @	获取 queue
	 */
	FC_INLINE void VK_Device::GetQueues() noexcept
		{
			// 获取 Graphics 队列
			vkGetDeviceQueue( this->vkDevice,
							  s_VK_PhyDevice->queueFamilies.graph,
							  0,
							  &this->config.queues.graph );

			// 获取 Presentation 队列
			vkGetDeviceQueue( this->vkDevice,
							  s_VK_PhyDevice->queueFamilies.present,
							  0,
							  &this->config.queues.present );
		}


	/*
	 @	设置 需要的设备特性
	 */
	FC_INLINE void VK_Device::SetFeatures( VkPhysicalDeviceFeatures& features ) noexcept
		{
			// 缓冲区 越界检查
			#if defined( FC_SECURITY ) || defined( FC_DEBUG )

				features.robustBufferAccess = VK_TRUE;

			#endif

			// 各向异性过滤
			features.samplerAnisotropy = VK_TRUE;

			// 宽线
			//features.wideLines = VK_TRUE;

			// 点 线 填充模式
			features.fillModeNonSolid = VK_TRUE;

			// 32位 索引
			//features.fullDrawIndexUint32 = VK_TRUE;

			// 每个附件独立控制 VkPipelineColorBlendAttachmentState
			//features.independentBlend = VK_TRUE;

			// 几何着色
			//features.geometryShader = VK_TRUE;

			// 多重采样插值
			//features.sampleRateShading = VK_TRUE;

			// 两个源的混合
			//features.dualSrcBlend = VK_TRUE;

			// 逻辑混合
			//features.logicOp = VK_TRUE;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	处理 设备丢失
	 */
	FC_INLINE bool VK_Device::on_Lost() noexcept
		{
			return this->Rebuild() == VK_SUCCESS;
		}


	/*
	 @	重建
	 */
	FC_INLINE VkResult VK_Device::Rebuild() noexcept
		{
			this->Destroy();

			return this->Create();
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁
	 */
	void VK_Device::Destroy() noexcept
		{
			// 销毁 逻辑设备
			if( this->vkDevice )
				{
					vkDestroyDevice( this->vkDevice,
									 s_VK_Allocator );
				}
		}


	/*
	 @	创建
	 */
	VkResult VK_Device::Create() noexcept
		{
			VkResult res;

			// 设置 需要的设备特性
			VkPhysicalDeviceFeatures features = {};

			this->SetFeatures( features );

			// 设置 queue create info 列表
			constexpr Float32 queuePriorities[ 2 ]
				{
					1.0f, 1.0f
				};

			VkDeviceQueueCreateInfo queueCreateInfos[ 2 ];

			for( uint32_t i{ 0 }; i < s_VK_PhyDevice->queueFamilies.count; ++i )
				{
					queueCreateInfos[ i ] =
						{
							.sType			  = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
							.pNext			  = nullptr,
							.flags			  = 0,
							.queueFamilyIndex = s_VK_PhyDevice->queueFamilies.data[ i ],
							.queueCount		  = 1,
							.pQueuePriorities = queuePriorities + i
						};
				}

			// 获取 extension count
			constexpr uint32_t EXTENSION_COUNT
				{
					static_cast< uint32_t >
					( std::ranges::size( EXTENSION_GROUP ) )
				};

			// 设置 device create info
			const VkDeviceCreateInfo deviceCreateInfo
				{
					.sType					 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
					.pNext					 = nullptr,
					.flags					 = 0,
					.queueCreateInfoCount	 = s_VK_PhyDevice->queueFamilies.count,
					.pQueueCreateInfos		 = queueCreateInfos,
					.enabledLayerCount		 = 0,
					.ppEnabledLayerNames	 = nullptr,
					.enabledExtensionCount	 = EXTENSION_COUNT,
					.ppEnabledExtensionNames = EXTENSION_GROUP,
					.pEnabledFeatures		 = &features
				};
	
			// 创建 逻辑设备
			VK_ERR_GUARD( vkCreateDevice( s_VK_PhyDevice,
										  &deviceCreateInfo,
										  s_VK_Allocator,
										  &this->vkDevice ) );
		
			if( res == VK_SUCCESS )
				{
					// 动态加载 设备级 函数接口
					#ifdef VK_NO_PROTOTYPES

						volkLoadDevice( this->vkDevice );

					#endif

					// 获取 queues
					this->GetQueues();
				}

			return res;
		}
}