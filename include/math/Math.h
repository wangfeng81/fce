/*
**
**	Math.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "../common/Archive.h"


namespace fce::math
{
	/*	Constant
	*/
	static constexpr auto E								= ( Float )std::numbers::e;
	static constexpr auto LOG2E							= ( Float )std::numbers::log2e;
	static constexpr auto LOG10E						= ( Float )std::numbers::log10e;
	static constexpr auto PI							= ( Float )std::numbers::pi;
	static constexpr auto LN2							= ( Float )std::numbers::ln2;
	static constexpr auto LN10							= ( Float )std::numbers::ln10;
	static constexpr auto SQRT2							= ( Float )std::numbers::sqrt2;
	static constexpr auto PHI							= ( Float )std::numbers::phi;

	/*	Radian & Degree
	*/
	FC_INLINE constexpr auto Radian( Real_number auto degree )	{ return Float( degree * ( PI / 180.0 ) );	}
	FC_INLINE constexpr auto Degree( Real_number auto radian )	{ return Float( radian * ( 180.0 / PI ) );	}

	/*	Min & Max & Mid
	*/
	template< typename T >
	FC_INLINE constexpr T Min( T a, T b )				{ return a < b ? a : b;						}
	template< typename T >
	FC_INLINE constexpr T Max( T a, T b )				{ return a > b ? a : b;						}
	template< typename T >
	FC_INLINE constexpr T Mid( T a, T b )				{ return ( a + b ) / 2;						}

	/*	Trigonometry
	*/
	FC_INLINE auto Sin( Real_number auto x )			{ return std::sin( x );						}
	FC_INLINE auto Cos( Real_number auto x )			{ return std::cos( x );						}
	FC_INLINE auto Tan( Real_number auto x )			{ return std::tan( x );						}
	FC_INLINE auto Csc( Real_number auto x )			{ return Inverse( Sin( x ) );				}
	FC_INLINE auto Sec( Real_number auto x )			{ return Inverse( Cos( x ) );				}
	FC_INLINE auto Cot( Real_number auto x )			{ return Inverse( Tan( x ) );				}

	FC_INLINE auto Sinh( Real_number auto x )			{ return std::sinh( x );					}
	FC_INLINE auto Cosh( Real_number auto x )			{ return std::cosh( x );					}
	FC_INLINE auto Tanh( Real_number auto x )			{ return std::tanh( x );					}
	FC_INLINE auto Csch( Real_number auto x )			{ return Inverse( Sinh( x ) );				}
	FC_INLINE auto Sech( Real_number auto x )			{ return Inverse( Cosh( x ) );				}
	FC_INLINE auto Coth( Real_number auto x )			{ return Inverse( Tanh( x ) );				}

	FC_INLINE auto ASin( Real_number auto x )			{ return std::asin( x );					}
	FC_INLINE auto ACos( Real_number auto x )			{ return std::acos( x );					}
	FC_INLINE auto ATan( Real_number auto x )			{ return std::atan( x );					}
	FC_INLINE auto ACsc( Real_number auto x )			{ return std::asin( Inverse( x ) );			}
	FC_INLINE auto ASec( Real_number auto x )			{ return std::acos( Inverse( x ) );			}
	FC_INLINE auto ACot( Real_number auto x )			{ return std::atan( Inverse( x ) );			}
	FC_INLINE auto ATan( Real_number auto y,
						 Real_number auto x )			{ return std::atan2( y, x );				}

	/*	Common
	*/
	FC_INLINE constexpr Bool Odd( Integer auto x )		{ return x & 1;								}

	FC_INLINE auto Abs( Real_number auto x )			{ return std::abs( x );						}
	FC_INLINE auto Inverse( Real_number auto x )		{ return Float_ONE / x;						}

	FC_INLINE auto Ceil( Floating_point auto x )		{ return std::ceil( x );					}
	FC_INLINE auto Floor( Floating_point auto x )		{ return std::floor( x );					}
	FC_INLINE auto Mod( Floating_point auto x,
						Floating_point auto y )			{ return std::fmod( x, y );					}
	FC_INLINE auto Modf( Floating_point auto x,
						Floating_point auto& intPart )	{ return std::modf( x, &intPart );			}

	template< typename T >
	FC_INLINE T Exp( T x )								{ return std::exp( x );						}
	template< typename T >
	FC_INLINE T Frexp( T x, Int32& exp )				{ return std::frexp( x, &exp );				}
 	template< typename T >
	FC_INLINE T Ldexp( T x, Int32 exp )					{ return std::ldexp( x, exp );				}

 	template< typename T >
	FC_INLINE T Log( T x )								{ return std::log( x );						}
 	template< typename T >
	FC_INLINE T Log10( T x )							{ return std::log10( x );					}
	template< typename T >
	FC_INLINE T Sqrt( T x )								{ return std::sqrt( x );					}
	template< typename T >
	FC_INLINE T Pow( T x, T exp )						{ return std::pow( x, exp );				}

	template< typename T >
	FC_INLINE constexpr T Square( T x )					{ return x * x;								}
	template< typename T >
	FC_INLINE constexpr T Double( T x )					{ return x + x;								}
	template< typename T >
	FC_INLINE constexpr T Half( T x )					{ return x * 0.5f;							}

	template< typename T >
	FC_INLINE constexpr T Lerp( T s, T d, Float t )		{ return s + ( s - d ) * t;					}
}



namespace fce
{
	/*
	 !	Interpolator
	 */
	template< typename T,
			  typename LERP = Float >

	class Interpolator
		{
			T src, dst, dif;

			Interpolator()
				{
				}

			Interpolator( const T src, const T dst )
				:
					src( src ),
					dst( dst ),
					dif( dst - src )
				{
				}

			T Lerp( LERP s )
				{
					return this->src
						 + this->dif * s;
				}
		};
}



namespace fce
{
	/*
	 !	Range2
	 */
	struct FC_API Range2 final
		{
			union
				{
					Float components[ 2 ];

					struct{ Float min, max; };
				};

			/*	Constructor
			*/
			Range2() = default;

			constexpr Range2( Float min,
							 Float max )
				: components
					{
						min, max
					}
				{
				}

			/*	Common
			*/
			Void Size( Float size )
				{
					this->max = this->min + size;
				}

			Float Size() const
				{
					return this->max - this->min;
				}

			Float HalfSize() const
				{
					return math::Half( this->Size() );
				}

			explicit operator Bool() const
				{
					return this->min != this->max;
				}

			Void Empty()
				{
					this->min = this->max = 0;
				}

			Float Middle() const
				{
					return math::Half( this->min
									 + this->max );
				}

			/*	Operation
			*/
			Range2 operator + ( Float a ) const
				{
					return Range2( this->min + a,
								  this->max + a );
				}

			Range2 operator - ( Float a ) const
				{
					return Range2( this->min - a,
								  this->max - a );
				}

			Range2 operator * ( Float a ) const
				{
					return Range2( this->min * a,
								  this->max * a );
				}

			Range2 operator / ( Float a ) const
				{
					return Range2( this->min / a,
								  this->max / a );
				}

			Range2& operator += ( Float a )
				{
					return *this = *this + a;
				}

			Range2& operator -= ( Float a )
				{
					return *this = *this - a;
				}

			Range2& operator *= ( Float a )
				{
					return *this = *this * a;
				}

			Range2& operator /= ( Float a )
				{
					return *this = *this / a;
				}

			/*	Contains & Intersects
			*/
			Bool operator == ( const Range2& range ) const
				{
					return Equal( this->min, range.min )
						&& Equal( this->max, range.max );
				}

			Bool operator != ( const Range2& range ) const
				{
					return NotEqual( this->min, range.min )
						|| NotEqual( this->max, range.max );
				}

			Bool operator && ( Float a ) const
				{
					return this->Contains( a );
				}

			Bool operator && ( const Range2& range ) const
				{
					return this->intersects( range );
				}

			Bool Contains( Float a ) const
				{
					return a >= this->min
						&& a <= this->max;
				}

			Bool Contains( const Range2& range ) const
				{
					return range.min >= this->min
						&& range.max <= this->max;
				}

			Bool intersects( const Range2& range ) const
				{
					return ! ( this->min > range.max
							|| this->max < range.min );
				}

			/*	Union
			*/
			Range2& operator |= ( const Range2& range )
				{
					if( this->min > range.min )
						this->min = range.min;
					if( this->max < range.max )
						this->max = range.max;

					return *this;
				}

			Range2 operator | ( const Range2& range ) const
				{
					return Range2( math::Min( this->min, range.min ),
								  math::Max( this->max, range.max ) );
				}

			/*	Intersection
			*/
			Range2& operator &= ( const Range2& range )
				{
					FC_ASSERT_LOG( this->intersects( range ),
								   "No intersection..." );

					if( this->min < range.min )
						this->min = range.min;
					if( this->max > range.max )
						this->max = range.max;

					return *this;
				}
			
			Range2 operator & ( const Range2& range ) const
				{
					FC_ASSERT_LOG( this->intersects( range ),
								   "No intersection..." );

					return Range2( math::Max( this->min, range.min ),
								  math::Min( this->max, range.max ) );
				}

			constinit static const Range2 ZERO;
		};
}