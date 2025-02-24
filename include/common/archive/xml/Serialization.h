/*
**
**	Xml Serialization.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::xml
{
	/*
	 !	Xml serialization
	 */
	class FC_API Serialization final
		{
			friend Xml;

		public:
			/*	Info
			 */
			template< class T >

			using Info = std::pair< const T&,
									const char* const >;

		public:
			/*	Serialize	group
			*/
			template< typename ...T >

			Void SerializeGroup( const Info< T >&... ) NOEXCEPT;

		public:
			/*	Serialize	base value
			*/
			Serialization& Serialize( const Base_value auto&,
									  const char* const ) NOEXCEPT;

			/*	Serialize	pointer objects
			*/
			Serialization& Serialize( const Archive_pointer auto&,
									  const char* const ) NOEXCEPT;

			Serialization& Serialize( const Stl_pointer auto&,
									  const char* const ) NOEXCEPT;

			Serialization& Serialize( const Raw_object auto&,
									  const char* const ) NOEXCEPT;

			/*	Serialize	reference objects
			*/
			Serialization& Serialize( const Container_object auto&,
									  const char* const ) NOEXCEPT;

			Serialization& Serialize( const auto&,
									  const char* const ) NOEXCEPT;

			/*	Direct access
			*/
			[[nodiscard]] tinyxml2::XMLElement* operator -> () noexcept;

		protected:
			/*	Constructor
			*/
			Serialization( tinyxml2::XMLElement* ) noexcept;

			/*	Set	attribute or text
			*/
			Serialization& SetValue( const char* const,
									 const char* const ) NOEXCEPT;

			Serialization& SetValue( const Char8* const,
									 const char* const ) NOEXCEPT;

			Serialization& SetValue( const Stl_u8string auto&,
									 const char* const ) NOEXCEPT;

			/*	Insert a new child element
			*/
			tinyxml2::XMLElement* InsertChild( const char* const ) NOEXCEPT;

		private:
			/*	Serialize	objects
			*/
			template< class T >

			requires requires{ T::ARCHIVE_NAME; }

			Void Serialize( const T& ) NOEXCEPT;

			Void Serialize( const ObjectEx& ) NOEXCEPT;

			Void Serialize( const Stl_pair auto& ) NOEXCEPT;

			Void Serialize( const Stl_tuple auto& ) NOEXCEPT;

		private:
			/*	Convert base value to utf8
			*/
			static const char* Convert( const Bool ) noexcept;

			static const char* Convert( const Enum_value auto ) NOEXCEPT;

			static const stl::u8string Convert( const Path& ) noexcept;

			static const stl::u8string Convert( const String& ) noexcept;

			static const stl::u8string Convert( const Character auto& ) noexcept;

			static const stl::u8string Convert( const Real_number auto ) noexcept;

			static const stl::u8string Convert( const Stl_string auto& ) NOEXCEPT;

			static const stl::u8string& Convert( const stl::u8string& ) noexcept;

			static const stl::u8string Convert( const Color2& ) noexcept;

			static const stl::u8string Convert( const Color4& ) noexcept;

			static const stl::u8string Convert( const Color_object auto& ) noexcept;

		private:
			/*	XML element
			*/
			tinyxml2::XMLElement* xmlElement;
		};
}