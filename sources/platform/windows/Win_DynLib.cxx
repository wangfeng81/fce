/*
**
**	Win_DynLib.
**
		Windows platform implementation

	Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::win
{
	/*
	 @	ππ‘Ï
	 */
	FC_INLINE Win_DynLib::Win_DynLib( const HMODULE hModule ) noexcept
		:
			hModule( hModule )
		{
		}
}