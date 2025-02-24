/*
**
**	Async object.
**
		One thread sets one thread to get
		single-producer / single-consumer

		It can only be set once
		and can be get multiple times

	Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../Utility.h"


namespace fce
{
	/*
	 !	Async object
	 */
	class FC_API AsyncObject final
		{
			/*	Friends
			*/
			template< X_object >

			friend class AsyncOperation;

		public:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( AsyncObject );

			~AsyncObject() = default;

			AsyncObject( Object* = nullptr ) noexcept;

			/*	Complete
			*/
			Void Complete();

			/*	Wait for completion
			*/
			Void Wait() const noexcept;

			/*	Is done
			*/
			[[nodiscard]]

			Bool isDone() const noexcept;

			/*	Get & Set	object
			*/
			template< X_object T = Object >

			[[nodiscard]]

			T* Get() const noexcept;

			Void Set( Object* );

			/*	Get & Set	progress
			*/
			[[nodiscard]]

			Float32 GetProgress() const noexcept;

			Void SetProgress( Float32 ) noexcept;

		protected:
			/*	Destroy
			*/
			Void Destroy() const noexcept;

		private:
			/*	Object
			*/
			Object* object;

			/*	Flag
			*/
			std::atomic_flag flag;

			/*	Progress
			*/
			std::atomic< Float32 > progress;
		};
}



/**
 !	class AsyncObject
 */
namespace fce
{
	/*
	 @	Wait
	 */
	FC_INLINE Void AsyncObject::Wait() const noexcept
		{
			this->flag.wait( false );
		}

	/*
	 @	Is done
	 */
	FC_INLINE Bool AsyncObject::isDone() const noexcept
		{
			return this->flag.test();
		}

	/*
	 @	Get progress
	 */
	FC_INLINE Float32 AsyncObject::GetProgress() const noexcept
		{
			return this->progress.load();
		}

	/*
	 @	Set progress
	 */
	FC_INLINE Void AsyncObject::SetProgress( Float32 progress ) noexcept
		{
			this->progress.store( progress );
		}

	/*
	 @	Get object
	 */
	template< X_object T >

	FC_INLINE T* AsyncObject::Get() const noexcept
		{
			this->Wait();

			FC_ASSERT_LOG( this->object
						== dynamic_cast< T* >( this->object ),
						   "Cannot convert to %s",
						   typeid( T ).name() );

			return static_cast< T* >( this->object );
		}
}