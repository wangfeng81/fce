/*
**
**	Timer.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../common/Job.h"


namespace fce
{
	/*
	 !	Timer

			Void( Void )
	 */
	class FC_API Timer final
		:
		/*	Trigger chain
		 */
		public InvokerChainWrapper<>
		{
			/*	Allocated by object pool
			*/
			OBJECT_POOL( Timer,
						 TimerManagement );

			/*	Friends
			*/
			friend Application;

			/*	Message
			 */
			struct FC_API Message final
				:
				public Object
				{
					/*	Allocated by polymorphic pool
					*/
					POLYMORPHIC_POOL( Message,
									  MemoryManagement );

				public:

					Message( Timer* timer )
						:
							timer( timer )
						{
						}

					Timer* timer;
				};

		public:
			/*	Duration

					Floating point number in seconds
			*/
			using Duration = std::chrono::duration< Float64 >;

			/*	Time point
			*/
			using TimePoint = std::chrono::time_point
							< std::chrono::steady_clock, Duration >;

			/*	PERIOD
			 */
			enum struct PERIOD
				:
					Byte
				{
					CUSTOM,

					DAY,
					MONTH,
					YEAR,

					WEEK,

					MAXIMUM
				};

			/*	Info
			 */
			struct FC_API Info final
				{
					/*	Model
					*/
					PERIOD period;

					UInt32 repeats;

					union
						{
							Duration duration;

							struct
								{
									UInt8 year, mon, day,
										  hour, min, sec;

									Word reserved;
								};
						};

					/*	Constructor
					*/
					Info() = default;

					Info( const Duration&,
						  const UInt32 repeats = 0 );

					Info( const tm& startTime,
						  const PERIOD period,
						  const UInt32 repeats = 0 );

					/*	Conversion
					*/
					operator tm() const;
				};

		public:
			/*	Get info
			*/
			const Info& GetInfo() const;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Timer );

			~Timer() = default;

			Timer( const Info& );

			/*	Reset
			*/
			Void Reset();

			/*	Check repeat
			*/
			Bool CheckRepeat() const;

			/*	isTimeout
			*/
			Bool isTimeout( TimePoint ) const;

			/*	Update trigger time
			*/
			Void UpdateTriggerTime();

		private:
			/*	Info
			*/
			Info info;

			/*	Repeat count
			*/
			UInt32 repeats;

			/*	Trigger time
			*/
			TimePoint triggerTime;
		};
}



namespace fce
{
	/*
	 !	TimerManagement
	 */
	class FC_API TimerManagement final
		{
			/*	Friends
			*/
			friend Engine;

			friend Application;

			friend JobManagement;

		public:
			/*	Add
			*/
			Timer* Add( const Timer::Info& );

			Timer* Add( const Timer::Duration&,
						const UInt32 repeats = 0 );

			Timer* Add( const tm& startTime,
						const Timer::PERIOD,
						const UInt32 repeats = 0 );

			/*	Remove
			*/
			Void Remove( Timer* );

			/*	Cancel
			*/
			Void Cancel( Timer* );

			/*	Restart
			*/
			Void Restart( Timer* );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( TimerManagement );

			~TimerManagement();

			TimerManagement();

			/*	Finish
			*/
			Void Finish();

			/*	Thread proc
			*/
			Void operator() ();

			/*	Get the shortest timer
			*/
			Timer* GetShortest() const;

			/*	Remove
			*/
			Bool Remove( Timer*,
						 stl::vector< Timer* >& );

			/*	Update trigger time
			*/
			Timer::TimePoint UpdateTriggerTime();

		private:
			/*	timer
			*/
			Timer* shortest;

			/*	Thread
			*/
			std::atomic_flag continued;

			std::recursive_mutex rmutex;

			std::binary_semaphore semaphore;

			std::thread thread;

			/*	Timer list
			*/
			stl::vector< Timer* > openTimers;

			stl::vector< Timer* > closeTimers;

			ObjectPool< Timer, TimerManagement > pool;

			/*	Interval
			*/
			static constexpr Timer::Duration INTERVAL = 60s;
		};


	/*
	 @	TimerManagement singleton shortcut
	 */
	extern FC_API TimerManagement& s_TimerMgmt;
}



/**
 !	class Timer
 */
namespace fce
{
	/*
	 @	Get timer info
	 */
	FC_INLINE const Timer::Info& Timer::GetInfo() const
		{
			return this->info;
		}

	/*
	 @	Is timeout
	 */
	FC_INLINE Bool Timer::isTimeout( TimePoint timePoint ) const
		{
			return timePoint >= this->triggerTime;
		}
}



/**
 !	class TimerManagement
 */
namespace fce
{
	/*
	 @	Add Timer
	 */
	FC_INLINE Timer* TimerManagement::Add( const Timer::Duration& duration,
										   const UInt32 repeats )
		{
			return this->Add( { duration, repeats } );
		}

	/*
	 @	Add Timer
	 */
	FC_INLINE Timer* TimerManagement::Add( const tm& startTime,
										   const Timer::PERIOD period,
										   const UInt32 repeats )
		{
			return this->Add( { startTime, period, repeats } );
		}
}