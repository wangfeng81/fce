/*
**
**	CApplication.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#include "Application.h"
#include "Window.h"

#include "input/Ime.h"
#include "input/Mouse.h"
#include "input/Keyboard.h"

#include "../../gui/Gui.h"


//fc::CApplication* mCApplication=&fc::SHARED( fc::CApplication );
//
//
//void displayKeyboard(bool pShow)
//{
//    // Attaches the current thread to the JVM.
//    jint lResult;
//    jint lFlags = 0;
//
//    JavaVM* lJavaVM = mCApplication->activity->vm;
//    JNIEnv* lJNIEnv = mCApplication->activity->env;
//
//    JavaVMAttachArgs lJavaVMAttachArgs;
//    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
//    lJavaVMAttachArgs.name = "NativeThread";
//    lJavaVMAttachArgs.group = nullptr;
//
//    lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
//    if (lResult == JNI_ERR) {
//        return;
//    }
//
//    // Retrieves NativeActivity.
//    jobject lNativeActivity = mCApplication->activity->clazz;
//    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);
//
//    // Retrieves Context.INPUT_METHOD_SERVICE.
//    jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
//    jfieldID FieldINPUT_METHOD_SERVICE =
//        lJNIEnv->GetStaticFieldID(ClassContext,
//            "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
//    jobject INPUT_METHOD_SERVICE =
//        lJNIEnv->GetStaticObjectField(ClassContext,
//            FieldINPUT_METHOD_SERVICE);
//    //jniCheck(INPUT_METHOD_SERVICE);
//
//    // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
//    jclass ClassInputMethodManager = lJNIEnv->FindClass(
//        "android/view/inputmethod/InputMethodManager");
//    jmethodID MethodGetSystemService = lJNIEnv->GetMethodID(
//        ClassNativeActivity, "getSystemService",
//        "(Ljava/lang/String;)Ljava/lang/Object;");
//    jobject lInputMethodManager = lJNIEnv->CallObjectMethod(
//        lNativeActivity, MethodGetSystemService,
//        INPUT_METHOD_SERVICE);
//
//    // Runs getWindow().getDecorView().
//    jmethodID MethodGetWindow = lJNIEnv->GetMethodID(
//        ClassNativeActivity, "getWindow",
//        "()Landroid/view/Window;");
//    jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity,
//        MethodGetWindow);
//    jclass ClassWindow = lJNIEnv->FindClass(
//        "android/view/Window");
//    jmethodID MethodGetDecorView = lJNIEnv->GetMethodID(
//        ClassWindow, "getDecorView", "()Landroid/view/View;");
//    jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow,
//        MethodGetDecorView);
//
//    if (pShow) {
//        // Runs lInputMethodManager.showSoftInput(...).
//        jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID(
//            ClassInputMethodManager, "showSoftInput",
//            "(Landroid/view/View;I)Z");
//        jboolean lResult = lJNIEnv->CallBooleanMethod(
//            lInputMethodManager, MethodShowSoftInput,
//            lDecorView, lFlags);
//    } else {
//        // Runs lWindow.getViewToken()
//        jclass ClassView = lJNIEnv->FindClass(
//            "android/view/View");
//        jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID(
//            ClassView, "getWindowToken", "()Landroid/os/IBinder;");
//        jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView,
//            MethodGetWindowToken);
//
//        // lInputMethodManager.hideSoftInput(...).
//        jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID(
//            ClassInputMethodManager, "hideSoftInputFromWindow",
//            "(Landroid/os/IBinder;I)Z");
//        jboolean lRes = lJNIEnv->CallBooleanMethod(
//            lInputMethodManager, MethodHideSoftInput,
//            lBinder, lFlags);
//    }
//
//    // Finished with the JVM.
//    lJavaVM->DetachCurrentThread();
//}
namespace fce
{
	/*
	 !	Application instance.
	 */
	IApplication& Application = SHARED( android::CApplication );


	namespace android
	{
		/*
		!	����.
		*/
		CApplication::CApplication()
		{
			this->window = 0;
			this->bHasFocus = false;

			this->savedState = nullptr;
			this->savedStateSize = 0;
		}

		/*
		!	����.
		*/
		CApplication::~CApplication()
		{
		}


		/*
		 !	ע�ᴰ���࣬���Ҵ���������
		 @ hInst	����ʵ�����
		*/
		Bool CApplication::Create( IAppAgent* appAgent, ANativeActivity* ana,
							void* savedState, size_t savedStateSize )
		{
			FC_LOG("internalDataPath : %s", ana->internalDataPath);
			FC_LOG("externalDataPath : %s", ana->externalDataPath);

			this->appAgent = appAgent;

			//
			this->activity = ana;
			this->activity->instance = this;

			// ע�� android activity �ص�����
			this->RegisterANAC( this->activity->callbacks );

			// �洢״̬
			if( savedState && savedStateSize )
			{
				this->savedState = malloc( savedStateSize );
				this->savedStateSize = savedStateSize;
				memcpy( this->savedState, savedState, savedStateSize );
			}

			// ���� app thread ͨ�Źܵ�
			if( pipe( this->fdPipe ) )
				return False;

			// ����app thread ��������
			this->bDestroyRequested = false;

			// ��ʼ�� app thread ͬ���ź�
			if( sem_init( &this->sem, 0, 0 ) )
				return False;

			// ���� app thread 
			if( pthread_create(
					&this->thread, nullptr,
					AppThreadEntry, this ) )
				return False;

			return True;
		}


		/*
		 ! ����
		*/
		void CApplication::Destroy()
		{
			// ���� IAppAgent::onDestroy()
			this->appAgent->OnDestroy();
		}


		/*
		 ! ע�� ϵͳ�¼� �ص�����
		*/
		void CApplication::RegisterANAC( ANativeActivityCallbacks* anac )
		{
			anac->onDestroy					= onDestroy;
			anac->onStart					= onStart;
			anac->onResume					= onResume;
			anac->onPause					= onPause;
			anac->onStop					= onStop;
			anac->onLowMemory				= onLowMemory;
			anac->onSaveInstanceState		= onSaveInstanceState;
			anac->onConfigurationChanged	= onConfigurationChanged;
			anac->onWindowFocusChanged		= onWindowFocusChanged;
			anac->onNativeWindowCreated		= onNativeWindowCreated;
			anac->onNativeWindowDestroyed	= onNativeWindowDestroyed;
			anac->onInputQueueCreated		= onInputQueueCreated;
			anac->onInputQueueDestroyed		= onInputQueueDestroyed;
			anac->onContentRectChanged		= onContentRectChanged;
		}


		/*
		 ! �߳����
		*/
		void* CApplication::AppThreadEntry( void* param )
		{
			CApplication* app = (CApplication*)param;

			// ���� LOOPER
			app->looper = ALooper_prepare( 0 );

			// �� Pipe fd read �� LOOER��
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			// ���� �滻         �鿴API ����
			if( -1 == ALooper_addFd( app->looper,
									app->fdPipe[ PF_READ ],
									0, ALOOPER_EVENT_INPUT,
									app->OnLooperCmd, app ) )
				return param;

			// ����
			app->Run();

			// ����
			app->Destroy();

			return param;
		}


		/*
		 ! ��ʼ��Ϣѭ��
		*/
		int CApplication::Run()
		{
			// loop waiting for stuff to do.
			while( ALOOPER_POLL_ERROR !=
						ALooper_pollAll(
							0,//app->bHasFocus ? 0 : -1,
							nullptr, nullptr, nullptr ) )
			{
				if( this->bDestroyRequested )
					break;

				if( this->bHasFocus )
				{
					// ���� IAppAgent::onFrame()
					this->appAgent->OnFrame();

					SHARED( gui::CGui ).Render();
					SHARED( CWindow ).Present();

					usleep( 10 );
				}
			}

			return (int)0;
		}


		/*
		 ! OnLooperCmd
		*/
		int CApplication::onLooperCmd( int fd, int events, void* data )
		{
			CApplication* app = (CApplication*)data;

			COMMAND cmd;

			if( app->RecvCommand( cmd ) )
			{
				switch( cmd )
				{
					case CMD_START:
						{
							sem_post( &app->sem );
							FC_LOG("CMD_START");
						}
						break;

					case CMD_RESUME:
						{
							sem_post( &app->sem );
							FC_LOG("CMD_RESUME");
						}
						break;

					case CMD_PAUSE:
						{
							app->bHasFocus = false;
							sem_post( &app->sem );
							FC_LOG("CMD_PAUSE");
						}
						break;

					case CMD_STOP:
						{
							sem_post( &app->sem );
							FC_LOG("CMD_STOP");
						}
						break;

					case CMD_INPUT_CREATED:
						{
							// �� ��Ϣ����
							if( app->inputQueue )
							{
								AInputQueue_attachLooper(
									app->inputQueue, app->looper,
									0, app->OnLooperInput, app );
							}
							sem_post( &app->sem );
							FC_LOG("CMD_INPUT_CREATED");
						}
						break;

					case CMD_INPUT_DESTROYED:
						{
							// ���� ��Ϣ����
							if( app->inputQueue )
							{
								AInputQueue_detachLooper( app->inputQueue );
								app->inputQueue = nullptr;
							}
							sem_post( &app->sem );
							FC_LOG("CMD_INPUT_DESTROYED");
						}
						break;

					case CMD_INIT_WINDOW:
						{
							// ���� egl
							SHARED( CWindow ).Create( app->window );
							sem_post( &app->sem );

							// ���� IAppAgent::onCreate()
							if( ! app->appAgent->OnCreate() )
								return 0;

							FC_LOG("CMD_INIT_WINDOW");
						}
						break;

					case CMD_TERM_WINDOW:
						{
							SHARED( CWindow ).Destroy();
							app->window = nullptr;
							sem_post( &app->sem );
							FC_LOG("CMD_TERM_WINDOW");
						}
						break;

					case CMD_DESTROY:
						{
							app->bDestroyRequested = true;
							FC_LOG("CMD_DESTROY");
						}
						break;

					case CMD_GAINED_FOCUS:
						{
							app->bHasFocus = true;
							FC_LOG("CMD_GAINED_FOCUS");
						}
						break;

					case CMD_LOST_FOCUS:
						{
							app->bHasFocus = false;
							FC_LOG("CMD_LOST_FOCUS");
						}
						break;
				}
			}

			return 1;
		}


		/*
		 ! OnLooperInput
		*/
		int CApplication::onLooperInput( int fd, int events, void* data )
		{
			CApplication* app = (CApplication*)data;

			// ��ȡ���� inputEvent
			AInputEvent* inputEvent = nullptr;

			if( AInputQueue_getEvent( app->inputQueue, &inputEvent ) >= 0 )
			{
				// ����Ƿ��� Ԥ�����¼�
				if( ! AInputQueue_preDispatchEvent( app->inputQueue, inputEvent ) )
				{
					// �Ƿ���
					int32_t handled = 0;

					// �¼�����
					switch( AInputEvent_getType( inputEvent ) )
					{
						// ��������
						case AINPUT_EVENT_TYPE_KEY:
							break;

						// ��������
						case AINPUT_EVENT_TYPE_MOTION:
							{
								// �¼�Դ
								switch( AInputEvent_getSource( inputEvent ) )
								{
									case AINPUT_SOURCE_MOUSE:
									case AINPUT_SOURCE_TRACKBALL:
									case AINPUT_SOURCE_TOUCHSCREEN:
										{
											// �������
											handled = SHARED( CMouse ).OnLooperInput( inputEvent );
										}
										break;
								}
							}
							break;
					}

					// ����ϵͳ
					AInputQueue_finishEvent( app->inputQueue, inputEvent, handled );
				}
			}

			return 1;
		}


		/*
		 ! SendCommand
		*/
		bool CApplication::SendCommand( COMMAND cmd )
		{
			return write( this->fdPipe[ PF_WRITE ],
						&cmd, sizeof(cmd) ) == sizeof(cmd);
		}


		/*
		 ! RecvCommand
		*/
		bool CApplication::RecvCommand( COMMAND& cmd )
		{
			return read( this->fdPipe[ PF_READ ],
						&cmd, sizeof(cmd) ) == sizeof(cmd);
		}



		/*
		 ! onStart
		*/
		void CApplication::onStart( ANativeActivity* ana )
		{
			CApplication* app = (CApplication*)ana->instance;

			app->SendCommand( CMD_START );
			sem_wait( &app->sem );
		}

		/*
		 ! onResume
		*/
		void CApplication::onResume( ANativeActivity* ana )
		{
			CApplication* app = (CApplication*)ana->instance;

			app->SendCommand( CMD_RESUME );
			sem_wait( &app->sem );
		}

		/*
		 ! onPause
		*/
		void CApplication::onPause( ANativeActivity* ana )
		{
			CApplication* app = (CApplication*)ana->instance;

			app->SendCommand( CMD_PAUSE );
			sem_wait( &app->sem );
		}

		/*
		 ! onStop
		*/
		void CApplication::onStop( ANativeActivity* ana )
		{
			CApplication* app = (CApplication*)ana->instance;

			app->SendCommand( CMD_STOP );
			sem_wait( &app->sem );

			// ǿ�йر�
			ANativeActivity_finish( app->activity );
		}

		/*
		 ! onDestroy
		*/
		void CApplication::onDestroy( ANativeActivity* ana )
		{
			CApplication* app = (CApplication*)ana->instance;

			// ������������
			app->SendCommand( CMD_DESTROY );

			// �ȴ� App thread ����
			pthread_join( app->thread, nullptr );

			// ���� App thread ͨ�Źܵ�
			for( int i{ 0 }; i < MAXNUM_PF; ++i )
				close( app->fdPipe[ i ] );

			// ���� App thread ͬ���ź�
			sem_destroy( &app->sem );

			// ǿ��ֹͣ
			exit( 0 );
		}

		/*
		 ! onLowMemory
		*/
		void CApplication::onLowMemory( ANativeActivity* ana )
		{
			CApplication* app = (CApplication*)ana->instance;
			app->SendCommand( CMD_LOW_MEMORY );
		}

		/*
		 ! onWindowFocusChanged
		*/
		void CApplication::onWindowFocusChanged( ANativeActivity* ana, int focused )
		{
			CApplication* app = (CApplication*)ana->instance;
			app->SendCommand( focused ?
					CMD_GAINED_FOCUS : CMD_LOST_FOCUS );
		}

		/*
		 ! onConfigurationChanged
		*/
		void CApplication::onConfigurationChanged( ANativeActivity* ana )
		{
			CApplication* app = (CApplication*)ana->instance;
			app->SendCommand( CMD_CONFIG_CHANGED );
		}

		/*
		 ! onSaveInstanceState
		*/
		void* CApplication::onSaveInstanceState( ANativeActivity* ana, size_t* outLen )
		{
			CApplication* app = (CApplication*)ana->instance;
			*outLen = app->savedStateSize;

			app->SendCommand( CMD_SAVE_STATE );

			return app->savedState;
		}

		/*
		 ! onNativeWindowCreated
		*/
		void CApplication::onNativeWindowCreated( ANativeActivity* ana, ANativeWindow* anw )
		{
			CApplication* app = (CApplication*)ana->instance;

			app->window = anw;
			app->SendCommand( CMD_INIT_WINDOW );
			sem_wait( &app->sem );
		}

		/*
		 ! onNativeWindowDestroyed
		*/
		void CApplication::onNativeWindowDestroyed( ANativeActivity* ana, ANativeWindow* anw )
		{
			CApplication* app = (CApplication*)ana->instance;

			app->SendCommand( CMD_TERM_WINDOW );
			sem_wait( &app->sem );
		}

		/*
		 ! onInputQueueCreated
		*/
		void CApplication::onInputQueueCreated( ANativeActivity* ana, AInputQueue* aiq )
		{
			CApplication* app = (CApplication*)ana->instance;

			app->inputQueue = aiq;
			app->SendCommand( CMD_INPUT_CREATED );
			sem_wait( &app->sem );
		}

		/*
		 ! onInputQueueDestroyed
		*/
		void CApplication::onInputQueueDestroyed( ANativeActivity* ana, AInputQueue* aiq )
		{
			CApplication* app = (CApplication*)ana->instance;

			app->SendCommand( CMD_INPUT_DESTROYED );
			sem_wait( &app->sem );
		}

		/*
		 ! onContentRectChanged
		*/
		void CApplication::onContentRectChanged( ANativeActivity* ana, const ARect* arect )
		{
			//FC_LOG("rect:%d,%d,%d,%d", arect->left,arect->right,arect->top,arect->bottom);
			//if( arect )
			//	SHARED( CWindow ).OnResize(
			//		CWindow::Size( arect->right - arect->left,
			//						arect->bottom - arect->top ) );
		}


		/*
		 ! �˳�
		*/
		Bool CApplication::Quit( DWord code )
		{
			ANativeActivity_finish( this->activity );
			return True;
		}
	}


	/*
	 ! Android native activity �������
	 @ activity			ԭ��activity
	 @ savedState		״̬����
	 @ savedStateSize	״̬�����С
	 */
	extern "C" FC_API
	void fcMain( IAppAgent* appAgent,
				ANativeActivity* anActivity,
				void* savedState,
				size_t savedStateSize)
	{
		fc::SHARED( fc::android::CApplication ).Create(
			appAgent, anActivity, savedState, savedStateSize );
	}
}