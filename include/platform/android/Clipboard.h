/*
**
**	android CClipboard.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "../../../interface/platform/Clipboard.h"


namespace fce
{
	namespace android
	{
		/*
		 !	android CClipboard
		 */
		class CClipboard : public IClipboard
		{
		public:
			Bool	Get( stl::wstring& str ) const;
			Bool	Set( const stl::wstring& str );

		private:
			CClipboard();
			SINGLETON( CClipboard );
		};
	}
}