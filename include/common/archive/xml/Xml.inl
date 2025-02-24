/*
**
**	Xml inlines
**
		Copyright (c) WangFeng since 2006...
*/

#include "Serialization.inl"
#include "Deserialization.inl"


namespace fce
{
	/*
	 @	Construc from object
	 */
	template< class T >

	requires requires{ T::ARCHIVE_NAME; }

	Xml::Xml( const T& obj,
			  const Char* const name ) NOEXCEPT
		:
			Xml()
		{
			Serialization serialization
				(
					this->NewElement
						(
							name ? name
								 : T::ARCHIVE_NAME
						)
				);

			obj >> serialization;
		}

	/*
	 @	Deserialize to object
	 */
	template< class T >

	requires requires{ T::ARCHIVE_NAME; }

	Void Xml::operator >> ( T& obj ) const NOEXCEPT
		{
			Deserialization deserialization
				(
					//this->FindElement( T::ARCHIVE_NAME )
					this->xmlDocument.RootElement()
				);

			if( deserialization )
				{
					obj << deserialization;
				}
		}

	/*
	 @	Check whether the xml is valid
	 */
	FC_INLINE Xml::operator Bool() const noexcept
		{
			return ! this->xmlDocument.Error();
		}

	/*
	 @	Find element
	 */
	FC_INLINE const tinyxml2::XMLElement* Xml::FindElement( const char* const name ) const NOEXCEPT
		{
			FC_ASSERT( name && *name );

			return this->xmlDocument.FirstChildElement( name );
		}
}



/*
 @	Xml macros
 */
#define XML_REGISTER( T )																			\
																									\
		reflectionXml.Register< T >()

#define XML_REFLECTION( T, ... )																	\
																									\
		REFLECTION_DECL( Xml, T, __VA_ARGS__, const ::fce::Xml::Deserialization& )

#define XML_CONSTRUCTOR( T )																		\
																									\
		explicit T( const ::fce::Xml::Deserialization& deser )										\
			{																						\
				*this << deser;																		\
			}

#define XML_CONSTRUCTOR_ENTITY( T, P )																\
																									\
		explicit T( ::fce::Entity& entity,															\
					const ::fce::Xml::Deserialization& deser )										\
			:																						\
				P( entity )																			\
			{																						\
				*this << deser;																		\
			}

 /*
  @	Xml archive declaration
  */
#define XML_ARCHIVE_DECLARATION																		\
																									\
		friend ::fce::Xml;																			\
																									\
		friend ::fce::Xml::Serialization;															\
																									\
		friend ::fce::Xml::Deserialization;


/*
 @	Xml archive
 */
#define XML_ARCHIVE( T, ... )																		\
																									\
		XML_ARCHIVE_DECLARATION																		\
																									\
		static constexpr inline char ARCHIVE_NAME[] = #T;											\
																									\
		void operator >> ( ::fce::Xml::Serialization& serialization ) const NOEXCEPT				\
			{																						\
				using Archive = ::fce::Xml::Serialization;											\
																									\
				serialization.SerializeGroup( __VA_ARGS__ );										\
			}																						\
																									\
		void operator << ( const ::fce::Xml::Deserialization& deserialization ) NOEXCEPT			\
			{																						\
				using Archive = ::fce::Xml::Deserialization;										\
																									\
				deserialization.DeserializeGroup( __VA_ARGS__ );									\
			}


/*
 @	Xml derived archive
 */
#define XML_DERIVED_ARCHIVE( T, P, ... )															\
																									\
		XML_ARCHIVE_DECLARATION																		\
																									\
		inline static const char ARCHIVE_NAME[] = #T;												\
																									\
		void operator >> ( ::fce::Xml::Serialization& serialization ) const NOEXCEPT override		\
			{																						\
				using Archive = ::fce::Xml::Serialization;											\
																									\
				P::operator >> ( serialization );													\
																									\
				serialization.SerializeGroup( __VA_ARGS__ );										\
			}																						\
																									\
		void operator << ( const ::fce::Xml::Deserialization& deserialization ) NOEXCEPT override	\
			{																						\
				using Archive = ::fce::Xml::Deserialization;										\
																									\
				P::operator << ( deserialization );													\
																									\
				deserialization.DeserializeGroup( __VA_ARGS__ );									\
			}