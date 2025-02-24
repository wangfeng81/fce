/*
**
**	Keyboard.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "../common/Utility.h"


/**
 !	Keyboard inner kit
 */
namespace fce::inner::keyboard
{
	/*	Key code
	 */
	enum struct KEY
		:
			Byte
		{
			_0,
			_1,
			_2,
			_3,
			_4,
			_5,
			_6,
			_7,
			_8,
			_9,

			_A,
			_B,
			_C,
			_D,
			_E,
			_F,
			_G,
			_H,
			_I,
			_J,
			_K,
			_L,
			_M,
			_N,
			_O,
			_P,
			_Q,
			_R,
			_S,
			_T,
			_U,
			_V,
			_W,
			_X,
			_Y,
			_Z,

			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,

			UP,
			DOWN,
			LEFT,
			RIGHT,

			INS,
			DEL,
			HOME,
			END,
			PRIOR,
			NEXT,

			ESC,
			TAB,
			BACK,
			ENTER,
			SPACE,
			PAUSE,
			SNAPSHOT,

			ALT_L,
			ALT_R,
			CTRL_L,
			CTRL_R,
			SHIFT_L,
			SHIFT_R,

			NUM_LOCK,
			CAPS_LOCK,
			SCROLL_LOCK,

			NUM_PAD_0,
			NUM_PAD_1,
			NUM_PAD_2,
			NUM_PAD_3,
			NUM_PAD_4,
			NUM_PAD_5,
			NUM_PAD_6,
			NUM_PAD_7,
			NUM_PAD_8,
			NUM_PAD_9,
			NUM_PAD_ADD,
			NUM_PAD_SUB,
			NUM_PAD_MUL,
			NUM_PAD_DIV,
			NUM_PAD_DEC,
			NUM_PAD_CLEAR,

			OEM_1,
			OEM_2,
			OEM_3,
			OEM_4,
			OEM_5,
			OEM_6,
			OEM_7,
			OEM_8,
			OEM_PLUS,
			OEM_MINUS,
			OEM_COMMA,
			OEM_PERIOD,

			MAXIMUM
		};

	/*	EVENT
	 */
	enum struct EVENT
		:
			Byte
		{
			KEY_UP,
			KEY_DOWN,

			MAXIMUM
		};

	/*	Message
	 */
	struct FC_API Message final
		{
			/*	EVENT
			*/
			EVENT e;

			/*	KEY
			*/
			KEY	key;

			/*	Source keyboard
			*/
			Keyboard& keyboard;

		public:
			/*	Constructor
			*/
			Message( Keyboard& ) noexcept;

			/*	Dispatch
			*/
			Bool Dispatch() const noexcept;
		};

	/*	Delegate handler
	 */
	using DelegateHandler = FilterGroupWrapper< UInt8( EVENT::MAXIMUM ),
												EVENT, const Message& >;
}


/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Keyboard.h"
#elif defined  FC_ANDROID
	#include "android/Android_Keyboard.h"
#elif defined  FC_LINUX
	#include "linux/Linux_Keyboard.h"
#elif defined  FC_IOS
	#include "ios/Ios_Keyboard.h"
#elif defined  FC_MAC
	#include "mac/Mac_Keyboard.h"
#endif


namespace fce
{
	/*
	 !	Keyboard.

			Bool( const Message& )
	 */
	class Keyboard final
		:
		/*	Event handler
		 */
		public inner::keyboard
					::DelegateHandler
		{
			/*	Friends
			*/
			friend Window;

			friend inner::keyboard::Message;

		public:
			/*	Using inner kit
			*/
			using Impl = inner::keyboard::Impl;

			using KEY = inner::keyboard::KEY;

			using EVENT = inner::keyboard::EVENT;

			using Message = inner::keyboard::Message;

			/*	Attached window
			*/
			Window& window;

			/*	Implementation
			*/
			Impl impl;

		public:
			/*	Check key status
			*/
			[[nodiscard]] Bool isPressed( const KEY ) const noexcept;

			[[nodiscard]] Bool isEnabled( const KEY ) const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~Keyboard() = default;

			Keyboard( Window& ) noexcept;

			/*	Dispatch
			*/
			Bool Dispatch( const Message& ) noexcept;

			/*	Convert key
			*/
			CONVERT_IMPL_ENUM_VALUE( KEY );
		};


	/*
	 @	Keyboard singleton shortcut
	 */
	extern FC_API Keyboard& s_Keyboard;
}



/**
 !	class Keyboard
 */
namespace fce
{
	/*
	 @	Check whether the key is pressed
	 */
	FC_INLINE Bool Keyboard::isPressed( const KEY key ) const noexcept
		{
			return this->impl.isPressed
						(
							Convert( key )
						);
		}

	/*
	 @	Check whether the key is enabled
	 */
	FC_INLINE Bool Keyboard::isEnabled( const KEY key ) const noexcept
		{
			return this->impl.isEnabled
						(
							Convert( key )
						);
		}
}