/*
**
**	String.
**
		Unify all strings

	Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Charset.h"


namespace fce
{
	/*
	 !	String

			Unified string
	 */
	class FC_API String final
		{
		public:
			/*	Constructor
			*/
			String() = default;

			String( const String& ) noexcept;

			String( String&& ) noexcept;

			/*	Constructor
			 
					Char32 *
					std::u32string
					std::u32string_view

					std::filesystem::path
			*/
			String( const Char32* const );

			String( const Char32* const,
					const UInt len );

			String( const stl::u32string& ) noexcept;

			String( stl::u32string&& ) noexcept;

			String( const std::u32string_view& ) noexcept;

			String( const std::filesystem::path& ) noexcept;

			/*	Constructor
			
					Convert charset
					from a std::basic_string
					  or a std::basic_string_view
					  or a null-terminated string
			*/
			String( const Character auto* const );

			String( const Character auto* const,
					const UInt len );

			String( const Stl_string auto& ) noexcept;

			String( const Stl_string_view auto& ) noexcept;

			/*	Constructor

					Forward to the constructor
					of the stl::u32string
			*/
			template< std::constructible_from
						   < stl::u32string > ...Args >

			String( Args&& ...args )
				:
					u32str( std::forward< Args >( args )... )
				{
				}

			/*	Conversion
			
					Char32 *
					stl::u32string
			*/
			[[nodiscard]] operator stl::u32string& () noexcept;

			[[nodiscard]] operator const stl::u32string& () const noexcept;

			[[nodiscard]] const Char32* const operator * () const noexcept;

			/*	Conversion
			
					Convert charset
					to a std::basic_string
			*/
			template< Character T >

			[[nodiscard]] operator stl::basic_string< T >() const noexcept;

			/*	Assignment
			*/
			String& Assign( const Char32* const,
							const UInt len );

			String& Assign( const Character auto* const,
							const UInt len );

			/*	Assignment
			*/
			String& operator = ( const String& ) noexcept;

			String& operator = ( String&& ) noexcept;

			/*	Assignment
			
					Char32
					Char32 *
					std::u32string
					std::u32string_view
					std::filesystem::path
					std::initializer_list< Char32 >
			*/
			String& operator = ( const Char32 ) noexcept;

			String& operator = ( const Char32* const );

			String& operator = ( const stl::u32string& ) noexcept;

			String& operator = ( stl::u32string&& ) noexcept;

			String& operator = ( const std::u32string_view& ) noexcept;

			String& operator = ( const std::filesystem::path& ) noexcept;

			String& operator = ( const std::initializer_list< Char32 >& ) noexcept;

			/*	Assignment

					Convert charset
					from a std::basic_string
					  or a std::basic_string_view
					  or a null-terminated string

					  or a integer
					  or a floating number
			*/
			String& operator = ( const Character auto* const );

			String& operator = ( const Stl_string auto& ) noexcept;

			String& operator = ( const Stl_string_view auto& ) noexcept;

			String& operator = ( const Real_number auto ) noexcept;

			/*	operator +=

					Append a String

					Include convert charset
					from a std::basic_string
					  or a std::basic_string_view
					  or a null-terminated string

					  or a integer
					  or a floating number
			*/
			String& operator += ( const String& ) noexcept;

			String& operator += ( const Real_number auto ) noexcept;

			/*	operator +=

					Char32
					Char32 *
					std::u32string
					std::u32string_view
			*/
			String& operator += ( const Char32 ) noexcept;

			String& operator += ( const Char32* const );

			String& operator += ( const stl::u32string& ) noexcept;

			String& operator += ( const std::u32string_view& ) noexcept;

			String& operator += ( const std::initializer_list< Char32 >& ) noexcept;

			/*	Iterator
			*/
			[[nodiscard]] auto end( this auto& ) noexcept;

			[[nodiscard]] auto begin( this auto& ) noexcept;

			/*	Access
			*/
			[[nodiscard]] auto operator -> ( this auto& ) noexcept;

			[[nodiscard]] auto& operator [] ( this auto&, const UInt );

			/*	Native
			*/
			[[nodiscard]] constexpr const auto& Native() const noexcept;

			/*	Empty
			*/
			[[nodiscard]] constexpr Bool Empty() const noexcept;

			/*	Clear
			*/
			constexpr Void Clear() noexcept;

			/*	Length
			*/
			[[nodiscard]] constexpr UInt Length() const noexcept;

			[[nodiscard]] static constexpr UInt Length( const Character auto* const );

			/*	Size
			
					Includes '\0'
			*/
			[[nodiscard]] constexpr UInt Size() const noexcept;

			[[nodiscard]] static constexpr UInt Size( const Character auto* const );

			/*
			 @	Memory size
			 */
			template< Bool NULL_TERMINATOR = true >

			[[nodiscard]] constexpr UInt Capacity() const noexcept;

			template< Character T >

			[[nodiscard]] static constexpr UInt Capacity( const T* const );

			template< Bool NULL_TERMINATOR = true,
					  Stl_string T >

			[[nodiscard]] static constexpr UInt Capacity( const T& ) noexcept;

			template< Bool NULL_TERMINATOR = true,
					  Stl_string_view T >

			[[nodiscard]] static constexpr UInt Capacity( const T& ) noexcept;

			template< Character T >

			[[nodiscard]] static constexpr UInt Capacity( const UInt len ) noexcept;

			/*	Copy length-limited string
			*/
			template< Character T >
	
			static Void Copy( const T* src,
									T* dst );

			/*	Copy length-limited string
			*/
			template< Character T >
	
			static auto Copy( const T* const src,
									T* const dst,
							  const UInt count );

			/*	Move length-limited string
			*/
			template< Character T >
	
			static auto Move( const T* const src,
									T* const dst,
							  const UInt count );
	
		private:
			/*	UTF_32 string
			*/
			stl::u32string u32str;
		};


	/*	concept		Character string ex
	 */
	template< typename T >

	concept Character_string_ex = Character_string< T >
							   || std::same_as< std::remove_cvref_t< T >, String >;
}


/**
 !	Format
 */
#include "string/Format.h"

/**
 !	Inlines
 */
#include "string/String.inl"