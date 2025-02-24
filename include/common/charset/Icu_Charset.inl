/*
**
**	Icu_Charset inlines
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::charset
{
	/*
	 @	Get converter
	 */
	template< Character T >

	FC_INLINE constexpr UConverter* Icu_Charset::GetConverter() const noexcept
		{
			return this->cnvtList[ UInt8( Charset::GetType< T >() ) ];
		}


	/*
	 @	Convert string
	 */
	template< Character Src,
			  Character Dst >

	FC_INLINE UInt Icu_Charset::Convert( const Src* const src, const UInt srcLen,
											   Dst* const dst, const UInt dstLen ) const
		{
			return Convert( this->GetConverter< Src >(),
							this->GetConverter< Dst >(),
							src, src + srcLen,
							dst, dst + dstLen )

				>> Charset::Offset< Dst >();
		}
}