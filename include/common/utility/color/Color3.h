/*
**
**	Color 3 Byte.
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Color3
	 
			RGB 888
	 */
	struct FC_API alignas( Byte ) Color3 final
		{
			/*	Model
			 */
			union
				{
					Color1 components[ 3 ];

					struct
						{
							Color1 r, g, b;
						};
				};

			static constexpr UInt8 DIMENSION = 3;

			/*	Constructor
			*/
			Color3() = default;

			constexpr Color3( const Color1 c ) noexcept
				:
					components
						{
							c, c, c
						}
				{
				}

			constexpr Color3( const Color1 r,
							  const Color1 g,
							  const Color1 b ) noexcept
				:
					components
						{
							r, g, b
						}
				{
				}

			constexpr Color3( const Color2& color2 ) noexcept
				:
					components
						{
							Color1( color2.r << 3 ),
							Color1( color2.g << 2 ),
							Color1( color2.b << 3 )
						}
				{
				}

			/*	Conversion
			*/
			explicit constexpr operator Color1() const noexcept
				{
					return this->Gray();
				}

			explicit constexpr operator Color2() const noexcept
				{
					return Color2( this->r >> 3,
								   this->g >> 2,
								   this->b >> 3 );
				}

			/*	Access
			*/
			Color1& operator [] ( const UInt8 idx )
				{
					FC_ASSERT( idx < 3 );

					return this->components[ idx ];
				}

			Color1 operator [] ( const UInt8 idx ) const
				{
					FC_ASSERT( idx < 3 );

					return this->components[ idx ];
				}

			/*	Compare
			*/
			constexpr Bool operator == ( const Color3& color3 ) const noexcept
				{
					return this->r == color3.r
						&& this->g == color3.g
						&& this->b == color3.b;
				}

			constexpr Bool operator != ( const Color3& color3 ) const noexcept
				{
					return this->r != color3.r
						|| this->g != color3.g
						|| this->b != color3.b;
				}

			/*	Combinatorial =
			*/
			constexpr Color3& operator += ( const Color1 c ) noexcept
				{
					return *this = *this + c;
				}

			constexpr Color3& operator -= ( const Color1 c ) noexcept
				{
					return *this = *this - c;
				}

			constexpr Color3& operator *= ( const Color1F f ) noexcept
				{
					return *this = *this * f;
				}

			constexpr Color3& operator += ( const Color3& color3 ) noexcept
				{
					return *this = *this + color3;
				}

			constexpr Color3& operator -= ( const Color3& color3 ) noexcept
				{
					return *this = *this - color3;
				}

			constexpr Color3& operator *= ( const Color3& color3 ) noexcept
				{
					return *this = *this * color3;
				}

			/*	Blend operation
			*/
			constexpr Color1 Gray() const noexcept
				{
					return ( this->r +
							 this->g +
							 this->b ) / 3;
				}

			constexpr Color3 operator ~ () const noexcept
				{
					return Color3( ~this->r,
								   ~this->g,
								   ~this->b );
				}

			constexpr Color3 operator + ( const Color1 c ) const noexcept
				{
					return Color3( std::min( this->r + c, 0xFF ),
								   std::min( this->g + c, 0xFF ),
								   std::min( this->b + c, 0xFF ) );
				}

			constexpr Color3 operator - ( const Color1 c ) const noexcept
				{
					return Color3( this->r > c ? this->r - c : 0,
								   this->g > c ? this->g - c : 0,
								   this->b > c ? this->b - c : 0 );
				}

			constexpr Color3 operator * ( const Color1F f ) const noexcept
				{
					return Color3( ( Color1 )( this->r * f ),
								   ( Color1 )( this->g * f ),
								   ( Color1 )( this->b * f ) );
				}

			constexpr Color3 operator + ( const Color3& color3 ) const noexcept
				{
					return Color3( std::min( this->r + color3.r, 0xFF ),
								   std::min( this->g + color3.g, 0xFF ),
								   std::min( this->b + color3.b, 0xFF ) );
				}

			constexpr Color3 operator - ( const Color3& color3 ) const noexcept
				{
					return Color3( this->r > color3.r ? this->r - color3.r : 0,
								   this->g > color3.g ? this->g - color3.g : 0,
								   this->b > color3.b ? this->b - color3.b : 0 );
				}

			constexpr Color3 operator * ( const Color3& color3 ) const noexcept
				{
					return Color3( ( Color1 )( this->r * color3.r * ( 1.0f / 0xFF ) ),
								   ( Color1 )( this->g * color3.g * ( 1.0f / 0xFF ) ),
								   ( Color1 )( this->b * color3.b * ( 1.0f / 0xFF ) ) );
				}

			constexpr Color3 Min( const Color3& color3 ) const noexcept
				{
					return Color3( std::min( this->r, color3.r ),
								   std::min( this->g, color3.g ),
								   std::min( this->b, color3.b ) );
				}

			constexpr Color3 Max( const Color3& color3 ) const noexcept
				{
					return Color3( std::max( this->r, color3.r ),
								   std::max( this->g, color3.g ),
								   std::max( this->b, color3.b ) );
				}
		};
}