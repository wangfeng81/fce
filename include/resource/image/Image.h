/*
**
**	Image
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../../graphics/Graphics.h"

#include "Codec.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS

	#include "Bitmap.h"

#endif


namespace fce
{
	/*
	 !	Image
	 */
	class Image final
		{
			/*	Friends
			*/
			friend Texture2;

			/*	Cross platform Buffer
			*/
			#ifdef FC_VULKAN
				inner::vulkan::VK_TempBuffer buffer;
			#else
				Buffer buffer;
			#endif

		public:
			/*	TYPE
			*/
			using TYPE = inner::image::TYPE;

			/*	Info
			*/
			using Info = inner::image::Info;

			/*	Codec
			*/
			using Codec = inner::image::Codec;

			/*	Constructor
			*/
			Image() = default;

			Image( Image&& ) noexcept;

			Image( const Image& ) noexcept;

			explicit Image( const Info& ) noexcept;

			explicit Image( const Size size,
							const COLOR ) noexcept;

			explicit Image( const Int16 width,
							const Int16 height,
							const COLOR ) noexcept;

			explicit Image( const Path& );

			explicit Image( const Buffer& );

			/*	Clear
			*/
			Void Clear() noexcept;

			/*	Set Image
			*/
			Void Set( const Info& ) noexcept;

			Void Set( const Int16 width,
					  const Int16 height,
					  const COLOR color ) noexcept;

			/*	Assignment
			*/
			Image& operator = ( const Image& ) noexcept;

			Image& operator = ( Image&& ) noexcept;

			/*	Serialize
			*/
			Buffer& operator >> ( Buffer& ) const noexcept;

			/*	Save ( Syntax sugar )
			*/
			RESULT Save( const Path&,
						 const TYPE = TYPE::PNG ) const noexcept;

			/*	Convert color format
			*/
			[[nodiscard]] Image Convert( const COLOR ) const noexcept;

			/*	Encode ( Syntax sugar )
			*/
			Bool Encode( Buffer&,
						 const TYPE = TYPE::PNG ) const;

			[[nodiscard]] Buffer Encode( const TYPE = TYPE::PNG ) const;

			/*	Attributes
			*/
			[[nodiscard]] const Size& Size() const noexcept;

			[[nodiscard]] Int16 Height() const noexcept;

			[[nodiscard]] Int16 Width() const noexcept;

			[[nodiscard]] COLOR Color() const noexcept;

			[[nodiscard]] UInt8 Depth() const noexcept;

			[[nodiscard]] Int32 Pitch() const noexcept;

			[[nodiscard]] Int32 Area() const noexcept;

			[[nodiscard]] UInt Capacity() const noexcept;

			[[nodiscard]] Int32 Pitch( const UInt8 ) const noexcept;

			[[nodiscard]] UInt Capacity( const UInt8 ) const noexcept;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator Bool() const noexcept;

			/*	Direct access info
			*/
			[[nodiscard]] operator const Info&() const noexcept;

			[[nodiscard]] const Info* operator -> () const noexcept;

			/*	Get buffer
			*/
			[[nodiscard]] const auto& operator * () const noexcept;

			/*	Get pixel pointer
			*/
			template< Color_integer T = Byte >

			[[nodiscard]] auto* Pixels( this auto& );

			/*	Get row pointer
			*/
			template< Color_integer T = Color1 >

			[[nodiscard]] auto* Row( this auto&,
									 const Int16 row );

			/*	Get pixel pointer
			*/
			template< Color_integer T = Color1 >

			[[nodiscard]] auto* Pixel( this auto&,
									   const Int16 x,
									   const Int16 y = 0 );

			/*	Buffer offset
			*/
			[[nodiscard]] Int32 Offset( const Int16 x,
										const Int16 y ) const noexcept;

			[[nodiscard]] Int32 Offset( const Point& pt ) const noexcept;

		private:
			/*	Convert color format
			*/
			template< Color_integer SrcColor,
					  Color_integer DstColor >

			static Void Convert( const Void*,
									   Void*, Int32 );

			/*	Get converter
			*/
			using Converter = Void( * )( const Void*,
											   Void*, Int32 );

			static Converter GetConverter( const COLOR,
										   const COLOR ) noexcept;

		private:
			/*	Info
			*/
			Info info;

			/*	Converter list
			*/
			constinit static const Converter CONVERTER_LIST[ 4 ][ 4 ];
		};
}


/**
 !	Inlines
 */
#include "Image.inl"