/*
**
**	Archive.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Utility.h"


/**
 !	Buffer concept
 */
namespace fce
{
	/*	concept		Serializable
	 */
	template< class T >

	concept Serializable_L = requires( const T& obj, Buffer& buf )
								{
									obj >> buf;
								};

	template< class T >

	concept Serializable_R = requires( const T& obj, Buffer& buf )
								{
									buf << obj;
								};

	/*	concept		Deserializable
	 */
	template< class T >

	concept Deserializable_L = requires( T& obj, const Buffer& buf )
								{
									obj << buf;
								};

	template< class T >

	concept Deserializable_R = requires( T& obj, const Buffer& buf )
								{
									buf >> obj;
								};

	/*	concept		Archive pointer
	 */
	template< class T >

	concept Archive_pointer = Pointer_object< T >
						 && ! X_object< T >;
}


/**
 !	EDITOR
 */
#ifdef FC_EDITOR

	#include "archive/xml/Xml.h"
	#include "archive/xml/Xml.inl"

#else

	#define XML_REGISTER( T )
	#define XML_ARCHIVE( T, ... )
	#define XML_REFLECTION( T, ... )
	#define XML_DERIVED_ARCHIVE( P, T, ... )

	#define XML_CONSTRUCTOR( T )
	#define XML_CONSTRUCTOR_ENTITY( T, P )

	#define XML_ARCHIVE_DECLARATION

#endif

/**
 !	Reflection
 */
#include "archive/Reflection.h"
#include "archive/ObjectEx.h"

/**
 !	BIN
 */
#include "archive/bin/Buffer.h"
#include "archive/bin/Buffer.inl"



/**
 !	Archive info
 */
#ifdef FC_EDITOR
	#define ARCHIVE_INFO( T, U )	Archive::Info( T, U )
#else
	#define ARCHIVE_INFO( T, U )	T
#endif

#define ARCHIVE_UNIT( T )			ARCHIVE_INFO( T, #T )


/*
 @	Object archive
 */
#define OBJECT_ARCHIVE( T, ... )													\
																					\
		BIN_ARCHIVE( T, __VA_ARGS__ )												\
																					\
		XML_ARCHIVE( T, __VA_ARGS__ )												\
																					\
		ARCHIVE_CRC32( __VA_ARGS__ )


/*
 @	Derived archive
 */
#define DERIVED_ARCHIVE( T, P, ... )												\
																					\
		BIN_DERIVED_ARCHIVE( P, __VA_ARGS__ )										\
																					\
		XML_DERIVED_ARCHIVE( T, P, __VA_ARGS__ )