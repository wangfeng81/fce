/*
**
**	Timer.
**
		Copyright (c) WangFeng since 2000...
*/

#include "../../include/platform/Application.h"


/**
 !	Info
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	Timer::Info::Info( const Duration& duration,
					   const UInt32 repeats )
		:
			period( PERIOD::CUSTOM ),
			duration( duration ),
			repeats( repeats )
		{
		}


	/*
	 @	Constructor
	 */
	Timer::Info::Info( const tm& startTime,
					   const PERIOD period,
					   const UInt32 repeats )
		:
			reserved( 0 ),
			period( period ),
			repeats( repeats ),

			year( startTime.tm_year ),
			mon( startTime.tm_mon ),
			day( startTime.tm_mday ),

			hour( startTime.tm_hour ),
			min( startTime.tm_min ),
			sec( startTime.tm_sec )
		{
		}


	/*
	 @	Conversion
	 */
	Timer::Info::operator tm() const
		{
			return tm{ this->sec, this->min, this->hour,
					   this->day, this->mon, this->year,
					   0, 0, 0 };
		}
}



/**
 !	Timer
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	Timer::Timer( const Info& info )
		:
			info( info )
		{
			this->Reset();
		}


	/*
	 @	Reset
	 */
	Void Timer::Reset()
		{
			// ��λ�ظ�����
			this->repeats = 0;

			// ��ͬ�������� ������ʼʱ��
			switch( this->info.period )
				{
					case PERIOD::CUSTOM:
						{
							triggerTime =
								std::chrono::steady_clock::now();
						}
						break;

					case PERIOD::DAY:
						break;

					// ��Ӧ�� δ���������
					default: std::unreachable();
				}

			// ���µ� ����ʱ��
			this->UpdateTriggerTime();
		}


	/*
	 @	����ظ�����
	 */
	Bool Timer::CheckRepeat() const
		{
			// ��� �ظ�����
			if( this->info.repeats
			 && this->info.repeats == this->repeats )
				{
					return true;
				}

			return false;
		}


	/*
	 @	���� ����ʱ��
	 */
	Void Timer::UpdateTriggerTime()
		{
			// �ظ����� +1
			++this->repeats;

			// ��ͬ��������
			switch( this->info.period )
				{
					case PERIOD::CUSTOM:
						{
							this->triggerTime +=
								this->info.duration;
						}
						break;

					case PERIOD::DAY:
						{
							this->triggerTime += 24h;
						}
						break;

					case PERIOD::WEEK:
						{
							this->triggerTime += 24h * 7;
						}
						break;

					case PERIOD::MONTH:
						{
						}
						break;

					case PERIOD::YEAR:
						{
						}
						break;

					// ��Ӧ�� δ���������
					default: std::unreachable();
				}
		}
}



/**
 !	TimerManagement
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	TimerManagement::TimerManagement()
		:
			shortest( nullptr ),
			semaphore( 0 ),
			thread( &TimerManagement::operator(), this )
		{
		}


	/*
	 @	Destructor
	 */
	TimerManagement::~TimerManagement()
		{
			// ���� openTimers �б�
			for( Timer* timer : this->openTimers )
				{
					this->pool.Destruct( timer );
				}

			// ���� closeTimers �б�
			for( Timer* timer : this->closeTimers )
				{
					this->pool.Destruct( timer );
				}
		}


	/*
	 @	Add
	 */
	Timer* TimerManagement::Add( const Timer::Info& info )
		{
			Timer* timer{ this->pool.Generate( info ) };

			// �̼߳���
			this->rmutex.lock();

			// ���� open��
			this->openTimers.emplace_back( timer );

			// ��ȡ��� timer
			this->shortest = this->GetShortest();

			// �߳̽���
			this->rmutex.unlock();

			// ֪ͨ ˢ��
			this->semaphore.release();

			return timer;
		}


	/*
	 @	Remove
	 */
	Bool TimerManagement::Remove( Timer* timer,
								  stl::vector< Timer* >& listTimers )
		{
			// ���� timer
			auto p{ stl::find( listTimers, timer ) };

			if( p != listTimers.end() )
				{
					// �Ƴ� timer
					listTimers.erase( p );

					return true;
				}

			return false;
		}


	/*
	 @	Remove
	 */
	Void TimerManagement::Remove( Timer* timer )
		{
			// �̼߳���
			this->rmutex.lock();

			// ��open�Ƴ� timer
			if( this->Remove( timer, this->openTimers ) )
				{
					// ��ȡ��� timer
					this->shortest = this->GetShortest();
				}
			else
				{
					// ��close�Ƴ� timer
					this->Remove( timer, this->closeTimers );
				};

			// �߳̽���
			this->rmutex.unlock();

			// �ͷ� timer
			this->pool.Destroy( timer );

			// ֪ͨ ˢ��
			this->semaphore.release();
		}


	/*
	 @	Cancel
	 */
	Void TimerManagement::Cancel( Timer* timer )
		{
			// �̼߳���
			this->rmutex.lock();

			// �� openTimers �Ƴ�
			this->Remove( timer, this->openTimers );

			// ���� closeTimers
			this->closeTimers.emplace_back( timer );

			// ��ȡ��� timer
			this->shortest = this->GetShortest();

			// �߳̽���
			this->rmutex.unlock();

			// ֪ͨ ˢ��
			this->semaphore.release();
		}


	/*
	 @	Restart
	 */
	Void TimerManagement::Restart( Timer* timer )
		{
			// �̼߳���
			this->rmutex.lock();

			// ��λ
			timer->Reset();

			// �� closeTimers �ƻ� openTimers
			this->Remove( timer, this->closeTimers );

			this->openTimers.emplace_back( timer );

			// ��ȡ��� timer
			this->shortest = this->GetShortest();

			// �߳̽���
			this->rmutex.unlock();

			// ֪ͨ ˢ��
			this->semaphore.release();
		}


	/*
	 @	��ȡ���ʱ��� timer
	 */
	Timer* TimerManagement::GetShortest() const
		{
			// �Ƚϴ�С ����ʽ
			auto f = []( const Timer* a, const Timer* b ) NOEXCEPT
				{
					return a->triggerTime < b->triggerTime;
				};

			// ���� ���ʱ��
			const auto p
				{
					std::min_element( this->openTimers.begin(),
									  this->openTimers.end(), f )
				};

			// ����б�Ϊ�� �򷵻� nullptr
			return p != this->openTimers.end() ? *p : nullptr;
		}


	/*
	 @	UpdateTriggerTime
	 */
	Timer::TimePoint TimerManagement::UpdateTriggerTime()
		{
			// ��������
			std::lock_guard
				< std::recursive_mutex > lock( this->rmutex );

			// ��ȡ��ǰ time point
			auto now{ std::chrono::steady_clock::now() };

			// ��ȴ� ʱ��
			Timer::TimePoint maxTimePoint{ now + INTERVAL };

			// ����Ƿ�Ϊ��
			if( this->shortest )
				{
					// ��� �Ƿ�ʱ
					if( this->shortest->isTimeout( now ) )
						{
							// Ͷ�� ��ʱ����Ϣ
							s_App.Post< Timer::Message >( this->shortest );

							// ����Ƿ� �ﵽ�ظ�����
							if( this->shortest->CheckRepeat() )
								{
									// ȡ�� timer
									this->Cancel( this->shortest );

									// ����Ƿ�Ϊ��
									if( ! this->shortest )
										{
											return maxTimePoint;
										}
								}
							else
								{
									// ���� ����ʱ��
									this->shortest->UpdateTriggerTime();
								}

							// ˢ�� ��һ��timer
							this->shortest = this->GetShortest();
						}

					// ȡ��� ���̵�
					return math::Min( maxTimePoint,
									  this->shortest->triggerTime );
				}

			return maxTimePoint;
		}


	/*
	 @	Thread proc
	 */
	Void TimerManagement::operator() ()
		{
			// ���� true ��־
			this->continued.test_and_set();

			// �����ȼ�
			std::this_thread::yield();

			// �߳�ѭ��
			while( this->continued.test() )
				{
					// �ȴ�ʱ�䣬�������
					if( this->semaphore.try_acquire_until(
							this->UpdateTriggerTime() - 0.01s ) )
						{
						}
				};
		}


	/*
	 @	Finish
	 */
	Void TimerManagement::Finish()
		{
			// ֪ͨ���ȴ� �߳̽���
			this->continued.clear();

			this->semaphore.release();

			this->thread.join();
		}
}