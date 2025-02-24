/*
**
**	Xml Deserialization.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::xml
{
	/*
	 !	Xml deserialization
	 */
	class FC_API Deserialization final
		{
			friend Xml;

		public:
			/*	Info
			 */
			template< class T >

			using Info = std::pair< T&,
									const char* const >;

		public:
			/*	Travel child elements
			*/
			Void Travel( auto&& ) const NOEXCEPT;

			Void Travel( auto&&,
						 const char* const ) const NOEXCEPT;

			/*	Deserialize	group
			*/
			template< typename ...T >

			Void DeserializeGroup( Info< T >&&... ) const NOEXCEPT;

		public:
			/*	Deserialize	base value
			*/
			const Deserialization& Deserialize( Base_value auto&,
												const char* const ) const NOEXCEPT;

			/*	Deserialize	pointer objects
			*/
			const Deserialization& Deserialize( Archive_pointer auto&,
												const char* const ) const NOEXCEPT;

			const Deserialization& Deserialize( Stl_pointer auto&,
												const char* const ) const NOEXCEPT;

			/*	Deserialize	reference objects
			*/
			const Deserialization& Deserialize( auto&,
												const char* const ) const NOEXCEPT;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator Bool() const noexcept;

			/*	Direct access
			*/
			[[nodiscard]] const tinyxml2::XMLElement* operator -> () const noexcept;

		protected:
			/*	Constructor
			*/
			Deserialization( const tinyxml2::XMLElement* ) noexcept;

			/*	Get	attribute or text
			*/
			const char* GetValue( const char* const ) const noexcept;

			/*	Find a element
			*/
			const tinyxml2::XMLElement* FindGroup( const char* const ) const NOEXCEPT;

			const tinyxml2::XMLElement* FindElement( const char* const ) const NOEXCEPT;

		private:
			/*	Deserialize	objects
			*/
			template< class T >

			requires requires{ T::ARCHIVE_NAME; }

			Void Deserialize( T& ) const NOEXCEPT;

			template< class T, class U >

			Void Deserialize( std::pair< T, U >& ) const NOEXCEPT;

			template< typename ...Args >

			Void Deserialize( std::tuple< Args... >& ) const NOEXCEPT;

			Void Deserialize( Container_object auto& ) const NOEXCEPT;

			Void Deserialize( Raw_object auto& ) const NOEXCEPT;

		private:
			/*	Convert base value from utf8
			*/
			static Void Convert( Bool&,
								 const char* const ) noexcept;

			static Void Convert( Path&,
								 const char* const ) noexcept;

			static Void Convert( String&,
								 const char* const ) noexcept;

			static Void Convert( Stl_string auto&,
								 const char* const ) noexcept;

			static Void Convert( Character auto&,
								 const char* const ) noexcept;

			static Void Convert( Enum_value auto&,
								 const char* const ) noexcept;

			static Void Convert( Real_number auto&,
								 const char* const ) noexcept;

			static Void Convert( Color2&,
								 const char* const ) noexcept;

			static Void Convert( Color4&,
								 const char* const ) noexcept;

			static Void Convert( Color_object auto&,
								 const char* const ) noexcept;

		private:
			/*	XML element
			*/
			const tinyxml2::XMLElement* xmlElement;
		};
}