/*
**
**	Color 2 Byte.
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Color2
	 
			RGB 565
	 */
	struct FC_API alignas( Word ) Color2 final
		{
			/*	Model
			 */
			union
				{
					Word value;

					struct
						{
							Word r:5, g:6, b:5;
						};
					struct
						{
							Word a:1, r5:5, g5:5, b5:5;
						};
				};

			/*	Constructor
			*/
			Color2() = default;

			constexpr Color2( const Byte r,
							  const Byte g,
							  const Byte b ) noexcept
				:
					r( r ), g( g ), b( b )
				{
				}

			constexpr Color2( const Byte r,
							  const Byte g,
							  const Byte b,
							  const Byte a ) noexcept
				:
					a( a ), r5( r ), g5( g ), b5( b )
				{
				}

			constexpr Color2( const Color1 c ) noexcept
				:
					r( c >> 3 ),
					g( c >> 2 ),
					b( c >> 3 )
				{
				}

			constexpr Color2( const Word value ) noexcept
				:
					value( value )
				{
				}

			/*	Conversion
			*/
			explicit constexpr operator Word() const noexcept
				{
					return this->value;
				}

			explicit constexpr operator Color1() const noexcept
				{
					return this->Gray() << 3;
				}

			/*	Compare
			*/
			constexpr Bool operator == ( const Color2& color2 ) const noexcept
				{
					return this->value == color2.value;
				}

			constexpr Bool operator != ( const Color2& color2 ) const noexcept
				{
					return this->value != color2.value;
				}

			/*	Combinatorial =
			*/
			constexpr Color2& operator += ( const Color1 c ) noexcept
				{
					return *this = *this + c;
				}

			constexpr Color2& operator -= ( const Color1 c ) noexcept
				{
					return *this = *this - c;
				}

			constexpr Color2& operator *= ( const Color1F f ) noexcept
				{
					return *this = *this * f;
				}

			constexpr Color2& operator += ( const Color2& color2 ) noexcept
				{
					return *this = *this + color2;
				}

			constexpr Color2& operator -= ( const Color2& color2 ) noexcept
				{
					return *this = *this - color2;
				}

			constexpr Color2& operator *= ( const Color2& color2 ) noexcept
				{
					return *this = *this * color2;
				}

			/*	Blend operation
			*/
			constexpr Color1 Gray() const noexcept
				{
					return ( this->r +
							 this->g +
							 this->b ) / 3;
				}

			constexpr Color2 operator ~ () const noexcept
				{
					return Color2( Word( ~this->value ) );
				}

			constexpr Color2 operator + ( const Color1 c ) const noexcept
				{
					return Color2( std::min( this->r + c, 0x1F ),
								   std::min( this->g + c, 0x3F ),
								   std::min( this->b + c, 0x1F ) );
				}

			constexpr Color2 operator - ( const Color1 c ) const noexcept
				{
					return Color2( this->r > c ? this->r - c : 0,
								   this->g > c ? this->g - c : 0,
								   this->b > c ? this->b - c : 0 );
				}

			constexpr Color2 operator * ( const Color1F f ) const noexcept
				{
					return Color2( ( Color1 )( this->r * f ),
								   ( Color1 )( this->g * f ),
								   ( Color1 )( this->b * f ) );
				}

			constexpr Color2 operator + ( const Color2& color2 ) const noexcept
				{
					return Color2( std::min( this->r + color2.r, 0x1F ),
								   std::min( this->g + color2.g, 0x3F ),
								   std::min( this->b + color2.b, 0x1F ) );
				}

			constexpr Color2 operator - ( const Color2& color2 ) const noexcept
				{
					return Color2( this->r > color2.r ? this->r - color2.r : 0,
								   this->g > color2.g ? this->g - color2.g : 0,
								   this->b > color2.b ? this->b - color2.b : 0 );
				}

			constexpr Color2 operator * ( const Color2& color2 ) const noexcept
				{
					return Color2( ( Color1 )( this->r * color2.r * ( 1.0f / 0x1F ) ),
								   ( Color1 )( this->g * color2.g * ( 1.0f / 0x3F ) ),
								   ( Color1 )( this->b * color2.b * ( 1.0f / 0x1F ) ) );
				}

			constexpr Color2 Min( const Color2& color2 ) const noexcept
				{
					return Color2( ( Color1 )std::min( this->r, color2.r ),
								   ( Color1 )std::min( this->g, color2.g ),
								   ( Color1 )std::min( this->b, color2.b ) );
				}

			constexpr Color2 Max( const Color2& color2 ) const noexcept
				{
					return Color2( ( Color1 )std::max( this->r, color2.r ),
								   ( Color1 )std::max( this->g, color2.g ),
								   ( Color1 )std::max( this->b, color2.b ) );
				}
		};
}