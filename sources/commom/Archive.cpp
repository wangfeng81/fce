/*
**
**	Archive.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/resource/Storage.h"


/**
 !	class Object
 */
namespace fce
{
	/*
	 @	����	Ĭ�� �ƶ� ����
	 */
	Object::Object() noexcept
		:
			memoryPool( nullptr )
		{
		}

	Object::Object( Object&& obj ) noexcept
		:
			memoryPool( obj.memoryPool )
		{
		}

	Object::Object( const Object& obj ) noexcept
		:
			memoryPool( obj.memoryPool )
		{
		}

	/*
	 @	��ֵ	�ƶ� ����
	 */
	Object& Object::operator = ( Object&& obj ) noexcept
		{
			return *this;
		}

	Object& Object::operator = ( const Object& obj ) noexcept
		{
			return *this;
		}
}



/**
 !	class ObjectEx
 */
namespace fce
{
	/*
	 @	Class hash
	 */
	DWord ObjectEx::classHash() const noexcept
		{
			static const DWord CLASS_HASH( Crc32( "ObjectEx" ) );

			return CLASS_HASH;
		}


	/*
	 @	Class name
	 */
	const stl::string& ObjectEx::className() const noexcept
		{
			static const stl::string CLASS_NAME( "ObjectEx" );

			return CLASS_NAME;
		}


	/*
	 @	���л� �� class hash �ķ����
	 */
	Buffer& ObjectEx::Serialize( Buffer& buf ) const NOEXCEPT
		{
			return buf << this->classHash() << *this;
		}

	/*
	 !	�༭�� ģʽ
	 */
	#ifdef FC_EDITOR

	/*
	 @	���л� �� class name �ķ����
	 */
		Void ObjectEx::Serialize( Xml::Serialization& serialization ) const NOEXCEPT
			{
				serialization.Serialize( this->className(),
										 "className" );

				*this >> serialization;
			}

	#endif
}



/**
 !	Includes
 */
#include "Archive/Buffer.cxx"

#ifdef FC_EDITOR

	#include "Archive/Xml.cxx"

#endif