/*
**
**	Job.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/platform/Application.h"

#include "job/JobThread.cxx"


namespace fce
{
	/*
	 @	析构
	 */
	JobManagement::~JobManagement() noexcept
		{
			// 清空 job 队列
			this->Clear();

			// 析构 job threads 列表
			this->listJobThreads.DestructAll();
		}


	/*
	 @	构造
	 */
	JobManagement::JobManagement() noexcept
		:
			queue( 0 ),
			semaphore( 0 ),
			threadPool( this->OptimizeQuantity() )
		{
			// 设置 线程循环 true 初始标志
			this->continued.test_and_set();

			// 初始化 线程池
			for( std::thread& thread : this->threadPool )
				{
					thread = std::thread( &JobManagement::operator(),
										  this );
				}
		}


	/*
	 @	估算最优的 线程数量

			至少开启 一个线程
			如有多个 减去两个
	 */
	FC_INLINE UInt32 JobManagement::OptimizeQuantity() const noexcept
		{
			// 获取 硬件线程数量
			UInt32 n{ std::thread::hardware_concurrency() };

			// 估算 最优数量
			return n > 2 ? n - 2 : 1;
		}


	/*
	 @	清空 作业队列

			程序结束，可以仅析构 Job
			内存池统一释放内存
	 */
	Void JobManagement::Clear() noexcept
		{
			// 清空 job队列
			Job* job;

			while( this->queue.pop( job ) )
				{
					job->~Job();
				}
		}


	/*
	 @	Finish

			one call
	 */
	Void JobManagement::Finish() noexcept
		{
			// 通知 线程池 退出
			this->continued.clear();

			this->semaphore.release();

			// 清空 线程池 队列
			this->Clear();

			// 等待 JobThread 列表结束
			for( JobThread* jobThread : this->listJobThreads )
				{
					jobThread->Finish();
				}

			// 等待 线程池结束
			for( std::thread& thread : this->threadPool )
				{
					thread.join();
				}

			// 结束 定时器
			s_TimerMgmt.Finish();
		}


	/*
	 @	创建一个 JobThread
	 */
	JobThread* JobManagement::Generate( const Handler handler ) noexcept
		{
			return this->listJobThreads.Generate( handler );
		}


	/*
	 @	销毁一个 JobThread
	 */
	Void JobManagement::Destroy( JobThread* const jobThread ) noexcept
		{
			// 等待 JobThread 结束
			jobThread->Finish();

			// 销毁 JobThread
			this->listJobThreads.Destroy( jobThread );
		}


	/*
	 @	安排一个 作业任务
	 */
	Void JobManagement::Schedule( Job* const job ) noexcept
		{
			// 将 job 放入消息队列，循环确保成功
			while( ! this->queue.push( job ) )
				{
					std::this_thread::sleep_for( 1s );
				}

			// 唤醒一个空闲 thread 处理消息
			this->semaphore.release();
		}


	/*
	 @	作业线程池 入口函数
	 */
	Void JobManagement::operator() () NOEXCEPT
		{
			Job* job;

			// 线程池 循环，并检测标志 是否退出
			while( this->continued.test() )
				{
					// 从 mq 获取并弹出 job
					while( this->queue.pop( job ) )
						{
							// 捕获 异常
							#ifdef FC_SECURITY
							try {
							#endif

								// 执行 job
								job->Execute();

							#ifdef FC_SECURITY
								}
							catch( ... )
								{
									s_App.on_Exception();
								}
							#endif

							// 投递 主线程
							s_App.Post( Application::EVENT::JOB,
										job );
						}

					// 处理完队列后 进入等待
					this->semaphore.acquire();
				};

			// 通知 线程池内 剩下的线程退出
			this->semaphore.release();
		}
}