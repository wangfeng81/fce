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
	 @	构造	默认 移动 拷贝
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
	 @	赋值	移动 拷贝
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
	 @	序列化 带 class hash 的反射包
	 */
	Buffer& ObjectEx::Serialize( Buffer& buf ) const NOEXCEPT
		{
			return buf << this->classHash() << *this;
		}

	/*
	 !	编辑器 模式
	 */
	#ifdef FC_EDITOR

	/*
	 @	序列化 带 class name 的反射包
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