/*
**
**	Job.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "job/JobThread.h"


namespace fce
{
	/*
	 !	Job
	 */
	class FC_API Job
		:
		public Object
		{
			/*	Friends
			*/
			friend JobThread;

			friend JobManagement;

			/*	Allocated by polymorphic pool
			*/
			POLYMORPHIC_POOL( Job,
							  MemoryManagement );

		public:
			/*	Execute

					Sub thread
			*/
			virtual Void Execute() = 0;

			/*	On completed

					Main thread
			*/
			virtual Void onCompleted() NOEXCEPT
				{
				}

		protected:
			/*	Constructor & Destructor
			*/
			Job() = default;

			virtual ~Job() = default;
		};


	/*	concept		Derived job
	 */
	template< class T >

	concept X_job = std::derived_from< T, Job >;
}



namespace fce
{
	/*
	 !	Job management

			The job will be
			created on scheduling and

			automatically destroyed
			immediately after finished
	 */
	class FC_API JobManagement final
		{
			/*	Friends
			*/
			friend Singleton;

			friend Application;

			/*	Handler
			*/
			using Handler = JobThread::Handler;

		public:
			/*	Schedule
			*/
			template< X_job T,
					  typename ...Args >

			Void Schedule( Args&&... ) noexcept;

			/*	Destroy & Generate
			*/
			Void Destroy( JobThread* const ) noexcept;

			[[nodiscard]] JobThread* Generate( const Handler ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( JobManagement );

			~JobManagement() noexcept;

			JobManagement() noexcept;

			/*	Clear
			*/
			Void Clear() noexcept;

			/*	Finish
			*/
			Void Finish() noexcept;

			/*	Processing function
			*/
			Void operator() () NOEXCEPT;

			/*	Schedule
			*/
			Void Schedule( Job* const ) noexcept;

			/*	Optimize number of threads
			*/
			UInt32 OptimizeQuantity() const noexcept;

		private:
			/*	Job queue
			*/
			boost::lockfree::queue
				<
					Job *,
					boost::lockfree
						 ::fixed_sized< false >,
					boost::lockfree
						 ::allocator< stl::allocator< Object* > >
				>
				queue;

			/*	continued
			*/
			std::atomic_flag continued;

			/*	Semaphore
			*/
			std::binary_semaphore semaphore;

			/*	Thread pool
			*/
			stl::vector< std::thread > threadPool;

			/*	JobThread list
			*/
			AssetPool< JobManagement, JobThread > listJobThreads;
		};


	/*
	 @	s_JobMgmt
	 */
	SINGLETON_SHORTCUT_EX( JobManagement,
						   JobMgmt );
}



/**
 !	class JobManagement
 */
namespace fce
{
	/*
	 @	Schedule a job
	 */
	template< X_job T,
			  typename ...Args >

	FC_INLINE Void JobManagement::Schedule( Args&& ...args ) noexcept
		{
			T* job = s_MemMgmt.Generate< T >
						(
							std::forward< Args >( args )...
						);

			this->Schedule( job );
		}
}



/**
 !	JobThread ( Syntax sugar )
 */
namespace fce
{
	/*
	 @	Destroy
	 */
	FC_INLINE Void JobThread::Destroy() noexcept
		{
			return s_JobMgmt.Destroy( this );
		}

	/*
	 @	Create
	 */
	FC_INLINE JobThread* JobThread::Generate( Handler handler ) noexcept
		{
			return s_JobMgmt.Generate( handler );
		}
}