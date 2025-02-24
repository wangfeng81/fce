/*
**
**	Application.
**
		on_Frame
		on_Create
		on_Destroy �� impl ��������

		�ر�ʱ ͨ������ EXIT �¼�֪ͨ����

		��Ҫ�� on_Create �������

	Copyright (c) WangFeng since 2006...
*/

#include "../../include/platform/Application.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Application.cxx"
#elif defined  FC_ANDROID
	#include "android/Android_Application.cxx"
#elif defined  FC_LINUX
	#include "linux/Linux_Application.cxx"
#elif defined  FC_IOS
	#include "ios/Ios_Application.cxx"
#elif defined  FC_MAC
	#include "mac/Mac_Application.cxx"
#endif


namespace fce
{
	/*
	 @	����
	 */
	Application::Application() noexcept
		:
			handler( nullptr ),
			framework( nullptr )
		{
		}


	/*
	 @	Ͷ����Ϣ�����߳�
	 */
	Void Application::Post( const EVENT e,
							const Object* const obj ) noexcept
		{
			// ѭ��ȷ���ɹ� ( ������������� �ᵼ��ʧ�� )
			while( ! this->impl.Post( DWord( e ), obj ) )
				{
					std::this_thread::sleep_for( 1s );
				}
		}


	/*
	 @	���� �˳�
	 */
	Bool Application::Quit( const Bool now ) noexcept
		{
			/*	�ȼ�� now �Ƿ������˳�
				������ framework->onQuit 
			*/
			if( now || ( this->framework
					  && this->framework->on_Quit() ) )
				{
					// ���� �ر���ʾ
					this->PromptClosing();

					// ���� �ر���Ϣ
					this->Post( EVENT::EXIT,
								nullptr );

					return true;
				}

			return false;
		}


	/*
	 @	��ȡ ������Ļ
	 */
	Bool Application::Screenshot( Image& img ) const noexcept
		{
			return this->impl.Screenshot( img, 0, 0,
										  this->impl.GetScreenWidth(),
										  this->impl.GetScreenHeight() );
		}


	/*
	 @	ִ�� ϵͳ����
	 */
	Int Application::Execute( const stl::wstring& cmdLine,
									stl::string& feedback ) noexcept
		{
			#ifdef FC_WINDOWS
				{
					return this->impl.Execute( cmdLine, feedback );
				}
			#else
				{
					// ת�� ansi
					stl::string strCmdLine =
						std::filesystem::path( cmdLine ).string();

					// �򿪹ܵ� ��ִ������ 
					if( FILE* pipe = popen( strCmdLine.c_str(), "r" ) )
						{
							// ѭ����ȡ �ܵ���Ϣ
							char buf[ 1024 ];

							while( ! feof( pipe ) )
								{
									if( fgets( buf, sizeof( buf ), pipe ) )
										{
											feedback += buf;
										}
								}

							// �رղ����� code
							return pclose( pipe );
						}

					// ʧ�ܷ��� -1
					return -1;
				}
			#endif
		}


	/*
	 @	���� �ر���ʾ
	 */
	FC_INLINE Void Application::PromptClosing() noexcept
		{
			// ���� tilte
			s_Window.SetTitle( L"Closing... %s",
							   s_Window.GetTitle() );

			// ���ش��ڣ������û����룬�Լ���ʱ��ȴ�
			s_Window.SetMode( Window::MODE::MINIMIZE );
		}
}



/**
 !	��Ӧ �¼�
 */
namespace fce
{
	/*
	 @	On create
	 */
	Bool Application::on_Create( const WChar* const cmdLine ) NOEXCEPT
		{
			// ���� Graphics
			if( s_Graphics.Create() )
				{
					// ���� Window
					if( s_Window.Create() )
						{
							// ��󴴽� framework
							if( this->framework = InjectFramework() )
								{
									// ��Ӧ Framework::onCreate
									return this->framework->on_Create( cmdLine );
								}
						}
				}

			// ���� ����ʧ��
			s_Window.PromptBox( "\n App start failed..." );

			return false;
		}


	/*
	 @	On destroy
	 */
	Void Application::on_Destroy() NOEXCEPT
		{
			// ���� Job �߳�
			s_JobMgmt.Finish();

			// ���� framework
			if( this->framework )
				{
					this->framework->on_Destroy();

					this->framework = nullptr;
				}

			// ��� �������
			this->ClearComponents();

			// ��� assets
			s_AssetMgmt.Clear();

			// ��� messages
			this->impl.Finish();

			// ���� Window
			s_Window.Destroy();

			// ���� Graphics
			s_Graphics.Destroy();
		}


	/*
	 @	On frame update
	 */
	Void Application::on_Frame() NOEXCEPT
		{
			// on frame
			this->framework->on_Frame();

			// ��Ⱦ����
			s_Window.Render();
		}


	/*
	 @	On exception

			���� run ��ҵʱ�� �쳣

			���� create & destroy ���쳣����ֱ�� �˳�
	 */
	Void Application::on_Exception() noexcept
		{
			// ֻ����һ�� �쳣
			static Bool once = true;

			if( once )
				{
					// ��ȡ ��ǰ�쳣
					std::exception_ptr eptr =
						std::current_exception();

					// ����Ӧ Framework::on_Exception
					if( this->framework->on_Exception( eptr ) )
						{
							// �˳�
							once = false;

							this->Post( EVENT::EXIT,
										nullptr );
						}
				}
		}
}



/**
 !	�շ� ��Ϣ
 */
namespace fce
{
	/*
	 @	 �ַ���Ϣ

			impl ( this )
	 */
	FC_INLINE Void Application::Dispatch( const EVENT e,
										  Object * const obj ) noexcept
		{
			// �ַ���Ϣ
			switch( e )
				{
					case EVENT::USER:
						{
							this->on_User( obj );
						}
						break;

					case EVENT::JOB:
						{
							this->on_Job( obj );
						}
						break;

					case EVENT::NET:
						{
						}
						break;

					case EVENT::TIMER:
						{
							this->on_Timer( obj );
						}
						break;

					case EVENT::ASSET:
						{
							this->on_Asset( obj );
						}
						break;

					case EVENT::STORAGE:
						{
							this->on_Storage( obj );
						}
						break;

					// �����˳� ֱ�ӷ���
					case EVENT::EXIT:
						{
							this->on_Exit( obj );
						}
						return;
				}

			// ���� Message
			s_MemMgmt.Destroy( obj );
		}


	/*
	 @	 ��Ӧ user ��Ϣ
	 */
	FC_INLINE Void Application::on_User( Object* const obj ) NOEXCEPT
		{
			// ���� �û��������
			FC_ASSERT_LOG( this->handler,
						   L"Message handler must be set before posting" );

			this->handler( *obj );
		}


	/*
	 @	 ��Ӧ job ��Ϣ
	 */
	FC_INLINE Void Application::on_Job( Object* const obj ) NOEXCEPT
		{
			// ���� job��ɺ���
			Job * const job =
				static_cast< Job * const >( obj );

			job->onCompleted();
		}


	/*
	 @	��Ӧ Timer ��Ϣ
	 */
	FC_INLINE Void Application::on_Timer( Object* const obj ) NOEXCEPT
		{
			// ���� ��ʱ������
			Timer::Message * const msg =
				static_cast< Timer::Message * const >( obj );

			msg->timer->NotifyHandlers();
		}


	/*
	 @	 ��Ӧ Asset ��Ϣ
	 */
	FC_INLINE Void Application::on_Asset( Object* const obj ) NOEXCEPT
		{
			// ���� Asset ��ɺ���
			Asset::Msg_Async* const msg =
				static_cast< Asset::Msg_Async* const >( obj );

			msg->callback( *msg );
		}


	/*
	 @	 ��Ӧ Storage ��Ϣ
	 */
	FC_INLINE Void Application::on_Storage( Object* const obj ) NOEXCEPT
		{
			// ���� Storage �ص�����
			Storage::Msg_Async* const msg =
				static_cast< Storage::Msg_Async* const >( obj );

			msg->callback( *msg );
		}


	/*
	 @	��Ӧ EXIT ��Ϣ
	 */
	FC_INLINE Void Application::on_Exit( Object* const obj ) noexcept
		{
			this->impl.Destroy();
		}
}



/**
 !	�ڴ��

		���� map �洢����
		�ͷ�ʱ ����Ҫ size

		������Ҫ���� ��׳��
		���� 0�����䣬�ͷſ�ָ��
 */
namespace fce
{
	/*
	 @	�ͷ�
	 */
	Void Application::Free( Handle const block ) noexcept
		{
			// ���� map
			auto p{ this->mapBlocks.find( block ) };

			if( p != this->mapBlocks.end() )
				{
					// �ͷ� �ڴ�
					p->second->Free( block );

					// �Ƴ� map
					this->mapBlocks.erase( p );
				}
		}


	/*
	 @	����
	 */
	Handle Application::Malloc( const UInt size,
								const UInt alignment ) noexcept
		{
			// 0 �ߴ緵�� nullptr
			if( size )
				{
					// ��ȡ MemoryPool
					MemoryPool* pool
						{
							s_MemMgmt.GetPool( std::max( size, alignment ) )
						};

					// ���� �ڴ�
					Handle block{ pool->Malloc() };

					// ���� map
					this->mapBlocks.emplace( block, pool );

					return block;
				}

			return nullptr;
		}


	/*
	 @	���·���
	 */
	Handle Application::Realloc( Handle const oldBlock,
										const UInt newSize,
										const UInt alignment ) noexcept
		{
			// ��� oldBlock
			if( oldBlock )
				{
					// ��� size
					if( newSize )
						{
							// ���� ������ MemoryPool
							auto p{ this->mapBlocks.find( oldBlock ) };

							if( p != this->mapBlocks.end() )
								{
									MemoryPool* const oldPool{ p->second };

									const UInt oldSize{ oldPool->GetCellSize() };

									// ��� size �� alignment ��� ������
									if( newSize > oldSize || alignment > oldSize )
										{
											// ���·��� size �ߴ���ڴ�
											Handle newBlock
												{
													this->Malloc( newSize, alignment )
												};

											// ���� ԭ���� �� ���ڴ�
											memcpy( newBlock, oldBlock, oldSize );

											// �ͷ�ԭ�ڴ� �� �Ƴ�����
											oldPool->Free( oldBlock );

											this->mapBlocks.erase( p );

											// ���� ���ڴ�
											return newBlock;
										}
								}
						}
					else
						{
							// ��� size == 0 ���ͷ�
							this->Free( oldBlock );

							return nullptr;
						}
				}
			else
				{
					// ��� pOriginal == nullptr ���·���
					return this->Malloc( newSize, alignment );
				}

			return oldBlock;
		}
}