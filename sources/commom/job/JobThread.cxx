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
	 @	����
	 */
	AsyncObject::AsyncObject( Object* obj ) noexcept
		:
			object( obj ),
			progress( 0.0f )
		{
		}

	/*
	 @	���
	 */
	Void AsyncObject::Complete()
		{
			FC_ASSERT_LOG( this->object,
						   "Async obj has not been set" );

			this->flag.test_and_set();

			this->flag.notify_one();
		}

	/*
	 @	����

			ֻ���� һ���߳��ڵ���
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
	 @	����

			object ����Ϊ��
	 */
	Void AsyncObject::Destroy() const noexcept
		{
			// �ȴ� ���
			this->Wait();

			// �ͷ� obj
			s_MemMgmt.Destroy( this->object );
		}
}



/**
 !	JobThread
 */
namespace fce
{
	/*
	 @	����
	 */
	JobThread::~JobThread() noexcept
		{
			// ��� obj����
			this->Clear();
		}


	/*
	 @	����
	 */
	JobThread::JobThread( Handler handler ) NOEXCEPT
		:
			queue( 0 ),
			semaphore( 0 ),
			handler( handler )
		{
			FC_ASSERT_LOG( this->handler,
						   "JobThread: Invalid delegate handler" );

			// ���� �߳�ѭ�� true ��ʼ��־
			this->continued.test_and_set();

			// ���첢�ƶ��̣߳�ȷ�� continued ������ true
			thread = std::thread( &JobThread::operator(),
								  this );
		}


	/*
	 @	��� ��Ϣ����
	 */
	Void JobThread::Clear() noexcept
		{
			// ��� obj����
			Object* obj;

			while( this->queue.pop( obj ) )
				{
					s_MemMgmt.Destroy( obj );
				}
		}


	/*
	 @	���� �߳�
	 */
	Void JobThread::Finish() noexcept
		{
			// ���� ������־
			this->continued.clear();

			// �ͷ� �ȴ��ź�
			this->semaphore.release();

			// ��� ��Ϣ����
			this->Clear();

			// �ϲ� �߳�
			this->thread.join();
		}


	/*
	 @	Ͷ�� Object �� ��Ϣ����
	 */
	Void JobThread::Post( Object * const obj )
		{
			// ѭ��ȷ���ɹ�
			while( ! this->queue.push( obj ) )
				{
					// ʧ�������ߺ� ��������
					std::this_thread::sleep_for( 1s );
				}

			// �����߳� ������Ϣ
			this->semaphore.release();
		}


	/*
	 @	�߳� ��ں���
	 */
	Void JobThread::operator() ()
		{
			Object* obj;

			// �߳�ѭ����������־ �Ƿ��˳�
			while( this->continued.test() )
				{
					// ��ȡ������ obj
					while( this->queue.pop( obj ) )
						{
							// ���� �쳣
							#ifdef FC_SECURITY
							try {
							#endif

								// ���� ������
								if( this->handler( *obj ) )
									{
										// ����true ������ Message
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

					// ��������к� ����ȴ�
					this->semaphore.acquire();
				};
		}
}