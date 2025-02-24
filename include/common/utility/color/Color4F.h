/*
**
**	Color 4 Float.
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Color4F

			RGBA [ 0~1 ]
	 */
	struct FC_API alignas( Float32 ) Color4F final
		{
			/*	Model
			 */
			union
				{
					Color3F rgb;
					Color1F components[ 4 ];

					struct
						{
							Color1F r, g, b, a;
						};
				};

			static constexpr UInt8 DIMENSION = 4;

			/*	Constructor
			*/
			Color4F() = default;

			constexpr Color4F( const DWord value ) noexcept
				:
					Color4F( Color4( value ) )
				{
				}

			constexpr Color4F( const Color2& rgb,
							   const Color1F a = 1.0f ) noexcept
				:
					Color4F( Color3F( rgb ), a )
				{
				}

			constexpr Color4F( const Color3& rgb,
							   const Color1F a = 1.0f ) noexcept
				:
					Color4F( Color3F( rgb ), a )
				{
				}

			constexpr Color4F( const Color3F& rgb,
							   const Color1F a = 1.0f ) noexcept
				:
					components
						{
							rgb.r, rgb.g, rgb.b, a
						}
				{
				}

			constexpr Color4F( const Color4& color4 ) noexcept
				:
					components
						{
							Convert( color4.r ),
							Convert( color4.g ),
							Convert( color4.b ),
							Convert( color4.a ) 
						}
				{
				}

			constexpr Color4F( const Color1F r,
							   const Color1F g,
							   const Color1F b,
							   const Color1F a = 1.0f ) noexcept
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
					return DWord( Color4( *this ) );
				}

			explicit constexpr operator Color2() const noexcept
				{
					return Color2( this->rgb );
				}

			explicit constexpr operator Color3() const noexcept
				{
					return Color3( this->rgb );
				}

			explicit constexpr operator const Color3F&() const noexcept
				{
					return this->rgb;
				}

			explicit constexpr operator Color4() const noexcept
				{
					return Color4( Convert( this->r ),
								   Convert( this->g ),
								   Convert( this->b ),
								   Convert( this->a ) );
				}

			/*	Access
			*/
			Color1F& operator [] ( const UInt8 idx )
				{
					FC_ASSERT( idx < 4 );

					return this->components[ idx ];
				}

			Color1F operator [] ( const UInt8 idx ) const
				{
					FC_ASSERT( idx < 4 );

					return this->components[ idx ];
				}

			/*	Compare
			*/
			Bool operator == ( const Color4F& color4F ) const noexcept
				{
					return this->rgb == color4F.rgb
						&& Equal( this->a, color4F.a );
				}

			Bool operator != ( const Color4F& color4F ) const noexcept
				{
					return this->rgb != color4F.rgb
						|| NotEqual( this->a, color4F.a );
				}

			/*	Combinatorial =
			*/
			constexpr Color4F& operator += ( const Color1F c ) noexcept
				{
					return *this = *this + c;
				}

			constexpr Color4F& operator -= ( const Color1F c ) noexcept
				{
					return *this = *this - c;
				}

			constexpr Color4F& operator *= ( const Color1F f ) noexcept
				{
					return *this = *this * f;
				}

			constexpr Color4F& operator += ( const Color4F& color4F ) noexcept
				{
					return *this = *this + color4F;
				}

			constexpr Color4F& operator -= ( const Color4F& color4F ) noexcept
				{
					return *this = *this - color4F;
				}

			constexpr Color4F& operator *= ( const Color4F& color4F ) noexcept
				{
					return *this = *this * color4F;
				}

			/*	Blend operation
			*/
			constexpr Color4F operator ~ () const noexcept
				{
					return Color4F( 1.0f - this->r,
									1.0f - this->g,
									1.0f - this->b,
									1.0f - this->a );
				}

			constexpr Color4F operator + ( const Color1F c ) const noexcept
				{
					return Color4F( std::min( this->r + c, 1.0f ),
									std::min( this->g + c, 1.0f ),
									std::min( this->b + c, 1.0f ),
									std::min( this->a + c, 1.0f ) );
				}

			constexpr Color4F operator - ( const Color1F c ) const noexcept
				{
					return Color4F( this->r > c ? this->r - c : 0.0f,
									this->g > c ? this->g - c : 0.0f,
									this->b > c ? this->b - c : 0.0f,
									this->a > c ? this->a - c : 0.0f );
				}

			constexpr Color4F operator * ( const Color1F f ) const noexcept
				{
					return Color4F( this->r * f,
									this->g * f,
									this->b * f,
									this->a * f );
				}

			constexpr Color4F operator + ( const Color4F& color4F ) const noexcept
				{
					return Color4F( std::min( this->r + color4F.r, 1.0f ),
									std::min( this->g + color4F.g, 1.0f ),
									std::min( this->b + color4F.b, 1.0f ),
									std::min( this->a + color4F.a, 1.0f ) );
				}

			constexpr Color4F operator - ( const Color4F& color4F ) const noexcept
				{
					return Color4F( this->r > color4F.r ? this->r - color4F.r : 0.0f,
									this->g > color4F.g ? this->g - color4F.g : 0.0f,
									this->b > color4F.b ? this->b - color4F.b : 0.0f,
									this->a > color4F.a ? this->a - color4F.a : 0.0f );
				}

			constexpr Color4F operator * ( const Color4F& color4F ) const noexcept
				{
					return Color4F( this->r * color4F.r,
									this->g * color4F.g,
									this->b * color4F.b,
									this->a * color4F.a );
				}

			constexpr Color4F Min( const Color4F& color4F ) const noexcept
				{
					return Color4F( std::min( this->r, color4F.r ),
									std::min( this->g, color4F.g ),
									std::min( this->b, color4F.b ),
									std::min( this->a, color4F.a ) );
				}

			constexpr Color4F Max( const Color4F& color4F ) const noexcept
				{
					return Color4F( std::max( this->r, color4F.r ),
									std::max( this->g, color4F.g ),
									std::max( this->b, color4F.b ),
									std::max( this->a, color4F.a ) );
				}
		};
}