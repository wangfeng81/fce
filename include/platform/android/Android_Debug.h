/*
**
**	Android_Debug.
**
		Android platform implementation

	Copyright (c) WangFeng since 2000...
*/

namespace fce
{
	/*
	 !	Android Debug
	 */
	class FC_API Android_Debug final
		{
			friend Debug;

		protected:
			/*	Implementation
			*/
			Void Break() const;

			Bool isDebugging() const;

			Void Console( const std::string& ) const;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Android_Debug );

			~Android_Debug() = default;

			Android_Debug() = default;
		};


	/*
	 @	Debug implementation.
	 */
	using DebugImpl = Android_Debug;

	#define s_Android_Debug s_Debug.impl


	/*
	 @	Breakpoint
	 */
	FC_INLINE Void Android_Debug::Break() const
		{
		}

	/*
	 @	Check whether debugging is currently in progress
	 */
	FC_INLINE Bool Android_Debug::isDebugging() const
		{
			return false;
		}

	/*
	 @	Output to console
	 */
	FC_INLINE Void Android_Debug::Console( const std::string& str ) const
		{
			__android_log_write( ANDROID_LOG_DEBUG, "",
								 ( str + L'\n' ).c_str() );
		}

	/*
	 @	ASSERT
	 */
	#define ASSERT( expression, format, ... )								\
				{															\
					if( ! ( expression ) )									\
						{													\
							__assert2( __FILE__, __LINE__,					\
									   __PRETTY_FUNCTION__,					\
									   stl::string( Format(					\
										format, __VA_ARGS__ ) ).c_str() );	\
						}													\
				}
}