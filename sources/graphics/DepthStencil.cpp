/*
**
**	Depth stencil.
**
		Copyright (c) WangFeng since 2016...
*/

#include "../../include/graphics/DepthStencil.h"


/**
 !	DepthStencil inner kit
 */
namespace fce::inner::depthStencil
{
	/*
	 @	����
	 */
	Stencil::Stencil( const STENCIL failOp,
					  const STENCIL passOp,
					  const STENCIL depthFailOp,
					  const COMPARE compareOp,
					  const UInt32 compareMask,
					  const UInt32 writeMask,
					  const UInt32 reference ) noexcept
		:
			failOp		( failOp ),
			passOp		( passOp ),
			depthFailOp	( depthFailOp ),
			compareOp	( compareOp ),
			compareMask	( compareMask ),
			writeMask	( writeMask ),
			reference	( reference )
		{
		}

	/*
	 @	����
	 */
	Info::Info( const Bool enableDepth,
				const Bool enableStencil,
				const Bool enableWrite,
				const Bool enableBounds,
				const COMPARE compare,
				const Float32 minDepth,
				const Float32 maxDepth,
				const Stencil front,
				const Stencil back ) noexcept
		:
			enableDepth		( enableDepth ),
			enableStencil	( enableStencil ),
			enableWrite		( enableWrite ),
			enableBounds	( enableBounds ),
			compare			( compare ),
			minDepth		( minDepth ),
			maxDepth		( maxDepth ),
			front			( front ),
			back			( back )
		{
		}
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_DepthStencil.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_DepthStencil.cxx"
#elif defined  FC_METAL
	#include "metal/MT_DepthStencil.cxx"
#elif defined  FC_GLES
	#include "gles/ES_DepthStencil.cxx"
#endif


namespace fce
{
	/*
	 @	����
	 */
	DepthStencil::~DepthStencil() noexcept
		{
			this->Release();
		}

	/*
	 @	���� model
	 */
	DepthStencil::DepthStencil( RESULT& res,
								const TYPE type,
								const Size& size ) noexcept
		:
			impl()
		{
			res = this->Initialize( type, size );
		}
}



namespace fce
{
	/*
	 @	�ͷ� ( ԭ������ )
	 */
	FC_INLINE Void DepthStencil::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	��ʼ�� ( ԭ������ )
	 */
	FC_INLINE RESULT DepthStencil::Initialize( const TYPE type,
											   const Size& size ) noexcept
		{
			const auto res
				{
					this->impl.Initialize( size, type )
				};

			return Graphics::Convert( res );
		}
}