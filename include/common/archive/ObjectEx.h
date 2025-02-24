/*
**
**	ObjectEx.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 !	Object ex
	 */
	class FC_API ObjectEx
		:
		private Object
		{
		public:
			/*	Serialize
			*/
			Buffer& Serialize( Buffer& ) const NOEXCEPT;

			virtual Buffer& operator >> ( Buffer& ) const NOEXCEPT;

			/*	Class name
			*/
			[[nodiscard]] virtual DWord classHash() const noexcept;

			[[nodiscard]] virtual const stl::string& className() const noexcept;

		protected:
			/*	Reflection base
			*/
			REFLECTION_BASE( ObjectEx )

			/*	Constructor & Destructor
			*/
			using Object::Object;

			~ObjectEx() override = default;

			/*	Deserialize
			*/
			virtual const Buffer& operator << ( const Buffer& );

			/*	Editor mode
			*/
			#ifdef FC_EDITOR

				Void Serialize( Xml::Serialization& ) const NOEXCEPT;

				virtual Void operator >> ( Xml::Serialization& ) const NOEXCEPT;

				virtual Void operator << ( const Xml::Deserialization& ) NOEXCEPT;

			#endif
		};

	/*	concept		Derived object ex
	 */
	template< class T >

	concept X_objectEx = std::derived_from< T, ObjectEx >;
}



/**
 !	class ObjectEx
 */
namespace fce
{
	/*
	 @	Buffer	Serialize & Deserialize
	 */
	FC_INLINE const Buffer& ObjectEx::operator << ( const Buffer& buf )
		{
			return buf;
		}

	FC_INLINE Buffer& ObjectEx::operator >> ( Buffer& buf ) const NOEXCEPT
		{
			return buf;
		}


	#ifdef FC_EDITOR

		/*
		 @	XML		Serialize & Deserialize
		 */
		FC_INLINE Void ObjectEx::operator << ( const Xml::Deserialization& ) NOEXCEPT
			{
			}

		FC_INLINE Void ObjectEx::operator >> ( Xml::Serialization& ) const NOEXCEPT
			{
			}

	#endif
}