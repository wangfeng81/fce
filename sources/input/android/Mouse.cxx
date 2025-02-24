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
		 @ pt	�����
		*/
		Bool CMouse::SetPos( const Point& pt )
		{
			return False;
		}


		/*
		 ! ��ⰴ��
		 @ KEY		����
		*/
		Bool CMouse::isPressing( KEY key ) const
		{
			return this->context.msg != MSG_L_UP;
		}


		/*
		 ! ���񴰿�����Ϣ
		 @ capture		�Ƿ񲶻�
		*/
		Void CMouse::SetCapture( Bool capture )
		{
		}


		/*
		 ! ���������ڵ���Ϣ
		 @ inputEvent	�¼�
		*/
		int32_t CMouse::onLooperInput( AInputEvent* inputEvent )
			{
				int32_t handle = 0;
				size_t count = AMotionEvent_getPointerCount( inputEvent );

				for( size_t i{ 0 }; i < count; ++i )
					{
						// ��������
						this->context.pos.Set(
							(Int16)AMotionEvent_getX( inputEvent, i ),
							(Int16)AMotionEvent_getY( inputEvent, i ) );

						// ת����Ϣ
						switch( AMotionEvent_getAction( inputEvent ) )
							{
								case AMOTION_EVENT_ACTION_UP   : this->context.msg = MSG_L_UP;	 break;
								case AMOTION_EVENT_ACTION_DOWN : this->context.msg = MSG_L_DOWN; break;
								case AMOTION_EVENT_ACTION_MOVE : this->context.msg = MSG_MOVE;	 break;
							}

						// GUI ����
						if( SHARED( gui::CGui ).HandleMouse( this->context ) )
							handle = 1;
					}

				return handle;
			}
	}
}