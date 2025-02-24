/*
**
**	Color 3 Float.
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Color3F
	 
			RGB [ 0~1 ]
	 */
	struct FC_API alignas( Float32 ) Color3F final
		{
			/*	Model
			 */
			union
				{
					Color1F components[ 3 ];

					struct
						{
							Color1F r, g, b;
						};
				};

			static constexpr UInt8 DIMENSION = 3;

			/*	Constructor
			*/
			Color3F() = default;

			constexpr Color3F( const Color1F c ) noexcept
				:
					components
						{
							c, c, c
						}
				{
				}

			constexpr Color3F( const Color1F r,
							   const Color1F g,
							   const Color1F b ) noexcept
				:
					components
						{
							r, g, b
						}
				{
				}

			constexpr Color3F( const Color2& color2 ) noexcept
				:
					components
						{
							color2.r * ( 1.0f / 0x1F ),
							color2.g * ( 1.0f / 0x3F ),
							color2.b * ( 1.0f / 0x1F )
						}
				{
				}

			constexpr Color3F( const Color3& color3 ) noexcept
				:
					components
						{
							Convert( color3.r ),
							Convert( color3.g ),
							Convert( color3.b )
						}
				{
				}

			/*	Conversion
			*/
			explicit constexpr operator Color2() const noexcept
				{
					return Color2( Color1( this->r * 0x1F ),
								   Color1( this->g * 0x3F ),
								   Color1( this->b * 0x1F ) );
				}

			explicit constexpr operator Color3() const noexcept
				{
					return Color3( Convert( this->r ),
								   Convert( this->g ),
								   Convert( this->b ) );
				}

			/*	Access
			*/
			Color1F& operator [] ( const UInt8 idx )
				{
					FC_ASSERT( idx < 3 );

					return this->components[ idx ];
				}

			Color1F operator [] ( const UInt8 idx ) const
				{
					FC_ASSERT( idx < 3 );

					return this->components[ idx ];
				}

			/*	Compare
			*/
			Bool operator == ( const Color3F& color3F ) const noexcept
				{
					return Equal( this->r, color3F.r )
						&& Equal( this->g, color3F.g )
						&& Equal( this->b, color3F.b );
				}

			Bool operator != ( const Color3F& color3F ) const noexcept
				{
					return NotEqual( this->r, color3F.r )
						|| NotEqual( this->g, color3F.g )
						|| NotEqual( this->b, color3F.b );
				}

			/*	Combinatorial =
			*/
			constexpr Color3F& operator += ( const Color1F c ) noexcept
				{
					return *this = *this + c;
				}

			constexpr Color3F& operator -= ( const Color1F c ) noexcept
				{
					return *this = *this - c;
				}

			constexpr Color3F& operator *= ( const Color1F f ) noexcept
				{
					return *this = *this * f;
				}

			constexpr Color3F& operator += ( const Color3F& color3F ) noexcept
				{
					return *this = *this + color3F;
				}

			constexpr Color3F& operator -= ( const Color3F& color3F ) noexcept
				{
					return *this = *this - color3F;
				}

			constexpr Color3F& operator *= ( const Color3F& color3F ) noexcept
				{
					return *this = *this * color3F;
				}

			/*	Blend operation
			*/
			constexpr Color1F Gray() const noexcept
				{
					return ( this->r +
							 this->g +
							 this->b ) / 3;
				}

			constexpr Color3F operator ~ () const noexcept
				{
					return Color3F( 1.0f - this->r,
									1.0f - this->g,
									1.0f - this->b );
				}

			constexpr Color3F operator + ( const Color1F c ) const noexcept
				{
					return Color3F( std::min( this->r + c, 1.0f ),
									std::min( this->g + c, 1.0f ),
									std::min( this->b + c, 1.0f ) );
				}

			constexpr Color3F operator - ( const Color1F c ) const noexcept
				{
					return Color3F( this->r > c ? this->r - c : 0.0f,
									this->g > c ? this->g - c : 0.0f,
									this->b > c ? this->b - c : 0.0f );
				}

			constexpr Color3F operator * ( const Color1F f ) const noexcept
				{
					return Color3F( this->r * f,
									this->g * f,
									this->b * f );
				}

			constexpr Color3F operator + ( const Color3F& color3F ) const noexcept
				{
					return Color3F( std::min( this->r + color3F.r, 1.0f ),
									std::min( this->g + color3F.g, 1.0f ),
									std::min( this->b + color3F.b, 1.0f ) );
				}

			constexpr Color3F operator - ( const Color3F& color3F ) const noexcept
				{
					return Color3F( this->r > color3F.r ? this->r - color3F.r : 0.0f,
									this->g > color3F.g ? this->g - color3F.g : 0.0f,
									this->b > color3F.b ? this->b - color3F.b : 0.0f );
				}

			constexpr Color3F operator * ( const Color3F& color3F ) const noexcept
				{
					return Color3F( this->r * color3F.r,
									this->g * color3F.g,
									this->b * color3F.b );
				}

			constexpr Color3F Min( const Color3F& color3F ) const noexcept
				{
					return Color3F( std::min( this->r, color3F.r ),
									std::min( this->g, color3F.g ),
									std::min( this->b, color3F.b ) );
				}

			constexpr Color3F Max( const Color3F& color3F ) const noexcept
				{
					return Color3F( std::max( this->r, color3F.r ),
									std::max( this->g, color3F.g ),
									std::max( this->b, color3F.b ) );
				}
		};
}