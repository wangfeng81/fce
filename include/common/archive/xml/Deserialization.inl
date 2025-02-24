/*
**
**	Deserialization inlines.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::xml
{
	/*
	 @	Check whether the xml is valid
	 */
	FC_INLINE Deserialization::operator Bool() const noexcept
		{
			return this->xmlElement;
		}

	/*
	 @	Direct access
	 */
	FC_INLINE const tinyxml2::XMLElement* Deserialization::operator -> () const noexcept
		{
			return this->xmlElement;
		}

	/*
	 @	Deserialize	pointer object
	 */
	FC_INLINE const Deserialization& Deserialization::Deserialize( Archive_pointer auto& obj,
																   const char* const name ) const NOEXCEPT
		{
			FC_ASSERT( obj );

			return this->Deserialize( *obj, name );
		}

	/*
	 @	Deserialize	Stl_pointer
	 */
	FC_INLINE const Deserialization& Deserialization::Deserialize( Stl_pointer auto& ptr,
																   const char* const name ) const NOEXCEPT
		{
			FC_ASSERT( ptr );

			return this->Deserialize( *ptr, name );
		}
}



namespace fce::inner::xml
{
	/*
	 @	Deserialize	object >>
	 */
	template< class T >

	requires requires{ T::ARCHIVE_NAME; }

	FC_INLINE Void Deserialization::Deserialize( T& obj ) const NOEXCEPT
		{
			obj << *this;
		}

	/*
	 @	Deserialize	std::pair
	 */
	template< class T, class U >

	FC_INLINE Void Deserialization::Deserialize( std::pair< T, U >& pair ) const NOEXCEPT
		{
			this->Deserialize( const_cast< std::remove_const_t< T >& >( pair.first ), "first" )
				 .Deserialize( const_cast< std::remove_const_t< U >& >( pair.second ), "second" );
		}

	/*
	 @	Deserialize	std::tuple
	 */
	template< typename ...Args >

	FC_INLINE Void Deserialization::Deserialize( std::tuple< Args... >& tp ) const NOEXCEPT
		{
			auto f = [ this, &tp ]< const std::size_t ...I >
								  ( const std::index_sequence< I... > ) NOEXCEPT
				{
					return ( this->Deserialize( std::get< I >( tp ),
												Xml::MakeTupleName( I ).c_str() ), ... );
				};

			f( std::index_sequence_for< Args... >{} );
		}

	/*
	 @	Deserialize	Container_object
	 */
	FC_INLINE Void Deserialization::Deserialize( Container_object auto& container ) const NOEXCEPT
		{
			using Range_t = std::remove_reference_t< decltype( container ) >;

			using Range_value_t = std::remove_reference_t<
								  std::ranges::range_value_t< Range_t > >;

			auto f = [ &container ]( const Deserialization& deserialization,
									 const std::size_t index ) NOEXCEPT
				{
					if constexpr( Fixed_array< Range_t > )
						{
							FC_ASSERT( index < std::ranges::size( container ) );

							deserialization.Deserialize( container[ index ], nullptr );
						}
					else
						{
							Range_value_t val;

							deserialization.Deserialize( val, nullptr );

							if constexpr( Container_with_resize< Range_t > )
								{
									if constexpr( Template< Range_t, std::forward_list > )
										{
											container.emplace_front( std::move( val ) );
										}
									else
										{
											container.emplace_back( std::move( val ) );
										}
								}
							else if constexpr( Container_with_emplace< Range_t > )
								{
									container.emplace( std::move( val ) );
								}
						}
				};

				this->Travel( f );
		}

	/*
	 @	Deserialize	Raw_object
	 */
	Void Deserialization::Deserialize( Raw_object auto& raw ) const NOEXCEPT
		{
			this->Deserialize( *raw.Construct() );
		}
}



namespace fce::inner::xml
{
	/*
	 @	Deserialize	objects
	 */
	const Deserialization& Deserialization::Deserialize( auto& obj,
														 const char* const name ) const NOEXCEPT
		{
			using T = std::remove_reference_t< decltype( obj ) >;

			const tinyxml2::XMLElement* child;

			if constexpr( Container_object< T > )
				{
					child = this->FindGroup( name );
				}
			else
				{
					child = this->FindElement( name );
				}

			if( child )
				{
					Deserialization( child ).Deserialize( obj );
				}

			return *this;
		}

	/*
	 @	Deserialize	base value
	 */
	const Deserialization& Deserialization::Deserialize( Base_value auto& obj,
														 const char* const name ) const NOEXCEPT
		{
			if( const char* const value{ this->GetValue( name ) } )
				{
					Convert( obj, value );
				}

			return *this;
		}

	/*
	 @	Travel child elements
	 */
	Void Deserialization::Travel( auto&& func ) const NOEXCEPT
		{
			std::size_t index{ 0 };

			for( const tinyxml2::XMLElement* child
					{
						this->xmlElement->FirstChildElement()
					};
				 child; child = child->NextSiblingElement() )
				{
					func( child, index++ );
				}
		}

	/*
	 @	Travel child group
	 */
	Void Deserialization::Travel( auto&& func,
								  const char* const name ) const NOEXCEPT
		{
			const tinyxml2::XMLElement* const group
				{
					this->FindGroup( name )
				};

			if( group )
				{
					Deserialization( group ).Travel( func );
				}
		}

	/*
	 @	Deserialize	group
	 */
	template< class ...T >

	FC_INLINE Void Deserialization::DeserializeGroup( Info< T >&&... group ) const NOEXCEPT
		{
			( this->Deserialize( group.first,
								 group.second ), ... );
		}
}



/**
	Convert base value from utf8
 */
namespace fce::inner::xml
{
	/*
	 @	Bool
	 */
	FC_INLINE Void Deserialization::Convert( Bool& b,
											 const char* const value ) noexcept
		{
			b = strcmp( value, "false" );
		}

	/*
	 @	Path
	 */
	FC_INLINE Void Deserialization::Convert( Path& path,
											 const char* const value ) noexcept
		{
			path = ( const Char8* )value;
		}

	/*
	 @	String
	 */
	FC_INLINE Void Deserialization::Convert( String& str,
											 const char* const value ) noexcept
		{
			str = ( const Char8* )value;
		}

	/*
	 @	Character
	 */
	FC_INLINE Void Deserialization::Convert( Character auto& ch,
											 const char* const value ) noexcept
		{
			Charset::Convert< false >( ( const Char8* )value,
									   strlen( value ),
									   &ch, 1 );
		}

	/*
	 @	Stl_string
	 */
	FC_INLINE Void Deserialization::Convert( Stl_string auto& str,
											 const char* const value ) noexcept
		{
			Charset::Convert( ( const Char8* )value, str );
		}

	/*
	 @	Enum_value
	 */
	FC_INLINE Void Deserialization::Convert( Enum_value auto& em,
											 const char* const value ) noexcept
		{
			using T = std::remove_reference_t< decltype( em ) >;

			em = Enum< T >::Get( value );
		}

	/*
	 @	Real_number
	 */
	FC_INLINE Void Deserialization::Convert( Real_number auto& rn,
											 const char* const value ) noexcept
		{
			using T = std::remove_reference_t< decltype( rn ) >;

			char* endPtr{ nullptr };

			if constexpr( std::same_as< T, Float32 > )
				{
					rn = strtof( value, &endPtr );
				}
			else if constexpr( std::same_as< T, Float64 > )
				{
					rn = std::strtod( value, &endPtr );
				}
			else if constexpr( std::same_as< T, long double > )
				{
					rn = std::strtold( value, &endPtr );
				}
			else if constexpr( Signed_integer< T > )
				{
					rn = static_cast< T >( strtoll( value, &endPtr, 0 ) );
				}
			else if constexpr( Unsigned_integer< T > )
				{
					rn = static_cast< T >( strtoull( value, &endPtr, 0 ) );
				}
			else
				{
					std::unreachable();
				}

			FC_WARNING( value != endPtr,
						"invalid real number string" );
		}

	/*
	 @	Color2
	 */
	FC_INLINE Void Deserialization::Convert( Color2& color2,
											 const char* const value ) noexcept
		{
			Convert( color2.value, value );
		}

	/*
	 @	Color4
	 */
	FC_INLINE Void Deserialization::Convert( Color4& color4,
											 const char* const value ) noexcept
		{
			Convert( color4.value, value );
		}

	/*
	 @	Color_object
	 */
	Void Deserialization::Convert( Color_object auto& color,
								   const char* const value ) noexcept
		{
			Color4 color4;

			Convert( color4.value, value );

			color = color4;
		}
}