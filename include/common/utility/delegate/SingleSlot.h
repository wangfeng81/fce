/*
**
**	Single function slot.
**
		Delegate

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Delegate
	 */
	template< typename Retval = Void,
			  typename ...Args >

	class Delegate final
		{
			using FunctionSlotPool =
					inner::delegate::
					FunctionSlotPool
					< Retval, Args... >;

		public:
			/*
			 @	Constructors
			 */
			Delegate() noexcept
				:
					slot( nullptr )
				{
				}

			Delegate( std::nullptr_t ) noexcept
				:
					slot( nullptr )
				{
				}

			Delegate( const Delegate& dlgt )
				:
					slot( FunctionSlotPool
						  ::Clone( dlgt.slot ) )
				{
				}

			Delegate( Delegate&& dlgt ) noexcept
				:
					slot( dlgt.slot )
				{
					dlgt.slot = nullptr;
				}

			template< typename ...FuncX >

			Delegate( FuncX... funcX ) NOEXCEPT
				:
					slot( nullptr )
				{
					this->Bind( funcX... );
				}

			/*
			 @	Destructor
			 */
			~Delegate() noexcept
				{
					FunctionSlotPool::
						Destroy( this->slot );
				}

			/*
			 @	Unbind
			 */
			Void Unbind() noexcept
				{
					if( this->slot )
						{
							FunctionSlotPool::
								Remove( this->slot );
						}
				}

			/*
			 @	Bind FuncX
			 */
			template< typename ...FuncX >

			Void Bind( FuncX ...funcX ) NOEXCEPT
				{
					this->Unbind();

					this->slot = FunctionSlotPool::
									Generate( funcX... );
				}
		
			/*
			 @	Bind Functor in-place
			 */
			template< class T, typename ...ArgsX >

			Void Bind( ArgsX&& ...argsX ) NOEXCEPT
				{
					this->Unbind();

					this->slot =
						FunctionSlotPool::
							template Malloc< T >(
								std::forward< ArgsX >
											( argsX )... );
				}
		
			/*
			 @	Validity
			 */
			explicit operator Bool() const noexcept
				{
					return this->slot;
				}

			/*
			 @	Operation
			 */
			Delegate& operator = ( std::nullptr_t ) noexcept
				{
					this->Unbind();

					return *this;
				}

			template< class Func >

			Delegate& operator = ( Func&& func ) noexcept
				{
					this->Bind( std::forward
								< Func >( func ) );

					return *this;
				}

			Delegate& operator = ( const Delegate& dlgt ) noexcept
				{
					this->Unbind();

					this->slot = FunctionSlotPool
								 ::Clone( dlgt.slot );

					return *this;
				}

			Delegate& operator = ( Delegate&& dlgt ) noexcept
				{
					// FC_ASSERT( this != &dlgt );

					std::swap( this->slot, dlgt.slot );

					return *this;
				}

			/*
			 @	Call

				Pre-detect the slot is null
			 */
			Retval operator() ( Args ...args ) const NOEXCEPT
				{
					FC_ASSERT( this->slot );

					return ( *this->slot )( args... );
				}

		private:
			/*	FunctionSlot instance pointer
			*/
			FunctionSlot< Retval, Args... > * slot;
		};
}



namespace fce
{
	/*
	 !	Delegate wrapper
	 
			for inheritance
	 */
	template< typename Retval,
			  typename ...Args >

	class DelegateWrapper
		{
		public:
			/*
			 @	Set handler
			 */
			template< typename ...FuncX >

			Void SetHandler( FuncX ...handler ) NOEXCEPT
				{
					this->handler.Bind( handler... );
				}

			template< class T, typename ...ArgsX >

			Void SetHandler( ArgsX&& ...argsX ) NOEXCEPT
				{
					this->handler.Bind< T >(
						std::forward< ArgsX >( argsX )... );
				}

			/*
			 @	Cancel handler
			 */
			Void SetHandler( std::nullptr_t ) noexcept
				{
					this->handler.Unbind();
				}

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( DelegateWrapper );

			~DelegateWrapper() = default;

			DelegateWrapper() = default;

		protected:
			/*	Delegate instance
			*/
			Delegate< Retval, Args... > handler;
		};
}