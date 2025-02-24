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
	 @	���õ� device ��չ�б�
	 */
	constinit const char* const VK_Device::EXTENSION_GROUP[]
		{
			// ������
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
}



namespace fce::inner::vulkan
{
	/*
	 @	����
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
	 @	��ȡ queue
	 */
	FC_INLINE void VK_Device::GetQueues() noexcept
		{
			// ��ȡ Graphics ����
			vkGetDeviceQueue( this->vkDevice,
							  s_VK_PhyDevice->queueFamilies.graph,
							  0,
							  &this->config.queues.graph );

			// ��ȡ Presentation ����
			vkGetDeviceQueue( this->vkDevice,
							  s_VK_PhyDevice->queueFamilies.present,
							  0,
							  &this->config.queues.present );
		}


	/*
	 @	���� ��Ҫ���豸����
	 */
	FC_INLINE void VK_Device::SetFeatures( VkPhysicalDeviceFeatures& features ) noexcept
		{
			// ������ Խ����
			#if defined( FC_SECURITY ) || defined( FC_DEBUG )

				features.robustBufferAccess = VK_TRUE;

			#endif

			// �������Թ���
			features.samplerAnisotropy = VK_TRUE;

			// ����
			//features.wideLines = VK_TRUE;

			// �� �� ���ģʽ
			features.fillModeNonSolid = VK_TRUE;

			// 32λ ����
			//features.fullDrawIndexUint32 = VK_TRUE;

			// ÿ�������������� VkPipelineColorBlendAttachmentState
			//features.independentBlend = VK_TRUE;

			// ������ɫ
			//features.geometryShader = VK_TRUE;

			// ���ز�����ֵ
			//features.sampleRateShading = VK_TRUE;

			// ����Դ�Ļ��
			//features.dualSrcBlend = VK_TRUE;

			// �߼����
			//features.logicOp = VK_TRUE;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	���� �豸��ʧ
	 */
	FC_INLINE bool VK_Device::on_Lost() noexcept
		{
			return this->Rebuild() == VK_SUCCESS;
		}


	/*
	 @	�ؽ�
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
	 @	����
	 */
	void VK_Device::Destroy() noexcept
		{
			// ���� �߼��豸
			if( this->vkDevice )
				{
					vkDestroyDevice( this->vkDevice,
									 s_VK_Allocator );
				}
		}


	/*
	 @	����
	 */
	VkResult VK_Device::Create() noexcept
		{
			VkResult res;

			// ���� ��Ҫ���豸����
			VkPhysicalDeviceFeatures features = {};

			this->SetFeatures( features );

			// ���� queue create info �б�
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

			// ��ȡ extension count
			constexpr uint32_t EXTENSION_COUNT
				{
					static_cast< uint32_t >
					( std::ranges::size( EXTENSION_GROUP ) )
				};

			// ���� device create info
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
	
			// ���� �߼��豸
			VK_ERR_GUARD( vkCreateDevice( s_VK_PhyDevice,
										  &deviceCreateInfo,
										  s_VK_Allocator,
										  &this->vkDevice ) );
		
			if( res == VK_SUCCESS )
				{
					// ��̬���� �豸�� �����ӿ�
					#ifdef VK_NO_PROTOTYPES

						volkLoadDevice( this->vkDevice );

					#endif

					// ��ȡ queues
					this->GetQueues();
				}

			return res;
		}
}