/*
**
**	Debug.
**
		fce bug policy£º
			normal errors => Robustness processing
			design errors => Assertion handling at debug only
		
		In the release version
		you can switch to the exception mode of the fce
		Then all the assertions that will cause the crash
		will be converted to exceptions

			try {}
			catch( fce::Assertion ) {}

		The main purpose of exception mode is to exit gracefully
		such as 'save' 'prompt' or 'restart' and release resources

	Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../common/Singleton.h"


namespace fce
{
	/*
	 !	Assertion
	 */
	class FC_API Assertion final
		:
		public std::exception
		{
		public:
			/*	Info
			*/
			const UInt32 line;
			const UInt32 column;

			const std::string file;
			const std::string func;

			const std::wstring tips;
			const std::string expression;

			/*	Constructor
			*/
			Assertion( Assertion&& ) noexcept;

			Assertion( const Assertion& ) = default;

			Assertion( const Char* const expression,
					   const std::wstring& tips,
					   const std::source_location& =
							 std::source_location::current() );

			static Void on_Exception() noexcept;

			/*	Format
			*/
			[[nodiscard]] std::wstring Format() const;

			/*	std::exception::what
			*/
			[[nodiscard]] const char* what() const noexcept;
		};
}



#ifdef FC_DEBUG

	namespace fce{ class Debug; }

	/**
	 !	Cross platform
	 */
	#ifdef FC_WINDOWS
		#include "windows/Win_Debug.h"
	#elif defined  FC_ANDROID
		#include "android/android_Debug.h"
	#elif defined  FC_LINUX
		#include "linux/linux_Debug.h"
	#elif defined  FC_IOS
		#include "ios/ios_Debug.h"
	#elif defined  FC_MAC
		#include "mac/mac_Debug.h"
	#endif


	namespace fce
	{
		/*
		 !	Debug
		 */
		class FC_API Debug final
			{
				/*	Friends
				*/
				friend Singleton;

			public:
				/*	TYPE
				 */
				enum struct TYPE
					:
						Byte
					{
						MSG,

						ERR,
						WARN,

						MAXIMUM
					};

				/*	Implementation
				*/
				inner::debug::Impl impl;

			public:
				/*	Common
				*/
				[[nodiscard]] Bool isDebugging() const;

				/*	Console output
				*/
				Void Console( const TYPE,
							  const std::wstring& ) const;

				/*	Assert
				*/
				Bool Assert( const Assertion& ) const;

			protected:
				/*	Constructor & Destructor
				*/
				Debug() = default;

				~Debug() = default;
			};


		/*
		 @	s_Debug
		 */
		SINGLETON_SHORTCUT( Debug );
	}



	/**
	 !	class Debug
	 */
	namespace fce
	{
		/*
		 @	isDebugging
		 */
		FC_INLINE Bool Debug::isDebugging() const
			{
				return this->impl.isDebugging();
			}
	}

#endif



/**
 !	Debug macros
 */
#ifdef FC_DEBUG

	/*
	 @	Debug environment
	 */
	#define FC_LOG( fmt, ... )															\
				{																		\
					s_Debug.Console( fce::Debug::TYPE::MSG,								\
										  fce::Format( fmt, __VA_ARGS__ ) );			\
				}

	#define FC_TAG_LOG( type, fmt, ... )												\
				{																		\
					s_Debug.Console( type, fce::Format( fmt, __VA_ARGS__ ) );			\
				}

	#define FC_PROMPT( fmt, ... )														\
				{																		\
					s_Window.PromptBox( fmt, __VA_ARGS__ );								\
				}

	#define FC_WARNING( expression, fmt, ... )											\
				{																		\
					if( ! ( expression ) )												\
						{																\
							s_Debug.Console( fce::Debug::TYPE::WARN,					\
											 fce::Format( fmt, __VA_ARGS__ ) );			\
						}																\
				}

	#define FC_ASSERT( expression )														\
				{																		\
					if( ! ( expression ) )												\
						{																\
							fce::Assertion a( #expression,								\
											  L"Assertion failed..." );					\
																						\
							if( s_Debug.Assert( a ) )									\
								{														\
									FC_DEBUG_BREAK										\
								}														\
						}																\
				}

	#define FC_ASSERT_LOG( expression, fmt, ... )										\
				{																		\
					if( ! ( expression ) )												\
						{																\
							fce::Assertion a( #expression,								\
											  fce::Format( fmt, __VA_ARGS__ ) );		\
																						\
							if( s_Debug.Assert( a ) )									\
								{														\
									FC_DEBUG_BREAK										\
								}														\
						}																\
				}

#else

	/*
	 @	Non debug environment
	 */
	#define FC_LOG( fmt, ... )
	#define FC_PROMPT( fmt, ... )
	#define FC_TAG_LOG( type, fmt, ... )
	#define FC_WARNING( expression, fmt, ... )

	#ifdef FC_SECURITY

		/*
		 @	Exception environment
		 */
		#define FC_ASSERT( expression )													\
					{																	\
						if( ! ( expression ) )											\
							{															\
								try {													\
										throw fce::Assertion							\
												(										\
													#expression,						\
													L"Exception..."						\
												);										\
									}													\
								catch( ... )											\
									{													\
										Assertion::on_Exception();						\
									}													\
							}															\
					}

		#define FC_ASSERT_LOG( expression, fmt, ... )									\
					{																	\
						if( ! ( expression ) )											\
							{															\
								try {													\
										throw fce::Assertion							\
												(										\
													#expression,						\
													fce::Format( fmt, __VA_ARGS__ )		\
												);										\
									}													\
								catch( ... )											\
									{													\
										Assertion::on_Exception();						\
									}													\
							}															\
					}

	#else

		#define FC_ASSERT( expression )
		#define FC_ASSERT_LOG( expression, fmt, ... )

	#endif

#endif