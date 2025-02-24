/*
**
**	Graphics.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../math/Box.h"


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Graphics.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_Graphics.h"
#elif defined  FC_METAL
	#include "metal/MT_Graphics.h"
#elif defined  FC_GLES
	#include "gles/ES_Graphics.h"
#endif


namespace fce
{
	/*
	 !	Graphics
	 */
	class Graphics final
		{
			/*	Friends
			*/
			friend Engine;

			friend Window;

			friend Application;

			friend DescriptorPool;

			friend RenderPipeline;

			friend AssetManagement;

		public:
			/*	Implementation
			*/
			using Impl = inner::graphics::Impl;

			Impl impl;

		public:
			/*	Get native handle
			*/
			[[nodiscard]] const auto& GetHandle() const noexcept;

			/*	Convert RESULT
			*/
			[[nodiscard]] static RESULT Convert( const auto ) noexcept;

			/*	Convert COMPARE
			*/
			CONVERT_IMPL_ENUM_VALUE( COMPARE );

		protected:
			/*	Constructor & Destructor
			*/
			~Graphics();

			Graphics() noexcept;

			/*	Clear resources
			*/
			Void Clear() noexcept;

			/*	Create & Destroy
			*/
			Bool Create() noexcept;

			Void Destroy() noexcept;

			/*	Wait idle
			*/
			Bool WaitIdle() const noexcept;
		};


	/*
	 @	Graphics singleton shortcut
	 */
	extern FC_API Graphics& s_Graphics;
}



/**
 !	class Graphics
 */
namespace fce
{
	/*
	 @	Wait idle
	 */
	FC_INLINE Bool Graphics::WaitIdle() const noexcept
		{
			return this->impl.WaitIdle() == VK_SUCCESS;
		}

	/*
	 @	Get native handle
	 */
	FC_INLINE const auto& Graphics::GetHandle() const noexcept
		{
			return this->impl.GetHandle();
		}

	/*
	 @	Convert RESULT
	 */
	FC_INLINE RESULT Graphics::Convert( const auto res ) noexcept
		{
			return Impl::Convert( res );
		}
}