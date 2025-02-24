/*
**
**	Xml.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma push_macro("new")
	#undef new
	#include "../../../../third_party/tinyxml2/tinyxml2.h"
#pragma pop_macro("new")


namespace fce
{
	class Xml;

	namespace inner::xml
	{
		/*	concept		convert able to utf8
		 */
		template< class T >

		concept Base_value = Character< T >
						  || Stl_string< T >
						  || Enum_value< T >
						  || Real_number< T >
						  || Color_object< T >
						  || Any_of< std::remove_cvref_t< T >,
									 Bool, Path, String >;
	}
}


/**
 !	Includs
 */
#include "Serialization.h"

#include "Deserialization.h"


namespace fce
{
	/*
	 !	Xml
	 */
	class FC_API Xml final
		{
		public:
			/*	Using inner kit
			 */
			using Serialization = inner::xml::
								  Serialization;

			using Deserialization = inner::xml::
									Deserialization;

			/*	Friends
			*/
			friend Serialization;

			friend Deserialization;

		public:
			/*	Constructor
			*/
			Xml() noexcept;

			Xml( RESULT&,
				 const Path& ) NOEXCEPT;

			Xml( const Path& ) NOEXCEPT;

			Xml( RESULT&,
				 const Buffer& ) NOEXCEPT;

			Xml( const Buffer& ) NOEXCEPT;

			/*	Construc from object
			*/
			Xml( const ObjectEx&,
				 const stl::string& ) NOEXCEPT;

			template< class T >

			requires requires{ T::ARCHIVE_NAME; }

			Xml( const T&,
				 const Char* const = nullptr ) NOEXCEPT;

			/*	Deserialize to object
			*/
			template< class T >

			requires requires{ T::ARCHIVE_NAME; }

			Void operator >> ( T& ) const NOEXCEPT;

			/*	Save
			*/
			RESULT Save( const Path& ) noexcept;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator Bool() const noexcept;

		private:
			/*	Make tuple element name
			*/
			static std::string MakeTupleName( const std::size_t ) noexcept;

			/*	New element
			*/
			tinyxml2::XMLElement* NewElement( const char* const ) NOEXCEPT;

			/*	Find element
			*/
			const tinyxml2::XMLElement* FindElement( const char* const ) const NOEXCEPT;

		private:
			/*	XML element
			*/
			tinyxml2::XMLDocument xmlDocument;
		};
}