/*
**
**	Job thread.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#pragma push_macro("new")
	#undef new
	#include <boost/lockfree/queue.hpp> 
#pragma pop_macro("new")

#include "AsyncOperation.h"


namespace fce
{
	/*
	 !	Job thread with message queue

			Handler

				Bool ( Object & )

			If false is returned
			the message object will
			not be destroyed by the JobThread

			If you want to destroy it automatically
			return true
	 */
	class FC_API JobThread final
		{
			/*	Allocated by asset pool
			*/
			ASSET_POOL2( JobManagement,
						 JobThread );

		public:
			/*	Handler
			*/
			using Handler = Bool( * )( Object& );

			/*	Post
			*/
			template< X_object T,
					  typename ...Args >

			const T* Post( Args&&... );

			/*	Destroy & Generate
			*/
			Void Destroy() noexcept;

			[[nodiscard]] static JobThread* Generate( Handler ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( JobThread );

			~JobThread() noexcept;

			JobThread( Handler ) NOEXCEPT;

			/*	Clear
			*/
			Void Clear() noexcept;

			/*	Finish
			*/
			Void Finish() noexcept;

			/*	Post
			*/
			Void Post( Object * const );

			/*	Processing function
			*/
			Void operator() ();

		private:
			/*	Handler
			*/
			Handler handler;

			/*	Message queue
			*/
			boost::lockfree::queue
				<
					Object*,
					boost::lockfree
						 ::fixed_sized< false >,
					boost::lockfree
						 ::allocator< stl::allocator< Object* > >
				>
				queue;

			/*	Thread
			*/
			std::atomic_flag continued;

			std::binary_semaphore semaphore;

			std::thread thread;
		};
}



/**
 !	class JobThread
 */
namespace fce
{
	/*
	 @	Post
	 */
	template< X_object T,
			  typename ...Args >

	FC_INLINE const T* JobThread::Post( Args&& ...args )
		{
			T* obj
				{
					s_MemMgmt.Generate< T >
						(
							std::forward< Args >( args )...
						)
				};

			this->Post( obj );

			return obj;
		}
}