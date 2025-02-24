/*
**
**	Texture.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/graphics/Texture.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Texture.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_Texture.cxx"
#elif defined  FC_METAL
	#include "metal/MT_Texture.cxx"
#elif defined  FC_GLES
	#include "gles/ES_Texture.cxx"
#endif


namespace fce
{
	/*
	 @	析构
	 */
	Texture2::~Texture2() noexcept
		{
			this->Release();
		}

	/*
	 @	构造 Buffer
	 */
	Texture2::Texture2( RESULT& res,
						const Buffer& buf )
		:
			Texture2( res, Image( buf ) )
		{
		}

	/*
	 @	构造 Info ( 分配空间 )
	 */
	Texture2::Texture2( RESULT& res,
						const Info& info )
		:
			impl(),
			info( info )
		{
			// 临时转换： 一般不支持24位，后期再完善
			if( info.color == COLOR::_3 )
				{
					this->info.color = COLOR::_4;
				}

			// 初始化
			res = this->Initialize( this->info );
		}

	Texture2::Texture2( RESULT& res,
						const fce::Size& size,
						const COLOR color )
		:
			Texture2( res, Info( size, color ) )
		{
		}

	Texture2::Texture2( RESULT& res,
						const UInt16 width,
						const UInt16 height,
						const COLOR color )
		:
			Texture2( res, Info( width, height, color ) )
		{
		}

	/*
	 @	构造 Image
	 */
	Texture2::Texture2( RESULT& res,
						const Image& img )
		:
			Texture2( res, img.info )
		{
			if( res == RESULT::SUCCESS )
				{
					// 临时转换： 一般不支持24位，后期再完善
					if( img.Color() == COLOR::_3 )
						{
							res = this->Update
									(
										img.Convert( COLOR::_4 )
									);
						}
					else
						{
							res = this->Update( img );
						}
				}
		}
}



namespace fce
{
	/*
	 @	更新
	 */
	RESULT Texture2::Update( const Image& img,
							 const Point& dstOffset )
		{
			return this->Update( img,
								 img.Size(),
								 0, dstOffset );
		}

	/*
	 @	更新 ( 原生调用 )
	 */
	RESULT Texture2::Update( const Image& img,
							 const fce::Size& size,
							 const Point& srcOffset,
							 const Point& dstOffset )
		{
			FC_ASSERT( img );

			FC_ASSERT( srcOffset + size <= img.Size()
					&& dstOffset + size <= this->Size() );

			const auto res
				{
					this->impl.Update( img, size,
											srcOffset,
											dstOffset )
				};

			return Graphics::Convert( res );
		}
}



namespace fce
{
	/*
	 @	释放 ( 原生调用 )
	 */
	FC_INLINE Void Texture2::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	初始化 Image ( 原生调用 )
	 */
	FC_INLINE RESULT Texture2::Initialize( const Info& info )
		{
			// 检测 info 有效性
			FC_ASSERT( info );

			// 初始化 impl
			const auto res
				{
					this->impl.Initialize( info )
				};

			return Graphics::Convert( res );
		}
}