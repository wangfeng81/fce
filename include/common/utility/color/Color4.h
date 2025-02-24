/*
**
**	Color 4 Byte.
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Color4
	 
			RGBA 8888
	 */
	struct FC_API alignas( DWord ) Color4 final
		{
			/*	Model
			 */
			union
				{
					Color3 rgb;
					DWord  value;
					Color1 components[ 4 ];

					struct
						{
							Color1 r, g, b, a;
						};
				};

			static constexpr UInt8 DIMENSION = 4;

			/*	Constructor
			*/
			Color4() = default;

			constexpr Color4( const DWord value ) noexcept
				:
					value( value )
				{
				}

			constexpr Color4( const Color1 c,
							  const Color1 a = Byte_MAX ) noexcept
				:
					Color4( c, c, c, a )
				{
				}

			constexpr Color4( const Color2& rgb,
							  const Color1 a = Byte_MAX ) noexcept
				:
					Color4( Color3( rgb ), a )
				{
				}

			constexpr Color4( const Color3F& rgb,
							  const Color1 a = Byte_MAX ) noexcept
				:
					Color4( Color3( rgb ), a )
				{
				}

			constexpr Color4( const Color3& rgb,
							  const Color1 a = Byte_MAX ) noexcept
				:
					components
						{
							rgb.r, rgb.g, rgb.b, a
						}
				{
				}

			constexpr Color4( const Color1 r,
							  const Color1 g,
							  const Color1 b,
							  const Color1 a = Byte_MAX ) noexcept
				:
					components
						{
							r, g, b, a
						}
				{
				}

			/*	Conversion
			*/
			explicit constexpr operator DWord() const noexcept
				{
					return this->value;
				}

			explicit constexpr operator Color1() const noexcept
				{
					return this->Gray();
				}

			explicit constexpr operator Color2() const noexcept
				{
					return Color2( this->rgb );
				}

			explicit constexpr operator const Color3&() const noexcept
				{
					return this->rgb;
				}

			explicit constexpr operator Color3F() const noexcept
				{
					return Color3F( this->rgb );
				}

			/*	Access
			*/
			Color1& operator [] ( const UInt8 idx )
				{
					FC_ASSERT( idx < 4 );

					return this->components[ idx ];
				}

			Color1 operator [] ( const UInt8 idx ) const
				{
					FC_ASSERT( idx < 4 );

					return this->components[ idx ];
				}

			/*	Compare
			*/
			constexpr Bool operator == ( const Color4& color4 ) const noexcept
				{
					return this->value == color4.value;
				}

			constexpr Bool operator != ( const Color4& color4 ) const noexcept
				{
					return this->value != color4.value;
				}

			/*	Combinatorial =
			*/
			constexpr Color4& operator += ( const Color1 c ) noexcept
				{
					return *this = *this + c;
				}

			constexpr Color4& operator -= ( const Color1 c ) noexcept
				{
					return *this = *this - c;
				}

			constexpr Color4& operator *= ( const Color1F f ) noexcept
				{
					return *this = *this * f;
				}

			constexpr Color4& operator += ( const Color4& color4 ) noexcept
				{
					return *this = *this + color4;
				}

			constexpr Color4& operator -= ( const Color4& color4 ) noexcept
				{
					return *this = *this - color4;
				}

			constexpr Color4& operator *= ( const Color4& color4 ) noexcept
				{
					return *this = *this * color4;
				}

			/*	Blend operation
			*/
			constexpr Color1 Gray() const noexcept
				{
					return this->rgb.Gray();
				}

			constexpr Color4 operator ~ () const noexcept
				{
					return Color4( ~this->value );
				}

			constexpr Color4 operator + ( const Color1 c ) const noexcept
				{
					return Color4( std::min( this->r + c, 0xFF ),
								   std::min( this->g + c, 0xFF ),
								   std::min( this->b + c, 0xFF ),
								   std::min( this->a + c, 0xFF ) );
				}

			constexpr Color4 operator - ( const Color1 c ) const noexcept
				{
					return Color4( this->r > c ? this->r - c : 0,
								   this->g > c ? this->g - c : 0,
								   this->b > c ? this->b - c : 0,
								   this->a > c ? this->a - c : 0 );
				}

			constexpr Color4 operator * ( const Color1F f ) const noexcept
				{
					return Color4( ( Color1 )( this->r * f ),
								   ( Color1 )( this->g * f ),
								   ( Color1 )( this->b * f ),
								   ( Color1 )( this->a * f ) );
				}

			constexpr Color4 operator + ( const Color4& color4 ) const noexcept
				{
					return Color4( std::min( this->r + color4.r, 0xFF ),
								   std::min( this->g + color4.g, 0xFF ),
								   std::min( this->b + color4.b, 0xFF ),
								   std::min( this->a + color4.a, 0xFF ) );
				}

			constexpr Color4 operator - ( const Color4& color4 ) const noexcept
				{
					return Color4( this->r > color4.r ? this->r - color4.r : 0,
								   this->g > color4.g ? this->g - color4.g : 0,
								   this->b > color4.b ? this->b - color4.b : 0,
								   this->a > color4.a ? this->a - color4.a : 0 );
				}

			constexpr Color4 operator * ( const Color4& color4 ) const noexcept
				{
					return Color4( ( Color1 )( this->r * color4.r * ( 1.0f / 0xFF ) ),
								   ( Color1 )( this->g * color4.g * ( 1.0f / 0xFF ) ),
								   ( Color1 )( this->b * color4.b * ( 1.0f / 0xFF ) ),
								   ( Color1 )( this->a * color4.a * ( 1.0f / 0xFF ) ) );
				}

			constexpr Color4 Min( const Color4& color4 ) const noexcept
				{
					return Color4( std::min( this->r, color4.r ),
								   std::min( this->g, color4.g ),
								   std::min( this->b, color4.b ),
								   std::min( this->a, color4.a ) );
				}

			constexpr Color4 Max( const Color4& color4 ) const noexcept
				{
					return Color4( std::max( this->r, color4.r ),
								   std::max( this->g, color4.g ),
								   std::max( this->b, color4.b ),
								   std::max( this->a, color4.a ) );
				}
		};
}