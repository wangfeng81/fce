/*
**
**	Icu_Charset.
**
		ICU implementation

	Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::charset
{
	/*
	 !	Icu types
	 */
	typedef struct UConverter UConverter;

	typedef enum UErrorCode { U_ZERO_ERROR } UErrorCode;


	/*
	 !	Icu implementation.
	 */
	class Icu_Charset final
		{
			/*	Friends
			*/
			friend Charset;

		public:
			/*	Convert string
			*/
			template< Character Src,
					  Character Dst >

			UInt Convert( const Src* const, const UInt,
								Dst* const, const UInt ) const;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Icu_Charset );

			~Icu_Charset() = default;

			Icu_Charset() noexcept;

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			UErrorCode Initialize() noexcept;

		private:
			/*	Load & Query icu
			*/
			static Bool Load() noexcept;

			static Handle Query( const DynLib&,
								 stl::string& ) noexcept;

			/*	Get converter
			*/
			template< Character T >

			constexpr UConverter* GetConverter() const noexcept;

			/*	Convert string
			*/
			static UInt Convert( UConverter* const, UConverter* const,
								 const Void* const, const Void* const,
									   Void* const,		  Void* const );

		private:
			/*	Converter list
			*/
			UConverter* cnvtList[ UInt8( TYPE::MAXIMUM ) ];
		};


	/*
	 @	Charset icu implementation.
	 */
	using Impl = Icu_Charset;
}