/*
**
**	Serialization inlines.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::xml
{
	/*
	 @	Insert	a new child element
	 */
	FC_INLINE tinyxml2::XMLElement* Serialization::InsertChild( const char* const name ) NOEXCEPT
		{
			FC_ASSERT( name && *name );

			return this->xmlElement->InsertNewChildElement( name );
		}

	/*
	 @	Direct access
	 */
	FC_INLINE tinyxml2::XMLElement* Serialization::operator -> () noexcept
		{
			return this->xmlElement;
		}

	/*
	 @	Set	attribute or text
	 */
	FC_INLINE Serialization& Serialization::SetValue( const Char8* const sz8,
													  const char* const name ) NOEXCEPT
		{
			return this->SetValue( ( const char* const )sz8, name );
		}

	FC_INLINE Serialization& Serialization::SetValue( const Stl_u8string auto& str8,
													  const char* const name ) NOEXCEPT
		{
			return this->SetValue( str8.c_str(), name );
		}
}



namespace fce::inner::xml
{
	/*
	 @	Serialize	pointer object
	 */
	FC_INLINE Serialization& Serialization::Serialize( const Archive_pointer auto& obj,
													   const char* const name ) NOEXCEPT
		{
			FC_ASSERT( obj );

			return this->Serialize( *obj, name );
		}

	/*
	 @	Serialize	Stl_pointer
	 */
	FC_INLINE Serialization& Serialization::Serialize( const Stl_pointer auto& ptr,
													   const char* const name ) NOEXCEPT
		{
			FC_ASSERT( ptr );

			return this->Serialize( *ptr, name );
		}

	/*
	 @	Serialize	Raw_object
	 */
	Serialization& Serialization::Serialize( const Raw_object auto& raw,
											 const char* const name ) NOEXCEPT
		{
			if( raw )
				{
					return this->Serialize( *raw, name );
				}

			return *this;
		}

	/*
	 @	Serialize	std::ranges
	 */
	Serialization& Serialization::Serialize( const Container_object auto& container,
											 const char* const name ) NOEXCEPT
		{
			using Range_t = std::remove_reference_t< decltype( container ) >;

			using Range_value_t = std::remove_reference_t<
								  std::ranges::range_value_t< Range_t > >;

			const stl::string strGroup( stl::string( name ) + "_group" );

			if constexpr( std::ranges::sized_range< Range_t > )
				{
					if( ! std::ranges::size( container ) )
						{
							return *this;
						}
				}

			Serialization child( this->InsertChild( strGroup.c_str() ) );

			for( const auto& val : container )
				{
					if constexpr( Base_value< Range_value_t > )
						{
							Serialization( child.InsertChild( name ) )
												.Serialize( val, nullptr );
						}
					else
						{
							child.Serialize( val, name );
						}
				}

			return *this;
		}
}



namespace fce::inner::xml
{
	/*
	 @	Serialize	std::pair
	 */
	FC_INLINE Void Serialization::Serialize( const Stl_pair auto& pair ) NOEXCEPT
		{
			this->Serialize( pair.first, "first" )
				 .Serialize( pair.second, "second" );
		}

	/*
	 @	Serialize	std::tuple
	 */
	FC_INLINE Void Serialization::Serialize( const Stl_tuple auto& tp ) NOEXCEPT
		{
			auto f = [ this, &tp ]< const std::size_t ...I >
								  ( const std::index_sequence< I... > ) NOEXCEPT
				{
					return ( this->Serialize( std::get< I >( tp ),
											  Xml::MakeTupleName( I ).c_str() ), ... );
				};

			using Tuple_t = std::remove_reference_t< decltype( tp ) >;

			constexpr std::size_t N = std::tuple_size_v< Tuple_t >;

			f( std::make_index_sequence< N >{} );
		}

	/*
	 @	Serialize	object >>
	 */
	template< class T >

	requires requires{ T::ARCHIVE_NAME; }

	FC_INLINE Void Serialization::Serialize( const T& obj ) NOEXCEPT
		{
			if constexpr( std::derived_from< T, ObjectEx > )
				{
					obj.Serialize( *this );
				}
			else
				{
					obj >> *this;
				}
		}
}



namespace fce::inner::xml
{
	/*
	 @	Serialize	objects
	 */
	Serialization& Serialization::Serialize( const auto& obj,
											 const char* const name ) NOEXCEPT
		{
			Serialization child( this->InsertChild( name ) );

			child.Serialize( obj );

			return *this;
		}

	/*
	 @	Serialize	base value
	 */
	Serialization& Serialization::Serialize( const Base_value auto& value,
											 const char* const name ) NOEXCEPT
		{
			return this->SetValue( Convert( value ), name );
		}

	/*
	 @	Serialize	group
	 */
	template< class ...T >

	FC_INLINE Void Serialization::SerializeGroup( const Info< T >&... group ) NOEXCEPT
		{
			( this->Serialize( group.first,
							   group.second ), ... );
		}
}



/**
	Convert base value to utf8
 */
namespace fce::inner::xml
{
	/*
	 @	Bool
	 */
	FC_INLINE const char* Serialization::Convert( const Bool b ) noexcept
		{
			return b ? "true" : "false";
		}

	/*
	 @	Enum_value
	 */
	FC_INLINE const char* Serialization::Convert( const Enum_value auto em ) NOEXCEPT
		{
			return Enum< decltype( em ) >::Get( em );
		}

	/*
	 @	Path
	 */
	FC_INLINE const stl::u8string Serialization::Convert( const Path& path ) noexcept
		{
			return Convert( path.native() );
		}

	/*
	 @	String
	 */
	FC_INLINE const stl::u8string Serialization::Convert( const String& str ) noexcept
		{
			return Convert( str.Native() );
		}

	/*
	 @	Character
	 */
	FC_INLINE const stl::u8string Serialization::Convert( const Character auto& ch ) noexcept
		{
			return Charset::Convert< Char8 >( &ch, 1 );
		}

	/*
	 @	Real_number
	 */
	FC_INLINE const stl::u8string Serialization::Convert( const Real_number auto rn ) noexcept
		{
			return ( const Char8* )std::to_string( rn ).c_str();
		}

	/*
	 @	Stl_string
	 */
	FC_INLINE const stl::u8string Serialization::Convert( const Stl_string auto& str ) NOEXCEPT
		{
			return Charset::Convert< Char8 >( str );
		}

	/*
	 @	stl::u8string
	 */
	FC_INLINE const stl::u8string& Serialization::Convert( const stl::u8string& str8 ) noexcept
		{
			return str8;
		}

	/*
	 @	Color2
	 */
	FC_INLINE const stl::u8string Serialization::Convert( const Color2& color2 ) noexcept
		{
			return Convert( color2.value );
		}

	/*
	 @	Color4
	 */
	FC_INLINE const stl::u8string Serialization::Convert( const Color4& color4 ) noexcept
		{
			return Convert( color4.value );
		}

	/*
	 @	Color_object
	 */
	FC_INLINE const stl::u8string Serialization::Convert( const Color_object auto& color ) noexcept
		{
			return Convert( Color4( color ) );
		}
}