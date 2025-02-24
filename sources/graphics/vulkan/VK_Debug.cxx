/*
**
**	VK_Debug.
**
		Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{

	/*
	 @	启用的 layer 列表
	 */
	constinit const char* const VK_Debug::LAYER_GROUP[]
		{
			"VK_LAYER_KHRONOS_validation",
		};

	/*
	 @	启用的 report 标志
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
	 @	构造
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
	 @	创建 调试报告
	 */
	FC_INLINE VkResult VK_Debug::Create() noexcept
		{
			VkResult res{ VK_ERROR_UNKNOWN };

			// 设置 create info
			constexpr VkDebugReportCallbackCreateInfoEXT createInfo
				{
					.sType		 = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
					.pNext		 = nullptr,
					.flags		 = REPORT_FLAGS,
					.pfnCallback = ProcessReport,
					.pUserData	 = nullptr
				};

			// 获取函数 vkCreateDebugReportCallbackEXT
			if( auto vkCreateDebugReportCallbackEXT = ( PFN_vkCreateDebugReportCallbackEXT )
													  vkGetInstanceProcAddr( s_VK_Instance,
																			 "vkCreateDebugReportCallbackEXT" ) )
				{
					// 创建 调试报告
					res = vkCreateDebugReportCallbackEXT( s_VK_Instance,
														  &createInfo,
														  s_VK_Allocator,
														  &this->vkReportCallback );
				}

			return res;
		}


	/*
	 @	销毁 调试报告
	 */
	FC_INLINE void VK_Debug::Destroy() noexcept
		{
			if( this->vkReportCallback )
				{
					// 获取函数 vkDestroyDebugReportCallbackEXT
					if( auto vkDestroyDebugReportCallbackEXT = ( PFN_vkDestroyDebugReportCallbackEXT )
															   vkGetInstanceProcAddr( s_VK_Instance,
																					  "vkDestroyDebugReportCallbackEXT" ) )
						{
							// 销毁 调试报告
							vkDestroyDebugReportCallbackEXT( s_VK_Instance,
															 this->vkReportCallback,
															 s_VK_Allocator );
						}
				}
		}


	/*
	 @	响应 调试报告的 回调函数
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

			// 根据类型 分别提示
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
	 @	检测 启用的 layer 列表 是否都支持
	 */
	VkBool32 VK_Debug::CheckLayers() noexcept
		{
			// 获取 layer propertie 列表
			LayerList arrSupportes;

			if( GetLayerList( arrSupportes ) == VK_SUCCESS )
				{
					// 遍历 需要开启的 layer 列表
					for( const char* const layer : LAYER_GROUP )
						{
							// 检测是否存在
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
	 @	检测 layer 是否存在
	 */
	VkBool32 VK_Debug::hasLayer( const char* const name,
								 const LayerList& arrLayers ) noexcept
		{
			/*
			 @	比较 层名字
			 */
			auto f = [ name ]( const VkLayerProperties& p ) NOEXCEPT
				{
					return ! strcmp( p.layerName, name );
				};

			// 查找 debug 校验层 是否存在
			return stl::find_if( arrLayers, f )
				!= arrLayers.end();
		}


	/*
	 @	获取 layer propertie 列表
	 */
	VkResult VK_Debug::GetLayerList( LayerList& arrLayers ) noexcept
		{
			// 获取 layer 数量
			uint32_t count;

			VkResult res
				{
					vkEnumerateInstanceLayerProperties( &count,
														VK_NULL_HANDLE )
				};

			// 获取 layer 列表
			if( res == VK_SUCCESS && count )
				{
					arrLayers.resize( count );

					res = vkEnumerateInstanceLayerProperties( &count,
															  arrLayers.data() );
				}

			return res;
		}
}