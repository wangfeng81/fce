/*
**
**	VK_Debug.
**
		Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{

	/*
	 @	���õ� layer �б�
	 */
	constinit const char* const VK_Debug::LAYER_GROUP[]
		{
			"VK_LAYER_KHRONOS_validation",
		};

	/*
	 @	���õ� report ��־
	 */
	constinit const VkDebugReportFlagsEXT VK_Debug::REPORT_FLAGS = VK_DEBUG_REPORT_ERROR_BIT_EXT
															     | VK_DEBUG_REPORT_WARNING_BIT_EXT
															     | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
															     | VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
															   //| VK_DEBUG_REPORT_DEBUG_BIT_EXT;
}



namespace fce::inner::vulkan
{
	/*
	 @	����
	 */
	FC_INLINE VK_Debug::VK_Debug() noexcept
		:
			vkReportCallback( VK_NULL_HANDLE )
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	���� ���Ա���
	 */
	FC_INLINE VkResult VK_Debug::Create() noexcept
		{
			VkResult res{ VK_ERROR_UNKNOWN };

			// ���� create info
			constexpr VkDebugReportCallbackCreateInfoEXT createInfo
				{
					.sType		 = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
					.pNext		 = nullptr,
					.flags		 = REPORT_FLAGS,
					.pfnCallback = ProcessReport,
					.pUserData	 = nullptr
				};

			// ��ȡ���� vkCreateDebugReportCallbackEXT
			if( auto vkCreateDebugReportCallbackEXT = ( PFN_vkCreateDebugReportCallbackEXT )
													  vkGetInstanceProcAddr( s_VK_Instance,
																			 "vkCreateDebugReportCallbackEXT" ) )
				{
					// ���� ���Ա���
					res = vkCreateDebugReportCallbackEXT( s_VK_Instance,
														  &createInfo,
														  s_VK_Allocator,
														  &this->vkReportCallback );
				}

			return res;
		}


	/*
	 @	���� ���Ա���
	 */
	FC_INLINE void VK_Debug::Destroy() noexcept
		{
			if( this->vkReportCallback )
				{
					// ��ȡ���� vkDestroyDebugReportCallbackEXT
					if( auto vkDestroyDebugReportCallbackEXT = ( PFN_vkDestroyDebugReportCallbackEXT )
															   vkGetInstanceProcAddr( s_VK_Instance,
																					  "vkDestroyDebugReportCallbackEXT" ) )
						{
							// ���� ���Ա���
							vkDestroyDebugReportCallbackEXT( s_VK_Instance,
															 this->vkReportCallback,
															 s_VK_Allocator );
						}
				}
		}


	/*
	 @	��Ӧ ���Ա���� �ص�����
	 */
	VKAPI_ATTR VkBool32 VKAPI_CALL VK_Debug::ProcessReport( VkDebugReportFlagsEXT msgFlags,
															VkDebugReportObjectTypeEXT objType,
															uint64_t srcObject,
															size_t location,
															int32_t msgCode,
															const char* const layerPrefix,
															const char* const msg,
															void* userData ) noexcept
		{
			const char* const fmt{ "Vulkan %s : %s [ code = %d ] : %s" };

			// �������� �ֱ���ʾ
			if( msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT )
				{
					s_Window.ErrorBox( fmt, layerPrefix, "ERROR", msgCode, msg );
				}
			else if( msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT )
				{
					FC_PROMPT( fmt, layerPrefix, "WARNING", msgCode, msg );
				}
			else if( msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT )
				{
					FC_PROMPT( fmt, layerPrefix, "PERFORMANCE WARNING", msgCode, msg );
				}
			else if( msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT )
				{
					FC_LOG( fmt, layerPrefix, "INFO", msgCode, msg );
				}
			else if( msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT )
				{
					FC_LOG( fmt, layerPrefix, "DEBUG", msgCode, msg );
				}

			return VK_FALSE;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	��� ���õ� layer �б� �Ƿ�֧��
	 */
	VkBool32 VK_Debug::CheckLayers() noexcept
		{
			// ��ȡ layer propertie �б�
			LayerList arrSupportes;

			if( GetLayerList( arrSupportes ) == VK_SUCCESS )
				{
					// ���� ��Ҫ������ layer �б�
					for( const char* const layer : LAYER_GROUP )
						{
							// ����Ƿ����
							if( ! hasLayer( layer,
											arrSupportes ) )
								{
									return VK_FALSE;
								}
						}

					return VK_TRUE;
				}

			return VK_FALSE;
		}


	/*
	 @	��� layer �Ƿ����
	 */
	VkBool32 VK_Debug::hasLayer( const char* const name,
								 const LayerList& arrLayers ) noexcept
		{
			/*
			 @	�Ƚ� ������
			 */
			auto f = [ name ]( const VkLayerProperties& p ) NOEXCEPT
				{
					return ! strcmp( p.layerName, name );
				};

			// ���� debug У��� �Ƿ����
			return stl::find_if( arrLayers, f )
				!= arrLayers.end();
		}


	/*
	 @	��ȡ layer propertie �б�
	 */
	VkResult VK_Debug::GetLayerList( LayerList& arrLayers ) noexcept
		{
			// ��ȡ layer ����
			uint32_t count;

			VkResult res
				{
					vkEnumerateInstanceLayerProperties( &count,
														VK_NULL_HANDLE )
				};

			// ��ȡ layer �б�
			if( res == VK_SUCCESS && count )
				{
					arrLayers.resize( count );

					res = vkEnumerateInstanceLayerProperties( &count,
															  arrLayers.data() );
				}

			return res;
		}
}