/*
**
**	Render pipeline.
**
		Copyright (c) WangFeng since 2016...
*/

#include "../../include/resource/Asset.h"
#include "../../include/platform/Window.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_RenderPipeline.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_RenderPipeline.cxx"
#elif defined  FC_METAL
	#include "metal/MT_RenderPipeline.cxx"
#elif defined  FC_GLES
	#include "gles/ES_RenderPipeline.cxx"
#endif


/**
 !	renderPipeline::Info
 */
namespace fce::inner::renderPipeline
{
	/*
	 @	����
	 */
	Info& Info::Update() noexcept
		{
			// ��ȡ ������ size
			const Size size
				{
					s_Window.surface.GetSize()
				};

			// ����Ĭ�� Viewport �� Scissor
			if( arrViewports.empty() )
				{
					this->Add< Viewport >( 0, size,
										   Size{ 0, 1 } );
				}

			if( arrScissors.empty() )
				{
					this->Add< Scissor >( 0, size );
				}

			return *this;
		}


	/*
	 @	���
	 */
	RESULT Info::Check() const noexcept
		{

			return RESULT::SUCCESS;
		}
}



/**
 !	RenderPipeline
 */
namespace fce
{
	/*
	 @	����
	 */
	RenderPipeline::~RenderPipeline() noexcept
		{
			this->Release();
		}

	/*
	 @	���� Buffer
	 */
	RenderPipeline::RenderPipeline( RESULT& res,
									const Buffer& buf )
		:
			RenderPipeline( res, Info( buf ).Update() )
		{
		}

	/*
	 @	���� Info
	 */
	RenderPipeline::RenderPipeline( RESULT& res,
									const Info& info )
		:
			impl(),
			vertexSize( info.vertexDescribe.UnitSize() ),
			renderPass( RenderPass::LoadAsset( info.renderPass ) ),
			descriptorPool( DescriptorPool::Request( info.arrDescriptors ) )
		{
			res = this->Initialize( info );
		}
}



namespace fce
{
	/*
	 @	�ͷ� ( ԭ������ )
	 */
	FC_INLINE Void RenderPipeline::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	��ʼ�� Info ( ԭ������ )
	 */
	FC_INLINE RESULT RenderPipeline::Initialize( const Info& info )
		{
			FC_ASSERT( this->renderPass
					&& this->descriptorPool );

			// ��ʼ�� impl
			const auto res
				{
					this->impl.Initialize( info, *this )
				};

			return Graphics::Convert( res );
		}
}