/*
**
**	Ime.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#include "Ime.h"

#include "../../../gui/Gui.h"


namespace fce
{
	/*
	 !	Ime instance.
	 */
	IIme& Ime = SHARED( android::CIme );


	namespace android
	{
	}
}