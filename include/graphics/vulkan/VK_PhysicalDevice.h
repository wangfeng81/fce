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
	 !	VK_PhysicalDevice
	 */
	class FC_API VK_PhysicalDevice final
		{
			/*	Friends
			*/
			friend VK_Graphics;

			/*	Config
			 */
			struct
				{
					VkSharingMode sharingMode;

					/*	Queue families
					 */
					struct
						{
							uint32_t count;

							/*	Indices
							 */
							union
								{
									uint32_t data[ 2 ];

									struct
										{
											uint32_t graph,
													 present;
										};
								};
						}
					queueFamilies;
				}
			config;

		public:
			/*	list
			*/
			using PhyDeviceList = stl::vector
								  < VkPhysicalDevice >;

			using ExtensionList = stl::vector
								  < VkExtensionProperties >;

			using QueueFamilyList = stl::vector
									< VkQueueFamilyProperties >;

		public:
			/*	Visit config
			*/
			[[nodiscard]] const auto* operator -> () const noexcept;

			/*	Get VkPhysicalDevice
			*/
			[[nodiscard]] operator const VkPhysicalDevice() const noexcept;

			[[nodiscard]] const VkPhysicalDevice& GetHandle() const noexcept;

			/*	Checks
			*/
			[[nodiscard]] VkBool32 CheckPresentation( const uint32_t ) const noexcept;

			[[nodiscard]] VkBool32 CheckPhyDevice( const VkPhysicalDevice ) const noexcept;

			/*	Pick physical device
			*/
			[[nodiscard]] VkPhysicalDevice PickPhyDevice( const PhyDeviceList& ) const noexcept;

			/*	Get list data
			*/
			static VkResult GetPhyDeviceList( PhyDeviceList& ) noexcept;

			static VkResult GetQueueFamilyList( QueueFamilyList&,
												const VkPhysicalDevice ) noexcept;

			static VkResult GetExtensionList( ExtensionList&,
											  const VkPhysicalDevice,
											  const char* const layerName = nullptr ) noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_PhysicalDevice );

			~VK_PhysicalDevice() = default;

			VK_PhysicalDevice() noexcept;

			/*	Initialize
			*/
			VkResult Initialize() noexcept;

			/*	Matchs
			*/
			void MatchSharingMode() noexcept;

			VkResult MatchQueueFamily() noexcept;

		private:
			/*	Physical device
			*/
			VkPhysicalDevice vkPhysicalDevice;
		};


	/*
	 @	VK_PhysicalDevice singleton shortcut
	 */
	#define s_VK_PhyDevice s_Graphics.impl.phyDevice
}



/**
 !	class VK_PhysicalDevice
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get config
	 */
	FC_INLINE const auto* VK_PhysicalDevice::operator -> () const noexcept
		{
			return &this->config;
		}

	/*
	 @	Get VkPhysicalDevice
	 */
	FC_INLINE VK_PhysicalDevice::operator const VkPhysicalDevice() const noexcept
		{
			return this->vkPhysicalDevice;
		}

	FC_INLINE const VkPhysicalDevice& VK_PhysicalDevice::GetHandle() const noexcept
		{
			return this->vkPhysicalDevice;
		}
}