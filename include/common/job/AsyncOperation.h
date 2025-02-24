/*
**
**	Async operation.
**
		The life cycle of the message
		is consistent with AsyncOperation

	Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "AsyncObject.h"


namespace fce
{
	/*
	 !	Async operation
	 */
	template< X_object T >

	class FC_API AsyncOperation final
		{
			/*	Friends
			*/
			friend AsyncObject;

			friend AssetManagement;

			friend StorageManagement;

		public:
			/*	Wait for completion
			*/
			Void Wait() const noexcept;

			/*	Is done
			*/
			[[nodiscard]]

			Bool isDone() const noexcept;

			/*	Get message object
			*/
			[[nodiscard]]

			const T* Get() const noexcept;

			/*	Get progress
			*/
			[[nodiscard]]

			Float32 GetProgress() const noexcept;

			/*	Get the member of the message
			*/
			const T* operator -> () const noexcept;

			/*	Constructor & Destructor
			*/
			~AsyncOperation() noexcept;

			AsyncOperation( AsyncOperation&& asyOp ) noexcept;

		protected:
			/*	Constructor
			*/
			AsyncOperation( const AsyncObject& asyObj ) noexcept;

		private:
			/*	Async object
			*/
			const AsyncObject* asyObj;
		};
}



/**
 !	class AsyncOperation
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	template< X_object T >

	FC_INLINE AsyncOperation< T >::AsyncOperation( const AsyncObject& asyObj ) noexcept
		:
			asyObj( &asyObj )
		{
		}

	template< X_object T >

	FC_INLINE AsyncOperation< T >::AsyncOperation( AsyncOperation&& asyOp ) noexcept
		:
			asyObj( asyOp.asyObj )
		{
			asyOp.asyObj = nullptr;
		}

	/*
	 @	Destructor
	 */
	template< X_object T >

	FC_INLINE AsyncOperation< T >::~AsyncOperation() noexcept
		{
			if( this->asyObj )
				{
					this->asyObj->Destroy();
				}
		}

	/*
	 @	Wait for completion
	 */
	template< X_object T >

	FC_INLINE Void AsyncOperation< T >::Wait() const noexcept
		{
			this->asyObj->Wait();
		}

	/*
	 @	Is done
	 */
	template< X_object T >

	FC_INLINE Bool AsyncOperation< T >::isDone() const noexcept
		{
			return this->asyObj->isDone();
		}

	/*
	 @	Get message object
	 */
	template< X_object T >

	FC_INLINE const T* AsyncOperation< T >::Get() const noexcept
		{
			return this->asyObj->Get< T >();
		}

	/*
	 @	Get progress
	 */
	template< X_object T >

	FC_INLINE Float32 AsyncOperation< T >::GetProgress() const noexcept
		{
			return this->asyObj->GetProgress();
		}

	/*
	 @	Get the member of the message
	 */
	template< X_object T >

	FC_INLINE const T* AsyncOperation< T >::operator -> () const noexcept
		{
			FC_ASSERT_LOG( this->isDone(),
						   "The async operation has not been completed" );

			const Object* obj( this->asyObj->object );

			FC_ASSERT_LOG( obj == dynamic_cast< const T* >( obj ),
						   "Cannot convert to %s",
						   typeid( T ).name() );

			return static_cast< const T* >( obj );
		}
}