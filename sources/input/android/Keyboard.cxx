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
		!	构造.
		*/
		CKeyboard::CKeyboard()
		{
		}

		/*
		!	析构.
		*/
		CKeyboard::~CKeyboard()
		{
		}


		/*
		 ! 获取 按键
		 @ KEY		按键
		*/
		IKeyboard::KEY CKeyboard::GetKey() const
		{
			return this->context.key;
		}


		/*
		 ! 检测按键
		 @ KEY		按键
		*/
		Bool CKeyboard::isPressing( KEY key ) const
		{
			return False;
		}
	}
}