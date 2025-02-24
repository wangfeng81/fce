/*
**
**	Singleton.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Types.h"


namespace fce
{
	/*
	 !	Singleton
	 */
	class FC_API Singleton final
		{
			/*	Constructor & Destructor
			*/
			~Singleton() = delete;

			Singleton() = delete;

		public:
			/*	Get dynamic instance
			*/
			template< class T >

			static T& Get() NOEXCEPT;
		};
}



/**
 !	class Singleton
 */
namespace fce
{
	/*
	 @	Get dynamic instance
	 */
	template< class T >

	T& Singleton::Get() NOEXCEPT
		{
			static T instance;

			return instance;
		}
}



/*
 @	Singleton shortcut
 */
#define SINGLETON_SHORTCUT_EX( T, A )			\
												\
		inline T& s_##A							\
			{									\
				fce::Singleton::Get< T >()		\
			};

#define SINGLETON_SHORTCUT( T )					\
												\
		SINGLETON_SHORTCUT_EX( T, T )

/*
 @	Singleton instance
 */
#define SINGLETON_INSTANCE( T )					\
												\
		inline static T& Instance() NOEXCEPT	\
			{									\
				static T instance;				\
												\
				return instance;				\
			}