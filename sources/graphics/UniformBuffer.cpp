/*
**
**	Uniform buffer.
**
		Copyright (c) WangFeng since 2000...
*/

#include "../../include/graphics/UniformBuffer.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_UniformBuffer.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_UniformBuffer.cxx"
#elif defined  FC_METAL
	#include "metal/MT_UniformBuffer.cxx"
#elif defined  FC_GLES
	#include "gles/ES_UniformBuffer.cxx"
#endif


/**
 !	UniformBuffer
 */
namespace fce
{
	/*
	 @	析构
	 */
	UniformBuffer::~UniformBuffer() noexcept
		{
			this->Release();
		}

	/*
	 @	构造
	 */
	UniformBuffer::UniformBuffer( RESULT& res,
								  const UInt size ) noexcept
		:
			impl( size )
		{
			res = this->Initialize();
		}
}



namespace fce
{
	/*
	 @	释放 ( 原生调用 )
	 */
	FC_INLINE Void UniformBuffer::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	初始化 ( 原生调用 )
	 */
	FC_INLINE RESULT UniformBuffer::Initialize() noexcept
		{
			const auto res
				{
					this->impl.Initialize()
				};

			return Graphics::Convert( res );
		}
}