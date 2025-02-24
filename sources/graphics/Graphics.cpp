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
	 @	构造
	 */
	Graphics::Graphics() noexcept
		{
		}

	/*
	 @	析构
	 */
	Graphics::~Graphics() noexcept
		{
		}
}



namespace fce
{
	/*
	 @	销毁
	 */
	Void Graphics::Destroy() noexcept
		{
			// 清空 资源
			this->Clear();

			// 销毁 impl
			this->impl.Destroy();
		}


	/*
	 @	创建
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
	 @	清空 资源
	 */
	Void Graphics::Clear() noexcept
		{
			// 等待 图形设备空闲
			this->WaitIdle();

			// 析构 资源 池
			DescriptorPool::DestructAll();

			UniformBuffer::DestructAll();

			DepthStencil::DestructAll();

			VertexBuffer::DestructAll();

			IndexBuffer::DestructAll();

			Texture2::DestructAll();

			Sampler::DestructAll();

			// 清空 impl
			this->impl.Clear();
		}
}