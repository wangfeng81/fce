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
			// 复位重复次数
			this->repeats = 0;

			// 不同周期类型 设置起始时间
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

					// 不应有 未处理的类型
					default: std::unreachable();
				}

			// 更新到 触发时间
			this->UpdateTriggerTime();
		}


	/*
	 @	检测重复次数
	 */
	Bool Timer::CheckRepeat() const
		{
			// 检测 重复次数
			if( this->info.repeats
			 && this->info.repeats == this->repeats )
				{
					return true;
				}

			return false;
		}


	/*
	 @	更新 触发时间
	 */
	Void Timer::UpdateTriggerTime()
		{
			// 重复计数 +1
			++this->repeats;

			// 不同周期类型
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

					// 不应有 未处理的类型
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
			// 析构 openTimers 列表
			for( Timer* timer : this->openTimers )
				{
					this->pool.Destruct( timer );
				}

			// 析构 closeTimers 列表
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

			// 线程加锁
			this->rmutex.lock();

			// 加入 open表
			this->openTimers.emplace_back( timer );

			// 获取最短 timer
			this->shortest = this->GetShortest();

			// 线程解锁
			this->rmutex.unlock();

			// 通知 刷新
			this->semaphore.release();

			return timer;
		}


	/*
	 @	Remove
	 */
	Bool TimerManagement::Remove( Timer* timer,
								  stl::vector< Timer* >& listTimers )
		{
			// 查找 timer
			auto p{ stl::find( listTimers, timer ) };

			if( p != listTimers.end() )
				{
					// 移除 timer
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
			// 线程加锁
			this->rmutex.lock();

			// 从open移除 timer
			if( this->Remove( timer, this->openTimers ) )
				{
					// 获取最短 timer
					this->shortest = this->GetShortest();
				}
			else
				{
					// 从close移除 timer
					this->Remove( timer, this->closeTimers );
				};

			// 线程解锁
			this->rmutex.unlock();

			// 释放 timer
			this->pool.Destroy( timer );

			// 通知 刷新
			this->semaphore.release();
		}


	/*
	 @	Cancel
	 */
	Void TimerManagement::Cancel( Timer* timer )
		{
			// 线程加锁
			this->rmutex.lock();

			// 从 openTimers 移除
			this->Remove( timer, this->openTimers );

			// 放入 closeTimers
			this->closeTimers.emplace_back( timer );

			// 获取最短 timer
			this->shortest = this->GetShortest();

			// 线程解锁
			this->rmutex.unlock();

			// 通知 刷新
			this->semaphore.release();
		}


	/*
	 @	Restart
	 */
	Void TimerManagement::Restart( Timer* timer )
		{
			// 线程加锁
			this->rmutex.lock();

			// 复位
			timer->Reset();

			// 从 closeTimers 移回 openTimers
			this->Remove( timer, this->closeTimers );

			this->openTimers.emplace_back( timer );

			// 获取最短 timer
			this->shortest = this->GetShortest();

			// 线程解锁
			this->rmutex.unlock();

			// 通知 刷新
			this->semaphore.release();
		}


	/*
	 @	获取最短时间的 timer
	 */
	Timer* TimerManagement::GetShortest() const
		{
			// 比较大小 函数式
			auto f = []( const Timer* a, const Timer* b ) NOEXCEPT
				{
					return a->triggerTime < b->triggerTime;
				};

			// 查找 最短时间
			const auto p
				{
					std::min_element( this->openTimers.begin(),
									  this->openTimers.end(), f )
				};

			// 如果列表为空 则返回 nullptr
			return p != this->openTimers.end() ? *p : nullptr;
		}


	/*
	 @	UpdateTriggerTime
	 */
	Timer::TimePoint TimerManagement::UpdateTriggerTime()
		{
			// 函数加锁
			std::lock_guard
				< std::recursive_mutex > lock( this->rmutex );

			// 获取当前 time point
			auto now{ std::chrono::steady_clock::now() };

			// 最长等待 时间
			Timer::TimePoint maxTimePoint{ now + INTERVAL };

			// 检测是否为空
			if( this->shortest )
				{
					// 检测 是否超时
					if( this->shortest->isTimeout( now ) )
						{
							// 投递 定时器消息
							s_App.Post< Timer::Message >( this->shortest );

							// 检测是否 达到重复次数
							if( this->shortest->CheckRepeat() )
								{
									// 取消 timer
									this->Cancel( this->shortest );

									// 检测是否为空
									if( ! this->shortest )
										{
											return maxTimePoint;
										}
								}
							else
								{
									// 更新 触发时间
									this->shortest->UpdateTriggerTime();
								}

							// 刷新 第一个timer
							this->shortest = this->GetShortest();
						}

					// 取间隔 更短的
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
			// 设置 true 标志
			this->continued.test_and_set();

			// 低优先级
			std::this_thread::yield();

			// 线程循环
			while( this->continued.test() )
				{
					// 等待时间，调整误差
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
			// 通知并等待 线程结束
			this->continued.clear();

			this->semaphore.release();

			this->thread.join();
		}
}