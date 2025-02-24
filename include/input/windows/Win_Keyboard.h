/*
**
**	Win_Keyboard.
**
		Windows platform implementation

	Copyright (c) WangFeng since 1999...
*/

namespace fce::inner::win
{
	/*
	 !	Win Keyboard.
	 */
	class FC_API Win_Keyboard final
		{
			/*	Friends
			*/
			friend Keyboard;

			friend Win_Window;

			/*	Using inner kit
			*/
			using KEY = keyboard::KEY;

			using EVENT = keyboard::EVENT;

			using Message = keyboard::Message;

		public:
			/*	Check key status
			*/
			Bool isPressed( const int vk ) const noexcept;

			Bool isEnabled( const int vk ) const noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Win_Keyboard );

			~Win_Keyboard() = default;

			Win_Keyboard( Keyboard& ) noexcept;

			/*	Handle messages
			 */
			KEY ProcessKey( const WPARAM ) noexcept;

			Bool on_WndProc( const UINT,
							 const WPARAM,
							 const LPARAM ) noexcept;

			/*	Convert key code
			 */
			static constexpr KEY Convert( const int vk ) noexcept;

		private:
			/*	Message
			*/
			Message message;

			/*	Key code list
			*/
			static constexpr int KEY_LIST[]
				{
					'0',
					'1',
					'2',
					'3',
					'4',
					'5',
					'6',
					'7',
					'8',
					'9',

					'A',
					'B',
					'C',
					'D',
					'E',
					'F',
					'G',
					'H',
					'I',
					'J',
					'K',
					'L',
					'M',
					'N',
					'O',
					'P',
					'Q',
					'R',
					'S',
					'T',
					'U',
					'V',
					'W',
					'X',
					'Y',
					'Z',

					VK_F1,
					VK_F2,
					VK_F3,
					VK_F4,
					VK_F5,
					VK_F6,
					VK_F7,
					VK_F8,
					VK_F9,
					VK_F10,
					VK_F11,
					VK_F12,

					VK_UP,
					VK_DOWN,
					VK_LEFT,
					VK_RIGHT,

					VK_INSERT,
					VK_DELETE,
					VK_HOME,
					VK_END,
					VK_PRIOR,
					VK_NEXT,

					VK_ESCAPE,
					VK_TAB,
					VK_BACK,
					VK_RETURN,
					VK_SPACE,
					VK_PAUSE,
					VK_SNAPSHOT,

					VK_LMENU,
					VK_RMENU,
					VK_LCONTROL,
					VK_RCONTROL,
					VK_LSHIFT,
					VK_RSHIFT,

					VK_NUMLOCK,
					VK_CAPITAL,
					VK_SCROLL,

					VK_NUMPAD0,
					VK_NUMPAD1,
					VK_NUMPAD2,
					VK_NUMPAD3,
					VK_NUMPAD4,
					VK_NUMPAD5,
					VK_NUMPAD6,
					VK_NUMPAD7,
					VK_NUMPAD8,
					VK_NUMPAD9,
					VK_ADD,
					VK_SUBTRACT,
					VK_MULTIPLY,
					VK_DIVIDE,
					VK_DECIMAL,
					VK_CLEAR,

					VK_OEM_1,
					VK_OEM_2,
					VK_OEM_3,
					VK_OEM_4,
					VK_OEM_5,
					VK_OEM_6,
					VK_OEM_7,
					VK_OEM_8,
					VK_OEM_PLUS,
					VK_OEM_MINUS,
					VK_OEM_COMMA,
					VK_OEM_PERIOD
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::keyboard
{
	/*
	 @	Keyboard win impl
	 */
	using Impl = win::Win_Keyboard;

	/*
	 @	Win_Keyboard singleton shortcut.
	 */
	#define s_Win_Keyboard s_Keyboard.impl
}



/**
 !	class Win_Keyboard
 */
namespace fce::inner::win
{
	/*
	 @	Check whether the key is pressed
	 */
	FC_INLINE Bool Win_Keyboard::isPressed( const int vk ) const noexcept
		{
			return GetKeyState( vk ) < 0;
		}

	/*
	 @	Check whether the key is enabled
	 */
	FC_INLINE Bool Win_Keyboard::isEnabled( const int vk ) const noexcept
		{
			return GetKeyState( vk ) & 1;
		}
}