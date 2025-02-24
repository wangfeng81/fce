/*
**
**	XImage
**
		采用 build 模式

	Copyright (c) WangFeng since 2000...
*/

#include "../../include/resource/XImage.h"

#include "image/image.cxx"


namespace fce
{
	/*
	 @	构造
	 */
	XImage::XImage()
		{
			// 创建 解码器
			this->listCodecs[ UInt8( TYPE::PNG ) ] = &inner::image::Png::instance;
			this->listCodecs[ UInt8( TYPE::BMP ) ] = &inner::image::Png::instance;
			this->listCodecs[ UInt8( TYPE::TGA ) ] = &inner::image::Png::instance;
			this->listCodecs[ UInt8( TYPE::JPG ) ] = &inner::image::Png::instance;
			this->listCodecs[ UInt8( TYPE::RAW ) ] = &inner::image::Png::instance;
			this->listCodecs[ UInt8( TYPE::FCI ) ] = &inner::image::Png::instance;
		}


	/*
	 @	GetCodec
	 */
	XImage::Codec* XImage::GetCodec( const TYPE type ) const
		{
			FC_ASSERT( type < TYPE::MAXIMUM );

			return this->listCodecs[ UInt8( type ) ];
		}


	/*
	 @	GetCodec
	 */
	XImage::Codec* XImage::GetCodec( const Buffer& buf ) const
		{
			// 获取 type
			const TYPE type{ this->GetType( buf ) };

			if( type < TYPE::MAXIMUM )
				{
					return this->GetCodec( type );
				}

			return nullptr;
		}


	/*
	 @	GetType
	 */
	XImage::TYPE XImage::GetType( const Buffer& buf ) const
		{
			if( *buf )
				{
					// 遍历所有 解码器 查找类型
					for( UInt8 i{ 0 }; i < UInt8( TYPE::MAXIMUM ); ++i )
						{
							if( this->listCodecs[ i ]->CheckType( buf ) )
								{
									return TYPE( i );
								}
						}
				}

			return TYPE::MAXIMUM;
		}


	/*
	 @	CheckType
	 */
	Bool XImage::CheckType( const Buffer& buf,
							const TYPE type ) const
		{
			return this->GetCodec( type )->CheckType( buf );
		}


	/*
	 @	GetInfo
	 */
	Bool XImage::GetInfo( const Buffer& buf,
								Info& info ) const
		{
			if( Codec* codec{ this->GetCodec( buf ) } )
				{
					return codec->GetInfo( buf, info );
				}

			return false;
		}


	/*
	 @	Decode from mem
	 */
	Bool XImage::Decode( Image& img, const Buffer& buf )
		{
			if( Codec* codec{ this->GetCodec( buf ) } )
				{
					return codec->Decode( buf, img );
				}

			return false;
		}


	/*
	 @	Encode to mem
	 */
	Bool XImage::Encode( const Image& img, Buffer& buf,
										   const TYPE type )
		{
			return this->GetCodec( type )->Encode( buf, img );
		}


	/*
	 @	Convert from mem
	 */
	Bool XImage::Convert( const Buffer& src, Buffer& dst,
											 const TYPE type )
		{
			Image img;

			if( this->Decode( img, src ) )
				{
					return this->Encode( img, dst, type );
				}

			return false;
		}
}