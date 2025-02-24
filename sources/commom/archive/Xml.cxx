/*
**
**	Xml.
**
		Copyright (c) WangFeng since 2006...
*/

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#pragma comment( lib, "Tinyxml2.lib" )
#elif defined  FC_ANDROID
	#pragma comment( lib, "Tinyxml2.so" )
#elif defined  FC_LINUX
	#pragma comment( lib, "Tinyxml2.so" )
#elif defined  FC_IOS
	#pragma comment( lib, "Tinyxml2.dylib" )
#elif defined  FC_MAC
	#pragma comment( lib, "Tinyxml2.dylib" )
#endif


/**
 !	Xml ���л�
 */
namespace fce::inner::xml
{
	/*
	 @	����
	 */
	Serialization::Serialization( tinyxml2::XMLElement* xmlElement ) noexcept
		:
			xmlElement( xmlElement )
		{
			FC_ASSERT( xmlElement );
		}

	/*
	 @	���� attribute or text
	 */
	Serialization& Serialization::SetValue( const char* const value,
											const char* const name ) NOEXCEPT
		{
			FC_ASSERT( value );

			if( name )
				{
					FC_ASSERT( *name  );

					this->xmlElement->SetAttribute( name, value );
				}
			else
				{
					this->xmlElement->SetText( value );
				}

			return *this;
		}

	/*
	 @	���л�	ObjectEx
	 */

	Void Serialization::Serialize( const ObjectEx& obj ) NOEXCEPT
		{
			obj.Serialize( *this );
		}
}



/**
 !	Xml �����л�
 */
namespace fce::inner::xml
{
	/*
	 @	����
	 */
	Deserialization::Deserialization( const tinyxml2::XMLElement* xmlElement ) noexcept
		:
			xmlElement( xmlElement )
		{
		}

	/*
	 @	��ȡ attribute or text
	 */
	const char* Deserialization::GetValue( const char* const name ) const noexcept
		{
			if( name )
				{
					return this->xmlElement->Attribute( name );
				}
			else
				{
					return this->xmlElement->GetText();
				}
		}

	/*
	 @	���� group
	 */
	const tinyxml2::XMLElement* Deserialization::FindGroup( const char* const name ) const NOEXCEPT
		{
			if( name )
				{
					FC_ASSERT( *name );

					const stl::string strGroupName( stl::string( name ) + "_group" );

					return this->xmlElement->FirstChildElement( strGroupName.c_str() );
				}

			return this->xmlElement;
		}

	/*
	 @	���� element
	 */
	const tinyxml2::XMLElement* Deserialization::FindElement( const char* const name ) const NOEXCEPT
		{
			if( name )
				{
					FC_ASSERT( *name );

					return this->xmlElement->FirstChildElement( name );
				}

			return this->xmlElement;
		}
}



/**
 !	Xml
 */
namespace fce
{
	/*
	 @	����
	 */
	Xml::Xml() noexcept
		{
			// ��� xml ����
			constexpr const char* szDeclaration
				{
					"xml version=\"1.0\" encoding=\"utf-8\""
				};

			tinyxml2::XMLDeclaration* xmlDeclaration
				{
					this->xmlDocument.NewDeclaration( szDeclaration )
				};

			this->xmlDocument.InsertFirstChild( xmlDeclaration );
		}

	/*
	 @	���� Path
	 */
	Xml::Xml( const Path& path ) NOEXCEPT
		:
			Xml( Buffer( path ) )
		{
		}

	Xml::Xml( RESULT& res,
			  const Path& path ) NOEXCEPT
		:
			Xml( res, Buffer( res, path ) )
		{
		}

	/*
	 @	���� Buffer
	 */
	Xml::Xml( const Buffer& buf ) NOEXCEPT
		{
			this->xmlDocument.Parse( buf.Ptr< const char >(),
									 buf.Size() );
		}

	Xml::Xml( RESULT& res,
			  const Buffer& buf ) NOEXCEPT
		{
			const tinyxml2::XMLError xmlError
				{
					this->xmlDocument.Parse( buf.Ptr< const char >(),
											 buf.Size() )
				};

			res = RESULT( xmlError );
		}

	/*
	 @	���� ObjectEx
	 */
	Xml::Xml( const ObjectEx& obj,
			  const stl::string& name ) NOEXCEPT
		:
			Xml()
		{
			Serialization serialization
				(
					this->NewElement( name.c_str() )
				);

			obj.Serialize( serialization );
		}

	/*
	 @	�½� element
	 */
	tinyxml2::XMLElement* Xml::NewElement( const char* const name ) NOEXCEPT
		{
			FC_ASSERT( name && *name );

			// �½� XMLElement
			tinyxml2::XMLElement* xmlElement
				{
					this->xmlDocument.NewElement( name )
				};

			// ��� XMLElement �� doc
			FC_ASSERT( xmlElement );

			this->xmlDocument.InsertEndChild( xmlElement );

			return xmlElement;
		}


	/*
	 @	���� tuple Ԫ������
	 */
	std::string Xml::MakeTupleName( const std::size_t idx ) noexcept
		{
			return "tuple_" + std::to_string( idx );
		};

	/*
	 @	����
	 */
	RESULT Xml::Save( const Path& name ) noexcept
		{
			return RESULT( this->xmlDocument.SaveFile( name.string().c_str() ) );
		}
}