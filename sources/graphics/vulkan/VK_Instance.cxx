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
	 @	���õ� instance ��չ�б�
	 */
	constinit const char* const VK_Instance::EXTENSION_GROUP[]
		{
			// ���ش��� surface ��չ
			VK_KHR_SURFACE_EXTENSION_NAME,

			// ��ƽ̨���� surface ����
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

			// ���Ա���
			#ifdef FC_DEBUG
				VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
				VK_EXT_DEBUG_REPORT_EXTENSION_NAME
			#endif
		};

	/*
	 @	���õ� API �汾

			VK_MAKE_API_VERSION( 0, 1, 1, 0 );
	 */
	constinit const uint32_t VK_Instance::API_VERSION = VK_API_VERSION_1_1;
}



namespace fce::inner::vulkan
{
	/*
	 @	����
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
	 @	����
	 */
	FC_INLINE void VK_Instance::Destroy() noexcept
		{
			// ���� debug
			#ifdef FC_DEBUG

				this->debug.Destroy();

			#endif

			// ���� instance
			this->DestroyInstance();
		}


	/*
	 @	����
	 */
	FC_INLINE VkResult VK_Instance::Create() noexcept
		{
			// ���� instance
			VkResult res
				{
					this->CreateInstance()
				};

			if( res == VK_SUCCESS )
				{
					// ���� debug
					#ifdef FC_DEBUG

						VK_ERR_RETURN( this->debug.Create() );

					#endif

					// ��ǰ�� window::Surface ֮ǰ���� �������� surface
					res = this->CreateSingleSurface();
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	���� instance
	 */
	FC_INLINE void VK_Instance::DestroyInstance() noexcept
		{
			// ���� instance
			if( this->vkInstance )
				{
					vkDestroyInstance( this->vkInstance,
									   s_VK_Allocator );
				}
		}


	/*
	 @	���� instance
	 */
	FC_INLINE VkResult VK_Instance::CreateInstance() noexcept
		{
			// ���� layer �б�
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

			// ��ȡ extension count
			constexpr uint32_t EXTENSION_COUNT
				{
					static_cast< uint32_t >
					( std::ranges::size( EXTENSION_GROUP ) )
				};

			// ���� App info
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

			// ���� Create info
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
			
			// ���� Instance
			VkResult res
				{
					vkCreateInstance( &createInfo,
									  s_VK_Allocator,
									  &this->vkInstance )
				};

			if( res == VK_SUCCESS )
				{
					// ��̬���� ʵ���� �����ӿ�
					#ifdef VK_NO_PROTOTYPES

						volkLoadInstance( this->vkInstance );

					#endif
				}

			return res;
		}


	/*
	 @	���� ƽ̨���ڵ� չʾҳ��

			��������

			��Щƽ̨�£��� VK_PhysicalDevice::MatchQueueFamily ��ȡ ���������� ��Ҫ���� surface �ͱ������𴴽�
	 */
	FC_INLINE VkResult VK_Instance::CreateSingleSurface() noexcept
		{
			// ������ VkSurfaceKHR
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

					// �ӳ��� VK_WindowSurface ���� �ര��֧��
					return VK_SUCCESS;

			#endif
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	��� ���õ� extension �б� �Ƿ�֧��
	 */
	VkBool32 VK_Instance::CheckExtensions( const char* const layerName ) noexcept
		{
			// ��ȡ extension �б�
			ExtensionList arrSupportes;

			if( GetExtensionList( arrSupportes,
								  layerName ) == VK_SUCCESS )
				{
					// ���� ��Ҫ���õ� extension �б�
					for( const char* const extension : EXTENSION_GROUP )
						{
							// ����Ƿ�֧��
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
	 @	��� ���� extension �Ƿ����
	 */
	VkBool32 VK_Instance::hasExtension( const char* const name,
										const ExtensionList& arrExtensions ) noexcept
		{
			/*
			 @	�Ƚ� extension ����
			 */
			auto f = [ name ]( const VkExtensionProperties& extProps ) NOEXCEPT
				{
					return ! strcmp( extProps.extensionName, name );
				};

			// ���� extension �Ƿ����
			return stl::find_if( arrExtensions, f )
				!= arrExtensions.end();
		}


	/*
	 @	��ȡ ʵ����չ��
	 */
	VkResult VK_Instance::GetExtensionList( ExtensionList& arrExtensions,
											const char* const layerName ) noexcept
		{
			// ��ȡ extension ����
			uint32_t count;

			VkResult res
				{
					vkEnumerateInstanceExtensionProperties( layerName, &count,
															VK_NULL_HANDLE )
				};

			// ��ȡ extension �б�
			if( res == VK_SUCCESS && count )
				{
					arrExtensions.resize( count );

					res = vkEnumerateInstanceExtensionProperties( layerName, &count,
																  arrExtensions.data() );
				}

			return res;
		}


	/*
	 @	��ȡ ������װ�汾
	 */
	//uint32_t VK_Instance::GetVersion() const
	//	{
	//		uint32_t apiVersion;

	//		vkEnumerateInstanceVersion( &apiVersion );

	//		return apiVersion;
	//	}
}