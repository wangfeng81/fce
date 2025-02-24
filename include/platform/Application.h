/*
**
**	Application.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Framework.h"
#include "Clipboard.h"

#include "../common/Timer.h"
#include "../resource/Asset.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Application.h"
#elif defined  FC_ANDROID
	#include "android/Android_Application.h"
#elif defined  FC_LINUX
	#include "linux/Linux_Application.h"
#elif defined  FC_IOS
	#include "ios/Ios_Application.h"
#elif defined  FC_MAC
	#include "mac/Mac_Application.h"
#endif


namespace fce
{
	/*
	 !	Application
	 */
	class FC_API Application final
		:
		public Entity
		{
			/*	Friends
			*/
			friend Engine;

			friend Timer;
			friend Window;
			friend Assertion;
			friend Framework;
			friend JobThread;
			friend JobManagement;
			friend AssetManagement;
			friend MemoryManagement;
			friend StorageManagement;

			friend inner::application::Impl;

			/*	EVENT
			 */
			enum struct EVENT
				:
					DWord
				{
					USER = inner::application::Impl::USER,

					JOB,
					NET,

					TIMER,
					ASSET,
					STORAGE,

					EXIT
				};

		public:
			/*	Handler
			*/
			using Handler = Void( * )( Object& );

			/*	Impl
			*/
			using Impl = inner::application::Impl;

			/*	Implementation
			*/
			Impl impl;

		public:
			/*	Post
			
					The maximum queue length is
					specified by the platform system
					Avoid thread blocking when its full
			*/
			template< X_object T,
					  typename ...Args >

			Void Post( Args&&... ) noexcept;

			/*	Set handler
			*/
			Void SetHandler( const Handler ) noexcept;

			/*	Request to exit

					Pass in false to trigger
					Framework::on_Quit()	
			*/
			Bool Quit( const Bool now = true ) noexcept;

			/*	Execute platform command line
			*/
			Int Execute( const stl::wstring& cmd,
						 const stl::wstring& args,
							   stl::string& feedback ) noexcept;

			Int Execute( const stl::wstring& cmdLine,
							   stl::string& feedback ) noexcept;

			/*	Screen shot
			*/
			Bool Screenshot( Image&,
							 const Int16 x,
							 const Int16 y,
							 const Int16 width,
							 const Int16 height ) const noexcept;

			Bool Screenshot( Image&,
							 const Point&,
							 const Size& ) const noexcept;

			Bool Screenshot( Image&,
							 const Rect& ) const noexcept;

			Bool Screenshot( Image& ) const noexcept;

			/*	shared framework
			*/
			template< X_framework T = Framework >

			[[nodiscard]] T& sharedFramework() const noexcept;

			/*	Get native handle
			*/
			[[nodiscard]] const auto& GetHandle() const noexcept;

			/*	Direct access framework
			*/
			[[nodiscard]] Framework* operator -> () const noexcept;

			/*	Malloc a block in a map with no construct
			*/
			[[nodiscard]] Handle Malloc( const UInt size,
										 const UInt alignment ) noexcept;

			[[nodiscard]] Handle Realloc( Handle const,
										  const UInt size,
										  const UInt alignment ) noexcept;

			/*	Malloc an array in a map with no construct
			*/
			template< typename T >

			[[nodiscard]] T* Malloc( const UInt n = 1,
									 const UInt alignment = sizeof( Handle ) ) noexcept;

			template< typename T >

			[[nodiscard]] T* Realloc( Handle const,
									  const UInt n = 1,
									  const UInt alignment = sizeof( Handle ) ) noexcept;

			/*	Free a block in a map with no destruct
			*/
			Void Free( Handle const ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			Application() noexcept;

			~Application() = default;

			/*	Response
			*/
			Bool on_Create( const WChar* const ) NOEXCEPT;

			Void on_Destroy() NOEXCEPT;

			Void on_Frame() NOEXCEPT;

			Void on_Exception() noexcept;

			/*	Post
			*/
			Void Post( const EVENT,
					   const Object* const ) noexcept;

			/*	Dispatch
			*/
			Void Dispatch( const EVENT,
						   Object * const ) noexcept;

			Void on_Exit( Object* const ) noexcept;

			Void on_User( Object* const ) NOEXCEPT;

			Void on_Job( Object* const ) NOEXCEPT;

			Void on_Timer( Object* const ) NOEXCEPT;

			Void on_Asset( Object* const ) NOEXCEPT;

			Void on_Storage( Object* const ) NOEXCEPT;

			/*	Prompt closing
			*/
			Void PromptClosing() noexcept;

		private:
			/*	Handler
			*/
			Handler handler;

			/*	Framework
			*/
			Framework* framework;

			/*	Blocks map
			*/
			stl::unordered_map< Handle,
								MemoryPool* > mapBlocks;
		};


	/*
	 @	Application singleton shortcut
	 */
	extern FC_API Application& s_App;
}



/**
 !	class Application
 */
namespace fce
{
	/*
	 @	Inject framework
	 */
	extern "C" Framework * InjectFramework();


	/*
	 @	Get native handle
	 */
	FC_INLINE const auto& Application::GetHandle() const noexcept
		{
			return this->impl.GetHandle();
		}

	/*
	 @	shared framework
	 */
	template< X_framework T >

	FC_INLINE T& Application::sharedFramework() const noexcept
		{
			return *this->framework;
		}

	/*
	 @	Direct access framework
	 */
	FC_INLINE Framework* Application::operator -> () const noexcept
		{
			return this->framework;
		}

	/*
	 @	Execute console command line
	 */
	FC_INLINE Int Application::Execute( const stl::wstring& cmd,
										const stl::wstring& args,
											  stl::string& feedback ) noexcept
		{
			return this->Execute( cmd + L' ' + args,
								  feedback );
		}

	/*
	 @	Set message handler
	 */
	FC_INLINE Void Application::SetHandler( const Handler handler ) noexcept
		{
			this->handler = handler;
		}

	/*
	 @	Post message

			Burn After Reading

			The message will be
			created on sending and

			automatically destroyed
			immediately after handling
	 */
	template< X_object T,
			  typename ...Args >

	FC_INLINE Void Application::Post( Args&& ...args ) noexcept
		{
			T* obj = s_MemMgmt.Generate< T >
						(
							std::forward< Args >( args )...
						);

			if constexpr( std::same_as< Timer::Message, T > )
				{
					this->Post( EVENT::TIMER, obj );
				}
			else
				{
					this->Post( EVENT::USER, obj );
				}
		}
}



/**
 !	Screen shot
 */
namespace fce
{
	/*
	 @	Rectangular area of viewport
	 */
	FC_INLINE Bool Application::Screenshot( Image& img,
											const Int16 x,
											const Int16 y,
											const Int16 width,
											const Int16 height ) const noexcept
		{
			return this->impl.Screenshot( img, x, y,
										  width, height );
		}

	/*
	 @	Rectangular area of viewport
	 */
	FC_INLINE Bool Application::Screenshot( Image& img,
											const Point& pt,
											const Size& size ) const noexcept
		{
			return this->Screenshot( img,
									 pt.x, pt.y,
									 size.x, size.y );
		}

	/*
	 @	Rectangular area of viewport
	 */
	FC_INLINE Bool Application::Screenshot( Image& img,
											const Rect& rect ) const noexcept
		{
			return this->Screenshot( img,
									 rect.min,
									 rect.Size() );
		}
}



/**
 !	Malloc
 */
namespace fce
{
	/*
	 @	Malloc a block in a map
	 */
	template< typename T >

	FC_INLINE T* Application::Malloc( const UInt n,
									  const UInt alignment ) noexcept
		{
			return reinterpret_cast< T* >
					(
						this->Malloc( sizeof( T ) * n,
									  alignment )
					);
		}


	/*
	 @	Realloc a block in a map
	 */
	template< typename T >

	FC_INLINE T* Application::Realloc( Handle const block,
											  const UInt n,
											  const UInt alignment ) noexcept
		{
			return reinterpret_cast< T* >
					(
						this->Realloc( block,
									   sizeof( T ) * n,
									   alignment )
					);
		}
}