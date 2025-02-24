/*
**
**	Sampler.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/graphics/Sampler.h"


/**
 !	Sampler inner kit
 */
namespace fce::inner::sampler
{
	/*
	 @	构造
	 */
	Info::Info( const MIPMAP mipmap,
				const FILTER filterMag,
				const FILTER filterMin,
				const ADDRESS addressU,
				const ADDRESS addressV,
				const ADDRESS addressW,
				const COMPARE compare,
				const Bool compareEnable ) noexcept
		:
			mipmap( mipmap ),
			filterMag( filterMag ),
			filterMin( filterMin ),
			addressU( addressU ),
			addressV( addressV ),
			addressW( addressW ),
			compare( compare ),
			compareEnable( compareEnable )
		{
		}


	/*
	 @	检测
	 */
	RESULT Info::Check() const noexcept
		{

			return RESULT::SUCCESS;
		}
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Sampler.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_Sampler.cxx"
#elif defined  FC_METAL
	#include "metal/MT_Sampler.cxx"
#elif defined  FC_GLES
	#include "gles/ES_Sampler.cxx"
#endif


namespace fce
{
	/*
	 @	析构
	 */
	Sampler::~Sampler() noexcept
		{
			this->Release();
		}

	/*
	 @	构造 Buffer
	 */
	Sampler::Sampler( RESULT& res,
					  const Buffer& buf )
		:
			Sampler( res, Info( buf ) )
		{
		}

	/*
	 @	构造 Info
	 */
	Sampler::Sampler( RESULT& res,
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
	 @	默认 Sampler
	 */
	Sampler* Sampler::Default() noexcept
		{
			static Sampler* sampler
				{
					Request( Info() )
				};

			return sampler;
		}
}



namespace fce
{
	/*
	 @	释放 ( 原生调用 )
	 */
	FC_INLINE Void Sampler::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	初始化 ( 原生调用 )
	 */
	FC_INLINE RESULT Sampler::Initialize( const Info& info )
		{
			const auto res
				{
					this->impl.Initialize( info )
				};

			return Graphics::Convert( res );
		}
}