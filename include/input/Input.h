/*
**
**	Input.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "Ime.h"
#include "Mouse.h"
#include "Keyboard.h"


namespace fce
{
	/*
	 !	Input.
	 */
	class FC_API Input final
		{
		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Input );

			~Input() = default;

			Input() = default;
		};


	/*
	 @	Input singleton shortcut.
	 */
	extern FC_API Input& s_Input;
}