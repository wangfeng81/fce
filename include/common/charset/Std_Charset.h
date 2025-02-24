/*
**
**	Std_Charset.
**
		C++ std implementation

	Copyright (c) WangFeng since 2012...
*/

namespace fce::inner::charset
{
	/*	Is conversion of between A and B
	 */
	#define IS_CONVERSION_BETWEEN( A, B )						\
																\
			template< typename Src, typename Dst >				\
																\
			concept Is_##A##_##B = Is_##A##_to_##B< Src, Dst >	\
								|| Is_##A##_to_##B< Dst, Src >

	/*	Is conversion of C to Utf
	 */
	#define IS_CONVERSION_TO_UTF( A, B, C )						\
																\
			template< typename Src, typename Dst >				\
																\
			concept Is_##A##_to_##B = std::same_as< Src, C >	\
								   && UTF_char< Dst >;			\
																\
			IS_CONVERSION_BETWEEN( A, B )

	/*	Is conversion of Src to Dst
	 */
	#define IS_CONVERSION_TO( A, B, S, D )						\
																\
			template< typename Src, typename Dst >				\
																\
			concept Is_##A##_to_##B = std::same_as< Src, S >	\
								   && std::same_as< Dst, D >;	\
																\
			IS_CONVERSION_BETWEEN( A, B )

	/*	Is conversion of	Std::codecvt supported
	 */
	IS_CONVERSION_TO( u32, u8, char32_t, char8_t );

	IS_CONVERSION_TO( u16, u8, char16_t, char8_t );

	IS_CONVERSION_TO( wcs, mbcs, wchar_t, char );

	/*	Is conversion of	u32 <=> u16
	 */
	IS_CONVERSION_TO( u32, u16, char32_t, char16_t );

	/*	Is conversion of	wcs | mbcs <=> unicode
	 */
	IS_CONVERSION_TO_UTF( wcs, utf, wchar_t );

	IS_CONVERSION_TO_UTF( mbcs, utf, char );

	/*	Can directly convert by Std::codecvt
	 */
	template< typename Src, typename Dst >

	concept Can_directly_convert = Is_u32_to_u8< Src, Dst >
								|| Is_u16_to_u8< Src, Dst >
								|| Is_wcs_to_mbcs< Src, Dst >;

	/*	Intermediate type of conversion

			u32  <=> u16
			mbcs <=> unicode
			wcs  <=> unicode
	 */
	template< Character Src, Character Dst >

	using Intermediate_type = std::conditional_t<
									Is_u32_u16< Src, Dst >, Char8,

							  std::conditional_t<
									Is_mbcs_utf< Src, Dst >, WChar,

							  std::conditional_t<
									Is_wcs_utf< Src, Dst >, Mapping_wchar,

															void > > >;
}


namespace fce::inner::charset
{
	/*
	 !	Std implementation.
	 */
	class Std_Charset final
		{
			/*	Friends
			*/
			friend Charset;

			/*	std::codecvt supported conversion types
			*/
			template< Character Src,
					  Character Dst >

			using Codecvt = std::codecvt< Src, Dst,
										  std::mbstate_t >;

			using Codecvt_u32_u8 = Codecvt< char32_t, char8_t >;

			using Codecvt_u16_u8 = Codecvt< char16_t, char8_t >;

			using Codecvt_wcs_mbcs = Codecvt< wchar_t, char >;

		public:
			/*	Convert string
			*/
			template< Character Src,
					  Character Dst >

			UInt Convert( const Src* const, const std::size_t,
								Dst* const, const std::size_t ) const;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Std_Charset );

			~Std_Charset() = default;

			Std_Charset() noexcept;

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize() noexcept;

		private:
			/*	Get std::codecvt reference
			*/
			template< Character Src,
					  Character Dst >

			constexpr const auto& GetCodecvt() const noexcept;

			/*	Convert directly by std::codecvt
			*/
			template< Character Src,
					  Character Dst >

			UInt DirectConvert( const Src* const, const std::size_t,
									  Dst* const, const std::size_t ) const;

			/*	Convert indirectly by std::codecvt
			*/
			template< Character Mid,
					  Character Src,
					  Character Dst >

			UInt IndirectConvert( const Src* const, const std::size_t,
										Dst* const, const std::size_t ) const;

		private:
			/*	locale
			*/
			const std::locale locale;

			/*	std::codecvt supported instance references
			*/
			const Codecvt_u32_u8& codecvt_u32_u8;

			const Codecvt_u16_u8& codecvt_u16_u8;

			const Codecvt_wcs_mbcs& codecvt_wcs_mbcs;
		};


	/*
	 @	Charset std implementation.
	 */
	using Impl = Std_Charset;
}