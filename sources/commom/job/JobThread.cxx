/*
**
**	Job.
**
		Copyright (c) WangFeng since 2006...
*/

/**
 !	AsyncObject
 */
namespace fce
{
	/*
	 @	构造
	 */
	AsyncObject::AsyncObject( Object* obj ) noexcept
		:
			object( obj ),
			progress( 0.0f )
		{
		}

	/*
	 @	完成
	 */
	Void AsyncObject::Complete()
		{
			FC_ASSERT_LOG( this->object,
						   "Async obj has not been set" );

			this->flag.test_and_set();

			this->flag.notify_one();
		}

	/*
	 @	设置

			只能在 一个线程内调用
	 */
	Void AsyncObject::Set( Object* obj )
		{
			FC_ASSERT_LOG( ! this->object,
						   "Async obj can only be set once" );

			this->object = obj;

			this->SetProgress( 1.0f );

			this->Complete();
		}

	/*
	 @	销毁

			object 不能为空
	 */
	Void AsyncObject::Destroy() const noexcept
		{
			// 等待 完成
			this->Wait();

			// 释放 obj
			s_MemMgmt.Destroy( this->object );
		}
}



/**
 !	JobThread
 */
namespace fce
{
	/*
	 @	析构
	 */
	JobThread::~JobThread() noexcept
		{
			// 清空 obj队列
			this->Clear();
		}


	/*
	 @	构造
	 */
	JobThread::JobThread( Handler handler ) NOEXCEPT
		:
			queue( 0 ),
			semaphore( 0 ),
			handler( handler )
		{
			FC_ASSERT_LOG( this->handler,
						   "JobThread: Invalid delegate handler" );

			// 设置 线程循环 true 初始标志
			this->continued.test_and_set();

			// 构造并移动线程，确保 continued 先设置 true
			thread = std::thread( &JobThread::operator(),
								  this );
		}


	/*
	 @	清空 消息队列
	 */
	Void JobThread::Clear() noexcept
		{
			// 清空 obj队列
			Object* obj;

			while( this->queue.pop( obj ) )
				{
					s_MemMgmt.Destroy( obj );
				}
		}


	/*
	 @	结束 线程
	 */
	Void JobThread::Finish() noexcept
		{
			// 设置 结束标志
			this->continued.clear();

			// 释放 等待信号
			this->semaphore.release();

			// 清空 消息队列
			this->Clear();

			// 合并 线程
			this->thread.join();
		}


	/*
	 @	投递 Object 到 消息队列
	 */
	Void JobThread::Post( Object * const obj )
		{
			// 循环确保成功
			while( ! this->queue.push( obj ) )
				{
					// 失败则休眠后 继续尝试
					std::this_thread::sleep_for( 1s );
				}

			// 唤醒线程 处理消息
			this->semaphore.release();
		}


	/*
	 @	线程 入口函数
	 */
	Void JobThread::operator() ()
		{
			Object* obj;

			// 线程循环，并检测标志 是否退出
			while( this->continued.test() )
				{
					// 获取并弹出 obj
					while( this->queue.pop( obj ) )
						{
							// 捕获 异常
							#ifdef FC_SECURITY
							try {
							#endif

								// 调用 处理函数
								if( this->handler( *obj ) )
									{
										// 返回true 则销毁 Message
										s_MemMgmt.Destroy( obj );
									}

							#ifdef FC_SECURITY
								}
							catch( ... )
								{
									s_MemMgmt.Destroy( obj );

									s_App.on_Exception();
								}
							#endif
						}

					// 处理完队列后 进入等待
					this->semaphore.acquire();
				};
		}
}