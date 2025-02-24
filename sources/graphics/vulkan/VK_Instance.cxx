/*
**
**	VK_Instance.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

#ifdef FC_DEBUG

	#include "VK_Debug.cxx"

#endif


namespace fce::inner::vulkan
{
	/*
	 @	启用的 instance 扩展列表
	 */
	constinit const char* const VK_Instance::EXTENSION_GROUP[]
		{
			// 本地窗口 surface 扩展
			VK_KHR_SURFACE_EXTENSION_NAME,

			// 各平台窗口 surface 名字
			#ifdef FC_WINDOWS
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
			#elif defined FC_ANDROID
				VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
			#elif defined FC_LINUX
				VK_KHR_XCB_SURFACE_EXTENSION_NAME,
			#elif defined FC_IOS
				VK_EXT_METAL_SURFACE_EXTENSION_NAME
				//VK_MVK_IOS_SURFACE_EXTENSION_NAME,
			#elif defined FC_MAC
				VK_MVK_MACOS_SURFACE_EXTENSION_NAME,
			#endif

			// 调试报告
			#ifdef FC_DEBUG
				VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
				VK_EXT_DEBUG_REPORT_EXTENSION_NAME
			#endif
		};

	/*
	 @	启用的 API 版本

			VK_MAKE_API_VERSION( 0, 1, 1, 0 );
	 */
	constinit const uint32_t VK_Instance::API_VERSION = VK_API_VERSION_1_1;
}



namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_Instance::VK_Instance() noexcept
		:
			vkInstance( VK_NULL_HANDLE )
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁
	 */
	FC_INLINE void VK_Instance::Destroy() noexcept
		{
			// 销毁 debug
			#ifdef FC_DEBUG

				this->debug.Destroy();

			#endif

			// 销毁 instance
			this->DestroyInstance();
		}


	/*
	 @	创建
	 */
	FC_INLINE VkResult VK_Instance::Create() noexcept
		{
			// 创建 instance
			VkResult res
				{
					this->CreateInstance()
				};

			if( res == VK_SUCCESS )
				{
					// 创建 debug
					#ifdef FC_DEBUG

						VK_ERR_RETURN( this->debug.Create() );

					#endif

					// 提前在 window::Surface 之前创建 单件窗口 surface
					res = this->CreateSingleSurface();
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁 instance
	 */
	FC_INLINE void VK_Instance::DestroyInstance() noexcept
		{
			// 销毁 instance
			if( this->vkInstance )
				{
					vkDestroyInstance( this->vkInstance,
									   s_VK_Allocator );
				}
		}


	/*
	 @	创建 instance
	 */
	FC_INLINE VkResult VK_Instance::CreateInstance() noexcept
		{
			// 设置 layer 列表
			#ifdef FC_DEBUG

				constexpr uint32_t LAYER_COUNT
					{
						static_cast< uint32_t >
						(
							std::ranges::size
							( VK_Debug::LAYER_GROUP )
						)
					};

				constexpr auto layerList
					{
						VK_Debug::LAYER_GROUP
					};

				const uint32_t layerCount
					{
						s_VK_Debug.CheckLayers()
						? LAYER_COUNT : 0
					};

			#else

				constexpr auto layerList{ nullptr };

				constexpr uint32_t layerCount{ 0 };

			#endif

			// 获取 extension count
			constexpr uint32_t EXTENSION_COUNT
				{
					static_cast< uint32_t >
					( std::ranges::size( EXTENSION_GROUP ) )
				};

			// 设置 App info
			constexpr VkApplicationInfo appInfo
				{
					.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO,
					.pNext				= nullptr,
					.pApplicationName	= "fce",
					.applicationVersion	= 0,
					.pEngineName		= "fce",
					.engineVersion		= 0,
					.apiVersion			= API_VERSION
				};

			// 设置 Create info
			const VkInstanceCreateInfo createInfo
				{
					.sType					 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
					.pNext					 = nullptr,
					.flags					 = 0,
					.pApplicationInfo		 = &appInfo,
					.enabledLayerCount		 = layerCount,
					.ppEnabledLayerNames	 = layerList,
					.enabledExtensionCount	 = EXTENSION_COUNT,
					.ppEnabledExtensionNames = EXTENSION_GROUP
				};
			
			// 创建 Instance
			VkResult res
				{
					vkCreateInstance( &createInfo,
									  s_VK_Allocator,
									  &this->vkInstance )
				};

			if( res == VK_SUCCESS )
				{
					// 动态加载 实例级 函数接口
					#ifdef VK_NO_PROTOTYPES

						volkLoadInstance( this->vkInstance );

					#endif
				}

			return res;
		}


	/*
	 @	创建 平台窗口的 展示页面

			单件窗口

			有些平台下，在 VK_PhysicalDevice::MatchQueueFamily 获取 队列族索引 需要依赖 surface 就必须提起创建
	 */
	FC_INLINE VkResult VK_Instance::CreateSingleSurface() noexcept
		{
			// 主窗口 VkSurfaceKHR
			VkSurfaceKHR& vkSurfaceKHR
				{
					s_Window.surface.impl.vkSurfaceKHR
				};

			#ifdef FC_ANDROID
				{
					const VkAndroidSurfaceCreateInfoKHR createInfo
						{
							.sType	= VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
							.pNext	= nullptr,
							.flags	= 0,
							.window	= s_Window.impl
						};

					return vkCreateAndroidSurfaceKHR( this->instance,
													  &createInfo,
													  s_VK_Allocator,
													  &vkSurfaceKHR );
				}
			#elif defined FC_IOS
				{
					const VkIOSSurfaceCreateInfoMVK createInfo
						{
							.sType	= VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK,
							.pNext	= nullptr,
							.flags	= 0,
							.pView	= s_Window.impl
						};

					// vkCreateMetalSurfaceEXT
					return vkCreateIOSSurfaceMVK( this->instance,
												  &createInfo,
												  s_VK_Allocator,
												  &vkSurfaceKHR );
				}
			#elif defined FC_MAC
				{
					const VkMacOSSurfaceCreateInfoMVK createInfo
						{
							.sType	= VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
							.pNext	= nullptr,
							.flags	= 0,
							.pView	= s_Window.impl
						};

					return vkCreateMacOSSurfaceMVK( this->instance,
													&createInfo,
													s_VK_Allocator,
													&vkSurfaceKHR );
				}
			#else

					// 延迟在 VK_WindowSurface 创建 多窗口支持
					return VK_SUCCESS;

			#endif
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	检测 启用的 extension 列表 是否都支持
	 */
	VkBool32 VK_Instance::CheckExtensions( const char* const layerName ) noexcept
		{
			// 获取 extension 列表
			ExtensionList arrSupportes;

			if( GetExtensionList( arrSupportes,
								  layerName ) == VK_SUCCESS )
				{
					// 遍历 需要启用的 extension 列表
					for( const char* const extension : EXTENSION_GROUP )
						{
							// 检测是否支持
							if( ! hasExtension( extension,
												arrSupportes ) )
								{
									return VK_FALSE;
								}
						}
				}

			return VK_TRUE;
		}


	/*
	 @	检测 单个 extension 是否存在
	 */
	VkBool32 VK_Instance::hasExtension( const char* const name,
										const ExtensionList& arrExtensions ) noexcept
		{
			/*
			 @	比较 extension 名字
			 */
			auto f = [ name ]( const VkExtensionProperties& extProps ) NOEXCEPT
				{
					return ! strcmp( extProps.extensionName, name );
				};

			// 查找 extension 是否存在
			return stl::find_if( arrExtensions, f )
				!= arrExtensions.end();
		}


	/*
	 @	获取 实例扩展表
	 */
	VkResult VK_Instance::GetExtensionList( ExtensionList& arrExtensions,
											const char* const layerName ) noexcept
		{
			// 获取 extension 数量
			uint32_t count;

			VkResult res
				{
					vkEnumerateInstanceExtensionProperties( layerName, &count,
															VK_NULL_HANDLE )
				};

			// 获取 extension 列表
			if( res == VK_SUCCESS && count )
				{
					arrExtensions.resize( count );

					res = vkEnumerateInstanceExtensionProperties( layerName, &count,
																  arrExtensions.data() );
				}

			return res;
		}


	/*
	 @	获取 本机安装版本
	 */
	//uint32_t VK_Instance::GetVersion() const
	//	{
	//		uint32_t apiVersion;

	//		vkEnumerateInstanceVersion( &apiVersion );

	//		return apiVersion;
	//	}
}