/*
**
**	VK_Graphics
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

#include "VK_Allocator.h"
#include "VK_Instance.h"
#include "VK_PhysicalDevice.h"
#include "VK_Device.h"
#include "VK_Command.h"
#include "VK_TempBuffer.h"


namespace fce::inner::vulkan
{
	/*
	 !	VK_Graphics
	 */
	class FC_API VK_Graphics final
		{
			/*	Friends
			*/
			friend Graphics;

		public:
			/*	VK_Allocator
			*/
			VK_Allocator allocator;

			/*	VK_Instance
			*/
			VK_Instance instance;

			/*	VK_PhysicalDevice
			*/
			VK_PhysicalDevice phyDevice;

			/*	VK_Device
			*/
			VK_Device device;

			/* VK_Command
			*/
			VK_Command command;

		public:
			/*	Get handle
			*/
			[[nodiscard]] const auto& GetHandle() const noexcept;

			/*	Convert RESULT
			*/
			[[nodiscard]] static RESULT Convert( const VkResult ) noexcept;

			/*	Wait idle
			*/
			VkResult WaitIdle() const noexcept;

			/*	on error
			*/
			bool on_Error( const VkResult ) noexcept;

		protected:
			/*	Create & Destroy
			*/
			void Destroy() noexcept;

			VkResult Create() noexcept;

			/*	Clear resources
			*/
			void Clear() noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Graphics );

			~VK_Graphics() = default;

			VK_Graphics() = default;

		private:
			/*	VkCompareOp list
			*/
			static constexpr VkCompareOp COMPARE_LIST[]
				{
					VK_COMPARE_OP_NEVER,
					VK_COMPARE_OP_ALWAYS,

					VK_COMPARE_OP_LESS,
					VK_COMPARE_OP_EQUAL,
					VK_COMPARE_OP_GREATER,

					VK_COMPARE_OP_NOT_EQUAL,
					VK_COMPARE_OP_LESS_OR_EQUAL,
					VK_COMPARE_OP_GREATER_OR_EQUAL
				};
		};
}



/*
 !	Predefine
 */
namespace fce::inner::graphics
{
	/*
	 @	Graphics vulkan implementation
	 */
	using Impl = vulkan::VK_Graphics;

	/*
	 @	VK_Graphics singleton shortcut.
	 */
	#define s_VK_Graphics s_Graphics.impl
}



/**
 !	class VK_Graphics
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get handle
	 */
	FC_INLINE const auto& VK_Graphics::GetHandle() const noexcept
		{
			return this->instance.GetHandle();
		}

	/*
	 @	Convert RESULT
	 */
	FC_INLINE RESULT VK_Graphics::Convert( const VkResult res ) noexcept
		{
			return RESULT( res );
		}

	/*
	 @	Wait idle
	 */
	FC_INLINE VkResult VK_Graphics::WaitIdle() const noexcept
		{
			return this->device.WaitIdle();
		}
}