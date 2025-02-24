/*
**
**	android Clipboard.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#include "Clipboard.h"


namespace fce
{
	/*
	 !	Clipboard instance.
	 */
	IClipboard& Clipboard = SHARED( android::CClipboard );


	namespace android
	{
		/*
		!	Constructor.
		*/
		CClipboard::CClipboard()
		{
		}


		/*
		 !	Get
		*/
		Bool CClipboard::Get( stl::wstring& str ) const
		{
			return False;
		}


		/*
		 !	Set
		*/
		Bool CClipboard::Set( const stl::wstring& str )
		{
			return False;
		}
	}
}