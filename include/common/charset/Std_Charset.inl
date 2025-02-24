/*
**
**	Std_Charset inlines
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce::inner::charset
{
	/*
	 @	Get std::codecvt reference
	 */
	template< Character Src,
			  Character Dst >

	FC_INLINE constexpr const auto& Std_Charset::GetCodecvt() const noexcept
		{
			if constexpr( Is_u32_u8< Src, Dst > )
				{
					return this->codecvt_u32_u8;
				}
			else if constexpr( Is_u16_u8< Src, Dst > )
				{
					return this->codecvt_u16_u8;
				}
			else if constexpr( Is_wcs_mbcs< Src, Dst > )
				{
					return this->codecvt_wcs_mbcs;
				}
			else
				{
					static_assert( "No corresponding codecvt" );
				}
		}


	/*
	 @	Convert string directly or indirectly
	 */
	template< Character Src,
			  Character Dst >

	FC_INLINE UInt Std_Charset::Convert( const Src const* src, const UInt srcLen,
											   Dst const* dst, const UInt dstLen ) const
		{
			using Mid = Intermediate_type< Src, Dst >;

			if constexpr( std::same_as< Mid, void > )
				{
					return this->DirectConvert( src, srcLen,
												dst, dstLen );
				}
			else
				{
					return this->IndirectConvert< Mid >( src, srcLen,
														 dst, dstLen );
				}
		}


	/*
	 @	Convert directly by std::codecvt
	 */
	template< Character Src,
			  Character Dst >

	UInt Std_Charset::DirectConvert( const Src* const src, const std::size_t srcLen,
										   Dst* const dst, const std::size_t dstLen ) const
		{
			const Src* src2;
				  Dst* dst2;

			std::mbstate_t mb{};

			std::codecvt_base::result res;

			const auto& codecvt
				{
					this->GetCodecvt< Src, Dst >()
				};

			if constexpr( Can_directly_convert< Src, Dst > )
				{
					res = codecvt.out( mb,
									   src, src + srcLen, src2,
									   dst, dst + dstLen, dst2 );
				}
			else if constexpr( Can_directly_convert< Dst, Src > )
				{
					res = codecvt.in( mb,
									  src, src + srcLen, src2,
									  dst, dst + dstLen, dst2 );
				}
			else
				{
					static_assert( "No corresponding conversion" );
				}

			// assert( res == std::codecvt_base::ok );

			return dst2 - dst;
		}


	/*
	 @	Convert indirectly by std::codecvt
	 */
	template< Character Mid,
			  Character Src,
			  Character Dst >

	UInt Std_Charset::IndirectConvert( const Src* const src, const std::size_t srcLen,
											 Dst* const dst, const std::size_t dstLen ) const
		{
			// If mid is the same as src or dst, it can be forcibly converted directly
			if constexpr( Same_char< Src, Mid > )
				{
					return this->Convert( reinterpret_cast< const Mid* >( src ), srcLen,
										  dst, dstLen );
				}
			else if constexpr( Same_char< Dst, Mid > )
				{
					return this->Convert( src, srcLen,
										  reinterpret_cast< Mid* >( dst ), dstLen );
				}
			else
				{
					stl::basic_string< Mid > mid;

					// Convert src to mid
					Charset::Convert( src, srcLen, mid );

					// Convert mid to dst
					return Charset::Convert< false >( mid, dst, dstLen );
				}
		}
}