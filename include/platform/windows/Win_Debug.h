/*
**
**	Win_Debug.
**
		Windows platform implementation

	Copyright (c) WangFeng since 2000...
*/


/*	Detecting memory leaks
*/
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define new new( _CLIENT_BLOCK, __FILE__, __LINE__ )


namespace fce::inner::win
{
	/*
	 !	Win Debug
	 */
	class Win_Debug final
		{
			/*	Friends
			*/
			friend Debug;

		public:
			/*	Is debugging
			*/
			Bool isDebugging() const;

			/*	Console output
			*/
			Void Console( const std::wstring& ) const;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Win_Debug );

			~Win_Debug() = default;

			Win_Debug();
		};
}



/*
 !	Implementation
 */
namespace fce::inner::debug
{
	/*
	 @	Debug win impl
	 */
	using Impl = win::Win_Debug;

	/*
	 @	Win_Debug singleton shortcut.
	 */
	#define s_Win_Debug s_Debug.impl
}



/**
 !	class Win_Debug
 */
namespace fce::inner::win
{
	/*
	 @	isDebugging
	 */
	#define FC_DEBUG_BREAK	DebugBreak();

	/*
	 @	Constructor
	 */
	FC_INLINE Win_Debug::Win_Debug()
		{
			_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF
						  | _CRTDBG_LEAK_CHECK_DF );
		}

	/*
	 @	Check whether debugging is currently in progress
	 */
	FC_INLINE Bool Win_Debug::isDebugging() const
		{
			return IsDebuggerPresent();
		}

	/*
	 @	Output to console
	 */
	FC_INLINE Void Win_Debug::Console( const std::wstring& str ) const
		{
			OutputDebugStringW( ( str + L'\n' ).c_str() );
		}
}