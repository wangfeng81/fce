/*
**
**	CApplication.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "../../../interface/platform/Application.h"


namespace fce
{
	namespace android
	{
		/*
		 !	CApplication.
		 */
		class CApplication : public IApplication
		{
		public:
			/*	App command
			 */
			enum COMMAND
				:
					UInt8
				{
					CMD_START,
					CMD_RESUME,
					CMD_PAUSE,
					CMD_STOP,
					CMD_DESTROY,

					CMD_INIT_WINDOW,
					CMD_TERM_WINDOW,
					CMD_INPUT_CREATED,
					CMD_INPUT_DESTROYED,

					CMD_GAINED_FOCUS,
					CMD_LOST_FOCUS,

					CMD_LOW_MEMORY,
					CMD_SAVE_STATE,

					CMD_CONFIG_CHANGED,
					CMD_WINDOW_RESIZED,
					CMD_WINDOW_REDRAW_NEEDED,
					CMD_CONTENT_RECT_CHANGED,
				};

			/*	Pipe fd type
			 */
			enum
			{
				PF_READ,
				PF_WRITE,

				MAXNUM_PF
			};

			int	fdPipe[ MAXNUM_PF ];

			/*
			 !	android object
			 */
			ANativeActivity*	activity;
			ANativeWindow*		window;
			AInputQueue*		inputQueue;
			ALooper*			looper;
			ARect				contentRect;

			/*
			 !	savedState
			 */
			void*				savedState;
			size_t				savedStateSize;

			/*
			 !	App thread
			 */
			pthread_t			thread;
			sem_t				sem;

			/*
			 !	App state
			 */
			bool				bDestroyRequested;
			bool				bHasFocus;

		public:
			Bool				Create( IAppAgent* appAgent, ANativeActivity* ana,
										void* savedState, size_t savedStateSize );
			Void				Destroy();

			int					Run();

		private:
			/*
			 !	App singleton
			 */
			CApplication();
			virtual ~CApplication();
			SINGLETON( CApplication );

			/*
			 !	Pipe command
			 */
			bool				SendCommand( COMMAND cmd );
			bool				RecvCommand( COMMAND& cmd );

			/*
			 !	App thread entry
			 */
			static void*		AppThreadEntry( void* param );

			/*
			 !	ALooper_callbackFunc
			 */
			static int			OnLooperCmd( int fd, int events, void* data );
			static int			OnLooperInput( int fd, int events, void* data );

			/*
			 !	Native Activity Callbacks
			 */
			void				RegisterANAC( ANativeActivityCallbacks* anac );
			static void			onStart( ANativeActivity* ana );
			static void			onResume( ANativeActivity* ana );
			static void			onPause( ANativeActivity* ana );
			static void			onStop( ANativeActivity* ana );
			static void			onDestroy( ANativeActivity* ana );
			static void			onLowMemory( ANativeActivity* ana );
			static void*		onSaveInstanceState( ANativeActivity* ana, size_t* outLen );
			static void			onConfigurationChanged( ANativeActivity* ana );
			static void			onWindowFocusChanged( ANativeActivity* ana, int focused );
			static void			onNativeWindowCreated( ANativeActivity* ana, ANativeWindow* anw );
			static void			onNativeWindowDestroyed( ANativeActivity* ana, ANativeWindow* anw );
			static void			onInputQueueCreated( ANativeActivity* ana, AInputQueue* aiq );
			static void			onInputQueueDestroyed( ANativeActivity* ana, AInputQueue* aiq );
			static void			onContentRectChanged( ANativeActivity* ana, const ARect* arect );

		public:
			Bool				Quit( DWord code = 0 );
		};
	}



	/*
	 @	fc main
	 */
	#define FC_MAIN	void ::ANativeActivity_onCreate( ANativeActivity*,	\
													 void*,	size_t );	\
																		\
					void Main( ANativeActivity*, void*, size_t )

	/*
	 @	Launch the framework
	 */
	#define FC_ROCK_THE_WORLD( FRAMEWORK )								\
																		\
			FC_INJECT_FRAMEWORK( FRAMEWORK )							\
																		\
			void ANativeActivity_onCreate( ANativeActivity* activity,	\
										   void* savedState,			\
										   size_t savedStateSize )		\
				{														\
					s_App.Main( activity, savedState, savedStateSize );	\
				}
}