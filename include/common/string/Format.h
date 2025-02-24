/*
**
**	Format.
**
		boost::basic_format

	Copyright (c) WangFeng since 2012...
*/

#pragma push_macro("new")
	#undef new
	#include <boost/format.hpp>
#pragma pop_macro("new")


namespace fce
{
	/*
	 !	Format

			Unified format
	 */
	class FC_API Format final
		{
		public:
			/*	Constructor
			*/
			Format() = default;

			template< class Fmt,
					  typename ...Args >

			Format( const Fmt&,
					const Args&... );

			template< class Fmt,
					  typename ...Args >

			Format( String&,
					const Fmt&,
					const Args&... );

			/*	Conversion
			*/
			template< Character T >

			[[nodiscard]] operator stl::basic_string< T >() const noexcept;

			[[nodiscard]] operator stl::wstring() const noexcept;

			[[nodiscard]] operator std::wstring() const noexcept;

			[[nodiscard]] operator String() const noexcept;

			/*	Parse format
			*/
			template< class Fmt,
					  typename ...Args >

			stl::wstring operator() ( const Fmt&,
									  const Args&... );

		private:
			/*	Convert char set
			*/
			template< typename T >

			static auto Convert( const T& arg );

		private:
			/*	boost::wformat
			*/
			boost::basic_format< WChar, std::char_traits< WChar >,
										stl::allocator< WChar > > fmt;
		};
}


/**
 !	Inlines
 */
#include "Format.inl"