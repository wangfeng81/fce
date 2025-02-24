/*
**
**	VK_Debug.
**
		Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_Debug
	 */
	class FC_API VK_Debug final
		{
			/*	Friends
			*/
			friend VK_Instance;

		public:
			/*	Layer list
			*/
			using LayerList = stl::vector
							  < VkLayerProperties >;

			static VkResult GetLayerList( LayerList& ) noexcept;

			/*	Has layer
			*/
			static VkBool32 hasLayer( const char* const name,
									  const LayerList& ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Debug );

			~VK_Debug() = default;

			VK_Debug() noexcept;

			/*	Create & Destroy
			*/
			void Destroy() noexcept;

			VkResult Create() noexcept;

			/*	Check layers
			*/
			static VkBool32 CheckLayers() noexcept;

			/*	Vk debug report callback function
			*/
			static VKAPI_ATTR VkBool32 VKAPI_CALL ProcessReport( VkDebugReportFlagsEXT,
																 VkDebugReportObjectTypeEXT,
																 uint64_t, size_t, int32_t,
																 const char*, const char*,
																 void* ) noexcept;

		private:
			/*	Vk debug report callback
			*/
			VkDebugReportCallbackEXT vkReportCallback;

			/*	Enabled layer group
			*/
			constinit static const char* const LAYER_GROUP[];

			/*	Enabled report flags
			*/
			constinit static const VkDebugReportFlagsEXT REPORT_FLAGS;
		};


	/*
	 @	VK_Debug singleton shortcut
	 */
	#define s_VK_Debug s_Graphics.impl.instance.debug
}