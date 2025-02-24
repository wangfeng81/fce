/*
**
**	Mouse.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "../common/Utility.h"


/**
 !	Mouse inner kit
 */
namespace fce::inner::mouse
{
	/*	Key
	 */
	enum struct KEY
		:
			Byte
		{
			L_BTN,
			R_BTN,
			M_BTN,

			X1_BTN,
			X2_BTN,

			MAXIMUM
		};

	/*	EVENT
	 */
	enum struct EVENT
		:
			Byte
		{
			MOVE,
			WHEEL,

			L_BTN_UP,
			L_BTN_DOWN,
			L_BTN_DBLCLK,

			R_BTN_UP,
			R_BTN_DOWN,
			R_BTN_DBLCLK,

			M_BTN_UP,
			M_BTN_DOWN,
			M_BTN_DBLCLK,

			X1_BTN_UP,
			X1_BTN_DOWN,
			X1_BTN_DBLCLK,

			X2_BTN_UP,
			X2_BTN_DOWN,
			X2_BTN_DBLCLK,

			MAXIMUM
		};

	/*	Message
	 */
	struct FC_API Message final
		{
			/*	EVENT
			*/
			EVENT e;

			/*	Pos
			*/
			Point position;

			/*	Wheel delta
			*/
			Int32 wheelDelta;

			/*	Source mouse
			*/
			Mouse& mouse;

		public:
			/*	Constructor
			*/
			Message( Mouse& ) noexcept;

			/*	Dispatch
			*/
			Bool Dispatch() const noexcept;
		};

	/*	Delegate handler
	 */
	using DelegateHandler = FilterGroupWrapper
							<
								UInt8( EVENT::MAXIMUM ),
								EVENT, const Message&
							>;
}


/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Mouse.h"
#elif defined  FC_ANDROID
	#include "android/Android_Mouse.h"
#elif defined  FC_LINUX
	#include "linux/Linux_Mouse.h"
#elif defined  FC_IOS
	#include "ios/Ios_Mouse.h"
#elif defined  FC_MAC
	#include "mac/Mac_Mouse.h"
#endif


namespace fce
{
	/*
	 !	Mouse.
		
			Bool( const Message& )
	 */
	class FC_API Mouse final
		:
		/*	Client area handler
		 */
		public inner::mouse
					::DelegateHandler
		{
			/*	Friends
			*/
			friend Window;

			friend inner::mouse::Message;

		public:
			/*	Using inner kit
			*/
			using Impl = inner::mouse::Impl;

			using KEY = inner::mouse::KEY;

			using EVENT = inner::mouse::EVENT;

			using Message = inner::mouse::Message;

			/*	Attached window
			*/
			Window& window;

			/*	Implementation
			*/
			Impl impl;

		public:
			/*	Capture messages
			*/
			Void Capture() noexcept;

			Void Uncapture() noexcept;

			/*	Show | Hide  cursor
			*/
			Void ShowCursor() noexcept;

			Void HideCursor() noexcept;

			/*	Get | Set  position
			*/
			Void SetPosition( const Point& ) noexcept;

			Void SetPosition( const Int16 x,
							  const Int16 y ) noexcept;

			[[nodiscard]] Point GetPosition() const noexcept;

			/*	Check button status
			*/
			[[nodiscard]] Bool isPressed( const KEY ) const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~Mouse() = default;

			Mouse( Window& ) noexcept;

			/*	Dispatch
			*/
			Bool Dispatch( const Message& ) noexcept;

			/*	Convert key
			*/
			CONVERT_IMPL_ENUM_VALUE( KEY );
		};


	/*
	 @	Mouse singleton shortcut
	 */
	extern FC_API Mouse& s_Mouse;
}



/**
 !	class Mouse
 */
namespace fce
{
	/*
	 @	Capture messages outside the window
	 */
	FC_INLINE Void Mouse::Capture() noexcept
		{
			this->impl.Capture();
		}

	/*
	 @	Cancel the message capture outside the window
	 */
	FC_INLINE Void Mouse::Uncapture() noexcept
		{
			this->impl.Uncapture();
		}

	/*
	 @	Show the cursor
	 */
	FC_INLINE Void Mouse::ShowCursor() noexcept
		{
			this->impl.ShowCursor();
		}

	/*
	 @	Hide the cursor
	 */
	FC_INLINE Void Mouse::HideCursor() noexcept
		{
			this->impl.HideCursor();
		}

	/*
	 @	Set cursor position
	 */
	FC_INLINE Void Mouse::SetPosition( const Int16 x,
									   const Int16 y ) noexcept
		{
			this->SetPosition( { x, y } );
		}

	/*
	 @	Check whether the button is pressed
	 */
	FC_INLINE Bool Mouse::isPressed( const KEY key ) const noexcept
		{
			return this->impl.isPressed( Convert( key ) );
		}
}