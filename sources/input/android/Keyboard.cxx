/*
**
**	CKeyboard.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#include "Keyboard.h"


namespace fce
{
	/*
	 !	Keyboard instance.
	 */
	IKeyboard& Keyboard = SHARED( android::CKeyboard );


	namespace android
	{
		/*
		!	����.
		*/
		CKeyboard::CKeyboard()
		{
		}

		/*
		!	����.
		*/
		CKeyboard::~CKeyboard()
		{
		}


		/*
		 ! ��ȡ ����
		 @ KEY		����
		*/
		IKeyboard::KEY CKeyboard::GetKey() const
		{
			return this->context.key;
		}


		/*
		 ! ��ⰴ��
		 @ KEY		����
		*/
		Bool CKeyboard::isPressing( KEY key ) const
		{
			return False;
		}
	}
}