/*
**
**	Application.
**
		on_Frame
		on_Create
		on_Destroy 由 impl 主导调用

		关闭时 通过发送 EXIT 事件通知销毁

		需要在 on_Create 加载组件

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
	 @	构造
	 */
	Application::Application() noexcept
		:
			handler( nullptr ),
			framework( nullptr )
		{
		}


	/*
	 @	投递消息到主线程
	 */
	Void Application::Post( const EVENT e,
							const Object* const obj ) noexcept
		{
			// 循环确保成功 ( 队列已满等情况 会导致失败 )
			while( ! this->impl.Post( DWord( e ), obj ) )
				{
					std::this_thread::sleep_for( 1s );
				}
		}


	/*
	 @	请求 退出
	 */
	Bool Application::Quit( const Bool now ) noexcept
		{
			/*	先检测 now 是否立即退出
				否则检测 framework->onQuit 
			*/
			if( now || ( this->framework
					  && this->framework->on_Quit() ) )
				{
					// 设置 关闭提示
					this->PromptClosing();

					// 发送 关闭消息
					this->Post( EVENT::EXIT,
								nullptr );

					return true;
				}

			return false;
		}


	/*
	 @	截取 整个屏幕
	 */
	Bool Application::Screenshot( Image& img ) const noexcept
		{
			return this->impl.Screenshot( img, 0, 0,
										  this->impl.GetScreenWidth(),
										  this->impl.GetScreenHeight() );
		}


	/*
	 @	执行 系统命令
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
					// 转换 ansi
					stl::string strCmdLine =
						std::filesystem::path( cmdLine ).string();

					// 打开管道 并执行命令 
					if( FILE* pipe = popen( strCmdLine.c_str(), "r" ) )
						{
							// 循环读取 管道信息
							char buf[ 1024 ];

							while( ! feof( pipe ) )
								{
									if( fgets( buf, sizeof( buf ), pipe ) )
										{
											feedback += buf;
										}
								}

							// 关闭并返回 code
							return pclose( pipe );
						}

					// 失败返回 -1
					return -1;
				}
			#endif
		}


	/*
	 @	设置 关闭提示
	 */
	FC_INLINE Void Application::PromptClosing() noexcept
		{
			// 设置 tilte
			s_Window.SetTitle( L"Closing... %s",
							   s_Window.GetTitle() );

			// 隐藏窗口，避免用户输入，以及长时间等待
			s_Window.SetMode( Window::MODE::MINIMIZE );
		}
}



/**
 !	响应 事件
 */
namespace fce
{
	/*
	 @	On create
	 */
	Bool Application::on_Create( const WChar* const cmdLine ) NOEXCEPT
		{
			// 创建 Graphics
			if( s_Graphics.Create() )
				{
					// 创建 Window
					if( s_Window.Create() )
						{
							// 最后创建 framework
							if( this->framework = InjectFramework() )
								{
									// 响应 Framework::onCreate
									return this->framework->on_Create( cmdLine );
								}
						}
				}

			// 引擎 启动失败
			s_Window.PromptBox( "\n App start failed..." );

			return false;
		}


	/*
	 @	On destroy
	 */
	Void Application::on_Destroy() NOEXCEPT
		{
			// 结束 Job 线程
			s_JobMgmt.Finish();

			// 销毁 framework
			if( this->framework )
				{
					this->framework->on_Destroy();

					this->framework = nullptr;
				}

			// 清空 所有组件
			this->ClearComponents();

			// 清空 assets
			s_AssetMgmt.Clear();

			// 清空 messages
			this->impl.Finish();

			// 销毁 Window
			s_Window.Destroy();

			// 销毁 Graphics
			s_Graphics.Destroy();
		}


	/*
	 @	On frame update
	 */
	Void Application::on_Frame() NOEXCEPT
		{
			// on frame
			this->framework->on_Frame();

			// 渲染窗口
			s_Window.Render();
		}


	/*
	 @	On exception

			处理 run 作业时的 异常

			对于 create & destroy 的异常另外直接 退出
	 */
	Void Application::on_Exception() noexcept
		{
			// 只处理一次 异常
			static Bool once = true;

			if( once )
				{
					// 获取 当前异常
					std::exception_ptr eptr =
						std::current_exception();

					// 先响应 Framework::on_Exception
					if( this->framework->on_Exception( eptr ) )
						{
							// 退出
							once = false;

							this->Post( EVENT::EXIT,
										nullptr );
						}
				}
		}
}



/**
 !	收发 消息
 */
namespace fce
{
	/*
	 @	 分发消息

			impl ( this )
	 */
	FC_INLINE Void Application::Dispatch( const EVENT e,
										  Object * const obj ) noexcept
		{
			// 分发消息
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

					// 程序退出 直接返回
					case EVENT::EXIT:
						{
							this->on_Exit( obj );
						}
						return;
				}

			// 销毁 Message
			s_MemMgmt.Destroy( obj );
		}


	/*
	 @	 响应 user 消息
	 */
	FC_INLINE Void Application::on_User( Object* const obj ) NOEXCEPT
		{
			// 调用 用户处理程序
			FC_ASSERT_LOG( this->handler,
						   L"Message handler must be set before posting" );

			this->handler( *obj );
		}


	/*
	 @	 响应 job 消息
	 */
	FC_INLINE Void Application::on_Job( Object* const obj ) NOEXCEPT
		{
			// 触发 job完成函数
			Job * const job =
				static_cast< Job * const >( obj );

			job->onCompleted();
		}


	/*
	 @	响应 Timer 消息
	 */
	FC_INLINE Void Application::on_Timer( Object* const obj ) NOEXCEPT
		{
			// 触发 定时器函数
			Timer::Message * const msg =
				static_cast< Timer::Message * const >( obj );

			msg->timer->NotifyHandlers();
		}


	/*
	 @	 响应 Asset 消息
	 */
	FC_INLINE Void Application::on_Asset( Object* const obj ) NOEXCEPT
		{
			// 触发 Asset 完成函数
			Asset::Msg_Async* const msg =
				static_cast< Asset::Msg_Async* const >( obj );

			msg->callback( *msg );
		}


	/*
	 @	 响应 Storage 消息
	 */
	FC_INLINE Void Application::on_Storage( Object* const obj ) NOEXCEPT
		{
			// 触发 Storage 回调函数
			Storage::Msg_Async* const msg =
				static_cast< Storage::Msg_Async* const >( obj );

			msg->callback( *msg );
		}


	/*
	 @	响应 EXIT 消息
	 */
	FC_INLINE Void Application::on_Exit( Object* const obj ) noexcept
		{
			this->impl.Destroy();
		}
}



/**
 !	内存块

		增加 map 存储管理
		释放时 不需要 size

		这里主要考虑 健壮性
		允许 0个分配，释放空指针
 */
namespace fce
{
	/*
	 @	释放
	 */
	Void Application::Free( Handle const block ) noexcept
		{
			// 查找 map
			auto p{ this->mapBlocks.find( block ) };

			if( p != this->mapBlocks.end() )
				{
					// 释放 内存
					p->second->Free( block );

					// 移出 map
					this->mapBlocks.erase( p );
				}
		}


	/*
	 @	分配
	 */
	Handle Application::Malloc( const UInt size,
								const UInt alignment ) noexcept
		{
			// 0 尺寸返回 nullptr
			if( size )
				{
					// 获取 MemoryPool
					MemoryPool* pool
						{
							s_MemMgmt.GetPool( std::max( size, alignment ) )
						};

					// 分配 内存
					Handle block{ pool->Malloc() };

					// 存入 map
					this->mapBlocks.emplace( block, pool );

					return block;
				}

			return nullptr;
		}


	/*
	 @	重新分配
	 */
	Handle Application::Realloc( Handle const oldBlock,
										const UInt newSize,
										const UInt alignment ) noexcept
		{
			// 检测 oldBlock
			if( oldBlock )
				{
					// 检测 size
					if( newSize )
						{
							// 查找 关联的 MemoryPool
							auto p{ this->mapBlocks.find( oldBlock ) };

							if( p != this->mapBlocks.end() )
								{
									MemoryPool* const oldPool{ p->second };

									const UInt oldSize{ oldPool->GetCellSize() };

									// 如果 size 或 alignment 变大 则扩容
									if( newSize > oldSize || alignment > oldSize )
										{
											// 重新分配 size 尺寸的内存
											Handle newBlock
												{
													this->Malloc( newSize, alignment )
												};

											// 拷贝 原数据 到 新内存
											memcpy( newBlock, oldBlock, oldSize );

											// 释放原内存 并 移除关联
											oldPool->Free( oldBlock );

											this->mapBlocks.erase( p );

											// 返回 新内存
											return newBlock;
										}
								}
						}
					else
						{
							// 如果 size == 0 则释放
							this->Free( oldBlock );

							return nullptr;
						}
				}
			else
				{
					// 如果 pOriginal == nullptr 则新分配
					return this->Malloc( newSize, alignment );
				}

			return oldBlock;
		}
}