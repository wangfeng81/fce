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
 !	Xml 序列化
 */
namespace fce::inner::xml
{
	/*
	 @	构造
	 */
	Serialization::Serialization( tinyxml2::XMLElement* xmlElement ) noexcept
		:
			xmlElement( xmlElement )
		{
			FC_ASSERT( xmlElement );
		}

	/*
	 @	设置 attribute or text
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
	 @	序列化	ObjectEx
	 */

	Void Serialization::Serialize( const ObjectEx& obj ) NOEXCEPT
		{
			obj.Serialize( *this );
		}
}



/**
 !	Xml 反序列化
 */
namespace fce::inner::xml
{
	/*
	 @	构造
	 */
	Deserialization::Deserialization( const tinyxml2::XMLElement* xmlElement ) noexcept
		:
			xmlElement( xmlElement )
		{
		}

	/*
	 @	获取 attribute or text
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
	 @	查找 group
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
	 @	查找 element
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
	 @	构造
	 */
	Xml::Xml() noexcept
		{
			// 添加 xml 声名
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
	 @	构造 Path
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
	 @	构造 Buffer
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
	 @	构造 ObjectEx
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
	 @	新建 element
	 */
	tinyxml2::XMLElement* Xml::NewElement( const char* const name ) NOEXCEPT
		{
			FC_ASSERT( name && *name );

			// 新建 XMLElement
			tinyxml2::XMLElement* xmlElement
				{
					this->xmlDocument.NewElement( name )
				};

			// 添加 XMLElement 到 doc
			FC_ASSERT( xmlElement );

			this->xmlDocument.InsertEndChild( xmlElement );

			return xmlElement;
		}


	/*
	 @	生成 tuple 元素名字
	 */
	std::string Xml::MakeTupleName( const std::size_t idx ) noexcept
		{
			return "tuple_" + std::to_string( idx );
		};

	/*
	 @	存盘
	 */
	RESULT Xml::Save( const Path& name ) noexcept
		{
			return RESULT( this->xmlDocument.SaveFile( name.string().c_str() ) );
		}
}