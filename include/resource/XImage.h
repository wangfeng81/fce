/*
**
**	XImage
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "Storage.h"

#include "image/Image.h"


namespace fce
{
	/*
	 !	XImage
	 */
	class XImage final
		{
			/*	Friends
			*/
			friend Engine;

		public:
			/*	Info
			 */
			using Info = Image::Info;

			/*	TYPE
			*/
			using TYPE = Image::TYPE;

			/*	Codec
			*/
			using Codec = Image::Codec;

			/*	Get codec
			*/
			[[nodiscard]] Codec* GetCodec( const TYPE ) const;

			[[nodiscard]] Codec* GetCodec( const Buffer& ) const;

			/*	Get info
			*/
			[[nodiscard]] TYPE GetType( const Buffer& ) const;

			[[nodiscard]] Bool GetInfo( const Buffer&,
										Info& ) const;

			[[nodiscard]] Bool CheckType( const Buffer&,
										  const TYPE ) const;

			/*	Decode & Encode
			*/
			Bool Decode( Image&, const Buffer& );

			Bool Encode( const Image&, Buffer&,
						 const TYPE = TYPE::PNG );

			/*	Convert type
			*/
			Bool Convert( const Buffer& src,
								Buffer& dst,
						  const TYPE = TYPE::PNG );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( XImage );

			XImage();

		private:
			/*	Codec list
			*/
			Codec* listCodecs[ UInt8( TYPE::MAXIMUM ) ];
		};


	/*
	 @	XImage singleton shortcut
	 */
	extern FC_API XImage& s_XImage;
}



/**
 !	class Image ( Syntax sugar )
 */
namespace fce
{
	/*
	 @	Encode to buffer
	 */
	FC_INLINE Bool Image::Encode( Buffer& buf,
								  const TYPE type ) const
		{
			return s_XImage.Encode( *this, buf, type );
		}

	/*
	 @	Encode directly
	 */
	FC_INLINE Buffer Image::Encode( const TYPE type ) const
		{
			Buffer buf;

			this->Encode( buf, type );

			return buf;
		}
}