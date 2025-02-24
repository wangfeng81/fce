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
	 @	����
	 */
	JobManagement::~JobManagement() noexcept
		{
			// ��� job ����
			this->Clear();

			// ���� job threads �б�
			this->listJobThreads.DestructAll();
		}


	/*
	 @	����
	 */
	JobManagement::JobManagement() noexcept
		:
			queue( 0 ),
			semaphore( 0 ),
			threadPool( this->OptimizeQuantity() )
		{
			// ���� �߳�ѭ�� true ��ʼ��־
			this->continued.test_and_set();

			// ��ʼ�� �̳߳�
			for( std::thread& thread : this->threadPool )
				{
					thread = std::thread( &JobManagement::operator(),
										  this );
				}
		}


	/*
	 @	�������ŵ� �߳�����

			���ٿ��� һ���߳�
			���ж�� ��ȥ����
	 */
	FC_INLINE UInt32 JobManagement::OptimizeQuantity() const noexcept
		{
			// ��ȡ Ӳ���߳�����
			UInt32 n{ std::thread::hardware_concurrency() };

			// ���� ��������
			return n > 2 ? n - 2 : 1;
		}


	/*
	 @	��� ��ҵ����

			������������Խ����� Job
			�ڴ��ͳһ�ͷ��ڴ�
	 */
	Void JobManagement::Clear() noexcept
		{
			// ��� job����
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
			// ֪ͨ �̳߳� �˳�
			this->continued.clear();

			this->semaphore.release();

			// ��� �̳߳� ����
			this->Clear();

			// �ȴ� JobThread �б����
			for( JobThread* jobThread : this->listJobThreads )
				{
					jobThread->Finish();
				}

			// �ȴ� �̳߳ؽ���
			for( std::thread& thread : this->threadPool )
				{
					thread.join();
				}

			// ���� ��ʱ��
			s_TimerMgmt.Finish();
		}


	/*
	 @	����һ�� JobThread
	 */
	JobThread* JobManagement::Generate( const Handler handler ) noexcept
		{
			return this->listJobThreads.Generate( handler );
		}


	/*
	 @	����һ�� JobThread
	 */
	Void JobManagement::Destroy( JobThread* const jobThread ) noexcept
		{
			// �ȴ� JobThread ����
			jobThread->Finish();

			// ���� JobThread
			this->listJobThreads.Destroy( jobThread );
		}


	/*
	 @	����һ�� ��ҵ����
	 */
	Void JobManagement::Schedule( Job* const job ) noexcept
		{
			// �� job ������Ϣ���У�ѭ��ȷ���ɹ�
			while( ! this->queue.push( job ) )
				{
					std::this_thread::sleep_for( 1s );
				}

			// ����һ������ thread ������Ϣ
			this->semaphore.release();
		}


	/*
	 @	��ҵ�̳߳� ��ں���
	 */
	Void JobManagement::operator() () NOEXCEPT
		{
			Job* job;

			// �̳߳� ѭ����������־ �Ƿ��˳�
			while( this->continued.test() )
				{
					// �� mq ��ȡ������ job
					while( this->queue.pop( job ) )
						{
							// ���� �쳣
							#ifdef FC_SECURITY
							try {
							#endif

								// ִ�� job
								job->Execute();

							#ifdef FC_SECURITY
								}
							catch( ... )
								{
									s_App.on_Exception();
								}
							#endif

							// Ͷ�� ���߳�
							s_App.Post( Application::EVENT::JOB,
										job );
						}

					// ��������к� ����ȴ�
					this->semaphore.acquire();
				};

			// ֪ͨ �̳߳��� ʣ�µ��߳��˳�
			this->semaphore.release();
		}
}