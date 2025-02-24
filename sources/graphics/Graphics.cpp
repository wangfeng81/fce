/*
**
**	Graphics.
**
		Copyright (c) WangFeng since 2000...
*/

// VMA: "stb-style" single header file
#define VMA_IMPLEMENTATION

#include "../../include/platform/Application.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Graphics.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_Graphics.cxx"
#elif defined  FC_METAL
	#include "metal/MT_Graphics.cxx"
#elif defined  FC_GLES
	#include "gles/ES_Graphics.cxx"
#endif


namespace fce
{
	/*
	 @	����
	 */
	Graphics::Graphics() noexcept
		{
		}

	/*
	 @	����
	 */
	Graphics::~Graphics() noexcept
		{
		}
}



namespace fce
{
	/*
	 @	����
	 */
	Void Graphics::Destroy() noexcept
		{
			// ��� ��Դ
			this->Clear();

			// ���� impl
			this->impl.Destroy();
		}


	/*
	 @	����
	 */
	Bool Graphics::Create() noexcept
		{
			const RESULT res
				{
					Convert( this->impl.Create() )
				};

			return res == RESULT::SUCCESS;
		}

	/*
	 @	��� ��Դ
	 */
	Void Graphics::Clear() noexcept
		{
			// �ȴ� ͼ���豸����
			this->WaitIdle();

			// ���� ��Դ ��
			DescriptorPool::DestructAll();

			UniformBuffer::DestructAll();

			DepthStencil::DestructAll();

			VertexBuffer::DestructAll();

			IndexBuffer::DestructAll();

			Texture2::DestructAll();

			Sampler::DestructAll();

			// ��� impl
			this->impl.Clear();
		}
}