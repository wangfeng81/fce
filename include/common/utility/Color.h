/*
**
**	Color
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*	COLOR
	 */
	enum struct COLOR
		:
			UInt8
		{
			_0,
			_1,
			_2,
			_3,
			_4,

			MAXIMUM,

			// Alias
			UNKNOWN	 = _0,
			MONO	 = _1,
			RGB565	 = _2,
			RGB888	 = _3,
			RGBA8888 = _4,

			GRAY	 = _1,
			ALPHA	 = _1,
			ARGB1555 = _2
		};


	/*
	 !	Color1
	 
			Gray
	 */
	using Color1  = Byte;

	using Color1F = Float32;

	/*
	 @	Convert color1
	 */
	FC_INLINE constexpr Color1F Convert( const Color1 color1 ) noexcept
		{
			return color1 * ( 1.0f / Byte_MAX );
		}

	FC_INLINE constexpr Color1 Convert( const Color1F color1F ) noexcept
		{
			return Color1( color1F * Byte_MAX );
		}
}


/**
 !	Color 2 3 4
 */
#include "color/Color2.h"

#include "color/Color3.h"
#include "color/Color3F.h"

#include "color/Color4.h"
#include "color/Color4F.h"


/**
 !	Color concept
 */
namespace fce
{
	/*	Float component
	 */
	template< typename T >

	concept Color_float = Any_of< std::remove_cvref_t< T >,
								  Color1F, Color3F, Color4F,
								  Float32 >;

	/*	Integer component
	 */
	template< typename T >

	concept Color_integer = Any_of< std::remove_cvref_t< T >,
									Color1, Color2, Color3, Color4,
									Byte,	Word,			DWord >;

	/*	Color object
	 */
	template< typename T >

	concept Color_object = Any_of< std::remove_cvref_t< T >,
								   Color2, Color3, Color4,
								   Color3F, Color4F >;
}



/**
 !	struct Color
 */
namespace fce
{
	/*
	 !	Color
	 */
	struct Color final
		{
			/*	FORMAT
			 */
			using FORMAT = COLOR;

			/*	CHANNEL
			 */
			enum struct CHANNEL
				:
					Byte
				{
					R,
					G,
					B,
					A,

					RG,
					RB,
					RA,
					GB,
					GA,
					BA,

					RGB,
					RGA,
					RBA,
					GBA,

					RGBA,

					MAXIMUM,

					RED		= R,
					GREEN	= G,
					BLUE	= B,
					ALPHA	= A,

					ALL		= RGBA
				};

			/*	Common
			 */
			static constexpr Color3	WHITE		{ 0xFF, 0xFF, 0xFF },
									SILVER		{ 0xC0, 0xC0, 0xC0 },
									GRAY		{ 0x80, 0x80, 0x80 },
									DIMGRAY		{ 0x40, 0x40, 0x40 },
									BLACK		{ 0x00, 0x00, 0x00 },
									
									RED			{ 0xFF, 0x00, 0x00 },
									GREEN		{ 0x00, 0xFF, 0x00 },
									BLUE		{ 0x00, 0x00, 0xFF },
									
									MAROON		{ 0x80, 0x00, 0x00 },
									DARKGREEN	{ 0x00, 0x80, 0x00 },
									NAVYBLUE	{ 0x00, 0x00, 0x80 },
									
									CYAN		{ 0x00, 0xFF, 0xFF },
									MAGENTA		{ 0xFF, 0x00, 0xFF },
									YELLOW		{ 0xFF, 0xFF, 0x00 },
									
									TEAL		{ 0x00, 0x80, 0x80 },
									PURPLE		{ 0x80, 0x00, 0x80 },
									OLIVE		{ 0x80, 0x80, 0x00 };

			/*
			 @	Bit depth
			 */
			static constexpr UInt8 Depth( const COLOR color ) noexcept
				{
					return UInt8( color ) << 3;
				}

			/*
			 @	Size
			 */
			static constexpr UInt8 Size( const Integer auto depth ) noexcept
				{
					return UInt8( depth ) >> 3;
				}
		};
}