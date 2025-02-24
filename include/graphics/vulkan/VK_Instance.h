/*
**
**	VK_Instance.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

#ifdef FC_DEBUG

	#include "VK_Debug.h"

#endif


namespace fce::inner::vulkan
{
	/*
	 !	VK_Instance
	 */
	class VK_Instance final
		{
			/*	Friends
			*/
			friend VK_Graphics;

		public:
			/* Vk debug
			*/
			#ifdef FC_DEBUG

				VK_Debug debug;

			#endif

			/*	API version
			*/
			constinit static const uint32_t API_VERSION;

		public:
			/*	Extension list
			*/
			using ExtensionList = stl::vector
								  < VkExtensionProperties >;

			/*	Get VkInstance
			*/
			[[nodiscard]] operator const VkInstance() const noexcept;

			[[nodiscard]] const VkInstance& GetHandle() const noexcept;

			/*	Has extension
			*/
			[[nodiscard]] static VkBool32 hasExtension( const char* const name,
														const ExtensionList& ) noexcept;

			/*	Get extension list
			*/
			static VkResult GetExtensionList( ExtensionList&,
											  const char* const layerName = nullptr ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Instance );

			~VK_Instance() = default;

			VK_Instance() noexcept;

			/*	Create & Destroy
			*/
			void Destroy() noexcept;

			VkResult Create() noexcept;

			/*	Instance
			*/
			void DestroyInstance() noexcept;

			VkResult CreateInstance() noexcept;

			/*	Create single surface
			*/
			VkResult CreateSingleSurface() noexcept;

			/*	Check EXTENSION_LIST[]
			*/
			[[nodiscard]] static VkBool32 CheckExtensions( const char* const layerName = nullptr ) noexcept;

		private:
			/*	VkInstance
			*/
			VkInstance vkInstance;

			/*	Enabled extension group
			*/
			constinit static const char* const EXTENSION_GROUP[];
		};


	/*
	 @	VK_Instance singleton shortcut
	 */
	#define s_VK_Instance s_Graphics.impl.instance
}



/**
 !	class VK_Instance
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkInstance
	 */
	FC_INLINE VK_Instance::operator const VkInstance() const noexcept
		{
			return this->vkInstance;
		}

	FC_INLINE const VkInstance& VK_Instance::GetHandle() const noexcept
		{
			return this->vkInstance;
		}
}