/*
**
**	RenderPass.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/platform/Window.h"


/**
 !	Attachment reference
 */
namespace fce::inner::renderPass
{
	/*
	 @	构造
	 */
	Reference::Reference( const UInt32 index,
						  const LAYOUT layout ) noexcept
		:
			index( index ),
			layout( layout )
		{
		}
}



/**
 !	Subpass dependency
 */
namespace fce::inner::renderPass
{
	/*
	 @	构造
	 */
	Dependency::Dependency( const UInt32 srcSubpass,
							const UInt32 dstSubpass ) noexcept
		:
			srcSubpass( srcSubpass ),
			dstSubpass( dstSubpass )
		{
		}
}



/**
 !	Attachment description
 */
namespace fce::inner::renderPass
{
	/*
	 @	构造
	 */
	Attachment::Attachment( const Multisample::TYPE	sample,
							const FORMAT			format,
							const LOAD				load,
							const STORE				store,
							const LOAD				stencilLoad,
							const STORE				stencilStore,
							const LAYOUT			initialLayout,
							const LAYOUT			finalLayout ) noexcept
		:
			sample			( sample ),
			format			( format ),
			load			( load ),
			store			( store ),
			stencilLoad		( stencilLoad ),
			stencilStore	( stencilStore ),
			initialLayout	( initialLayout ),
			finalLayout		( finalLayout )
		{
		}
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_RenderPass.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_RenderPass.cxx"
#elif defined  FC_METAL
	#include "metal/MT_RenderPass.cxx"
#elif defined  FC_GLES
	#include "gles/ES_RenderPass.cxx"
#endif


namespace fce
{
	/*
	 @	析构
	 */
	RenderPass::~RenderPass() noexcept
		{
			this->Release();
		}

	/*
	 @	构造 Buffer
	 */
	RenderPass::RenderPass( RESULT& res,
							const Buffer& buf )
		:
			RenderPass( res, Info( buf ) )
		{
		}

	/*
	 @	构造 Info
	 */
	RenderPass::RenderPass( RESULT& res,
							const Info& info )
		:
			impl()
		{
			res = this->Initialize( info );
		}
}



namespace fce
{
	/*
	 @	释放 ( 原生调用 )
	 */
	FC_INLINE Void RenderPass::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	初始化 ( 原生调用 )
	 */
	FC_INLINE RESULT RenderPass::Initialize( const Info& info )
		{
			const auto res
				{
					this->impl.Initialize( info )
				};

			return Graphics::Convert( res );
		}
}