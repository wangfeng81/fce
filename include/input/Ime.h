/*
**
**	Ime.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../math/Vector.h"
#include "../common/Utility.h"


/**
 !	Ime inner kit
 */
namespace fce::inner::ime
{
	/*	EVENT
	 */
	enum struct EVENT
		:
			Byte
		{
			START,
			END,

			TEXT,
			CHARACTER,

			MAXIMUM
		};

	/*	Message
	 */
	struct FC_API Message final
		{
			/*	EVENT
			*/
			EVENT e;

			/*	String
			*/
			String text;

			/*	Char32
			*/
			Char32 character;

			/*	Source ime
			*/
			Ime& ime;

		public:
			/*	Constructor
			*/
			Message( Ime& ) noexcept;

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
	#include "windows/Win_Ime.h"
#elif defined  FC_ANDROID
	#include "android/Android_Ime.h"
#elif defined  FC_LINUX
	#include "linux/Linux_Ime.h"
#elif defined  FC_IOS
	#include "ios/Ios_Ime.h"
#elif defined  FC_MAC
	#include "mac/Mac_Ime.h"
#endif


namespace fce
{
	/*
	 !	Ime.

			Bool( const Message& )
	 */
	class FC_API Ime final
		:
		/*	Event handler
		 */
		public inner::ime
					::DelegateHandler
		{
			/*	Friends
			*/
			friend Window;

			friend inner::ime::Message;

		public:
			/*	Using inner kit
			*/
			using Impl = inner::ime::Impl;

			using EVENT = inner::ime::EVENT;

			using Message = inner::ime::Message;

			/*	Attached window
			*/
			Window& window;

			/*	Implementation
			*/
			Impl impl;

		public:
			/*
			 @	Set position
			 */
			Void SetPosition( const Point& ) noexcept;

			Void SetPosition( const Int16 x,
							  const Int16 y ) noexcept;

			/*
			 @	Is opened
			 */
			[[nodiscard]] Bool isOpened() const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~Ime() = default;

			Ime( Window& ) noexcept;

			/*	Dispatch
			*/
			Bool Dispatch( const Message& ) noexcept;
		};


	/*
	 @	Ime singleton shortcut
	 */
	extern FC_API Ime& s_Ime;
}



/**
 !	class Ime
 */
namespace fce
{
	/*
	 @	Check whether the ime is opened
	 */
	//FC_INLINE Bool Ime::isOpened() const noexcept
	//	{
	//		return this->opened;
	//	}

	/*
	 @	Set ime position
	 */
	FC_INLINE Void Ime::SetPosition( const Int16 x,
									 const Int16 y ) noexcept
		{
			this->impl.SetPosition( x, y );
		}

	/*
	 @	Set ime position
	 */
	FC_INLINE Void Ime::SetPosition( const Point& pt ) noexcept
		{
			this->impl.SetPosition( pt.x, pt.y );
		}
}