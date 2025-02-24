/*
**
**	CMouse.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#include "Mouse.h"

#include "../../../gui/Gui.h"


namespace fce
{
	/*
	 !	Mouse instance.
	 */
	IMouse& Mouse = SHARED( android::CMouse );


	namespace android
	{
		/*
		 ! GetPos
		*/
		IMouse::Point CMouse::GetPos() const
		{
			return this->context.pos;
		}


		/*
		 ! GetWheelDelta
		*/
		Int16 CMouse::GetWheelDelta() const
		{
			return 0;
		}


		/*
		 ! SetPos
		 @ pt	坐标点
		*/
		Bool CMouse::SetPos( const Point& pt )
		{
			return False;
		}


		/*
		 ! 检测按键
		 @ KEY		按键
		*/
		Bool CMouse::isPressing( KEY key ) const
		{
			return this->context.msg != MSG_L_UP;
		}


		/*
		 ! 捕获窗口外消息
		 @ capture		是否捕获
		*/
		Void CMouse::SetCapture( Bool capture )
		{
		}


		/*
		 ! 处理主窗口的消息
		 @ inputEvent	事件
		*/
		int32_t CMouse::onLooperInput( AInputEvent* inputEvent )
			{
				int32_t handle = 0;
				size_t count = AMotionEvent_getPointerCount( inputEvent );

				for( size_t i{ 0 }; i < count; ++i )
					{
						// 保存坐标
						this->context.pos.Set(
							(Int16)AMotionEvent_getX( inputEvent, i ),
							(Int16)AMotionEvent_getY( inputEvent, i ) );

						// 转换消息
						switch( AMotionEvent_getAction( inputEvent ) )
							{
								case AMOTION_EVENT_ACTION_UP   : this->context.msg = MSG_L_UP;	 break;
								case AMOTION_EVENT_ACTION_DOWN : this->context.msg = MSG_L_DOWN; break;
								case AMOTION_EVENT_ACTION_MOVE : this->context.msg = MSG_MOVE;	 break;
							}

						// GUI 订阅
						if( SHARED( gui::CGui ).HandleMouse( this->context ) )
							handle = 1;
					}

				return handle;
			}
	}
}