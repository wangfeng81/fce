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
	 @	����
	 */
	UniformBuffer::~UniformBuffer() noexcept
		{
			this->Release();
		}

	/*
	 @	����
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
	 @	�ͷ� ( ԭ������ )
	 */
	FC_INLINE Void UniformBuffer::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	��ʼ�� ( ԭ������ )
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