/*
**
**	Uniform buffer.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "Graphics.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_UniformBuffer.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_UniformBuffer.h"
#elif defined  FC_METAL
	#include "metal/MT_UniformBuffer.h"
#elif defined  FC_GLES
	#include "gles/ES_UniformBuffer.h"
#endif


namespace fce
{
	/*
	 !	Uniform buffer
	 */
	class FC_API UniformBuffer final
		:
		protected ObjectContainerSugar
				  < UniformBuffer >
		{
			/*	Friends
			*/
			friend Graphics;

			/*	Allocated by object container
			*/
			OBJECT_CONTAINER_SUGAR( UniformBuffer );

		public:
			/*	Implementation
			*/
			using Impl = inner::uniformBuffer::Impl;

			friend Impl;

			Impl impl;

		public:
			/*	Update
			*/
			Void Update( const Void* ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			~UniformBuffer() noexcept;

			UniformBuffer( RESULT&,
						   const UInt size ) noexcept;

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize() noexcept;
		};
}



/**
 !	class UniformBuffer
 */
namespace fce
{
	/*
	 @	Update
	 */
	FC_INLINE Void UniformBuffer::Update( const Void* p ) NOEXCEPT
		{
			FC_ASSERT( p );

			this->impl.Update( p );
		}
}