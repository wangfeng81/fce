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
	 !	VK_Device
	 */
	class FC_API VK_Device final
		{
			/*	Friends
			*/
			friend VK_Graphics;

			/*	Config
			 */
			struct
				{
					/*	VkQueues
					 */
					struct
						{
							VkQueue graph,
									present;
						}
					queues;
				}
			config;

		public:
			/*	Config
			*/
			[[nodiscard]] const auto* operator -> () const noexcept;

			/*	Get VkDevice
			*/
			[[nodiscard]] operator const VkDevice() const noexcept;

			[[nodiscard]] const VkDevice& GetHandle() const noexcept;

			/*	Wait idle
			*/
			VkResult WaitIdle() const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Device );

			~VK_Device() = default;

			VK_Device() noexcept;

			/*	Create & Destroy
			*/
			void Destroy() noexcept;

			VkResult Create() noexcept;

			VkResult Rebuild() noexcept;

			/*	Get queues
			*/
			void GetQueues() noexcept;

			/*	Set features
			*/
			void SetFeatures( VkPhysicalDeviceFeatures& ) noexcept;

			/*	On lost
			*/
			bool on_Lost() noexcept;

		private:
			/*	Logical device
			*/
			VkDevice vkDevice;

			/*	Enabled extension group
			*/
			constinit static const char* const EXTENSION_GROUP[];
		};

	/*
	 @	VK_Device singleton shortcut
	 */
	#define s_VK_Device s_Graphics.impl.device
}



/**
 !	class VK_Device
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get config
	 */
	FC_INLINE const auto* VK_Device::operator -> () const noexcept
		{
			return &this->config;
		}

	/*
	 @	Get VkDevice
	 */
	FC_INLINE VK_Device::operator const VkDevice() const noexcept
		{
			return this->vkDevice;
		}

	FC_INLINE const VkDevice& VK_Device::GetHandle() const noexcept
		{
			return this->vkDevice;
		}
}