/*
**
**	Function slot chain.
**
		Invoker chain  |  Filter chain

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Delegate chain
	 */
	template< typename Retval,
			  typename ...Args >

	class DelegateChain final
		{
			/*	friend
			*/
			template< class Chain,
					  typename Index,
					  UInt Maximum >

			friend class DelegateGroup;

			/*	using
			*/
			using FunctionSlot = FunctionSlot
								 < Retval, Args... >;

			using FunctionSlotPool = inner::delegate::
									 FunctionSlotPool
									 < Retval, Args... >;

		public:
			/*
			 @	Constructor & Destructor
			 */
			DISABLE_COPY_MOVE( DelegateChain );

			DelegateChain() = default;

			~DelegateChain() noexcept
				{
					for( auto slot : this->listSlots )
						{
							FunctionSlotPool::Destroy( slot );
						}
				}

			/*
			 @	Empty
			 */
			Bool Empty() const noexcept
				{
					return this->listSlots.empty();
				}

			/*
			 @	Add function
			 */
			template< typename ...FuncX >

			Handle Add( FuncX ...funcX ) NOEXCEPT
				{
					FC_ASSERT_LOG(
						! this->Exists( funcX... ),
						"Add the same func handler into the delegate chain again..." );

					auto slot{ FunctionSlotPool::
								Generate( funcX... ) };

					FC_ASSERT( slot );

					this->listSlots.emplace_back( slot );

					return slot;
				}

			/*
			 @	Add functor in-place
			 */
			template< class T, typename ...ArgsX >

			Handle Add( ArgsX&& ...argsX ) NOEXCEPT
				{
					auto slot{ FunctionSlotPool::
								template Malloc< T >(
									std::forward< ArgsX >
												( argsX )... ) };

					FC_ASSERT_LOG(
						! this->Exists( slot->func ),
						"Add the same func handler into the delegate chain again..." );

					this->listSlots.emplace_back( slot );

					return slot;
				}
		
			/*
			 @	Remove function
			 */
			template< typename ...FuncX >

			Void Remove( FuncX ...funcX ) noexcept
				{
					FunctionSlot** const slot
						{
							this->Find( funcX... )
						};

					/*
					FC_ASSERT_LOG( slot,
								  "The function isn't exists..." );
					*/

					if( slot )
						{
							FunctionSlotPool::Remove( *slot );
						}
				}

			/*
			 @	Notify
			 */
			Retval Notify( Args ...args ) NOEXCEPT
				{
					UInt i{ 0 };

					UInt num{ this->listSlots.size() };

					while( i < num )
						{
							if( auto slot{ this->listSlots[ i ] } )
								{
									/*	Conditional compile of Filter
									*/
									if constexpr( std::convertible_to
													   < Retval, Bool > )
										{
											if( Retval retval{ ( *slot )
															   ( args... ) } )
												{
													return retval;
												}
										}
									/*	Conditional compile of Invoker
									*/
									else
										{
											( *slot )( args... );
										}

									++i;
								}
							else
								{
									this->listSlots.erase(
										this->listSlots.begin() + i );
									
									--num;
								}
						}

					/*	Conditional compile of Filter
					*/
					if constexpr( std::convertible_to< Bool, Retval > )
						{
							return false;
						}
				}

			/*
			 @	Has the function been added
			 */
			template< typename ...FuncX >

			Bool Exists( FuncX ...funcX ) noexcept
				{
					return this->Find( funcX... );
				}

		private:
			/*
			 @	Find function slot
			 */
			template< typename ...FuncX >

			FunctionSlot** Find( FuncX ...funcX ) noexcept
				{
					for( auto& slot : this->listSlots )
						{
							if( FunctionSlotPool::
									Compare( slot, funcX... ) )
								{
									return &slot;
								}
						}

					return nullptr;
				}
		
		private:
			/*	FunctionSlot container
			*/
			stl::vector< FunctionSlot* > listSlots;
		};


	/*
	 !	Invoker chain

			return Void
	 */
	template< typename ...Context >

	using InvokerChain = DelegateChain< Void, Context... >;

	/*
	 !	Filter chain

			return Bool
	 */
	template< typename ...Context >

	using FilterChain = DelegateChain< Bool, Context... >;
}



namespace fce
{
	/*
	 !	Delegate chain wrapper
	 
			for inheritance
	 */
	template< typename Retval,
			  typename ...Args >

	class DelegateChainWrapper
		{
		public:
			/*
			 @	Add handler
			 */
			template< typename ...FuncX >

			Handle AddHandler( FuncX ...handler ) NOEXCEPT
				{
					return this->dlgtChain.Add( handler... );
				}

			/*
			 @	Add functor handler in-place
			 */
			template< class T, typename ...ArgsX >

			Handle AddHandler( ArgsX&& ...argsX ) NOEXCEPT
				{
					return this->dlgtChain.Add< T >( argsX... );
				}
		
			/*
			 @	Remove handler
			 */
			template< typename ...FuncX >

			Void RemoveHandler( FuncX ...handler ) noexcept
				{
					this->dlgtChain.Remove( handler... );
				}

		protected:
			/*
			 @	Constructor & Destructor
			 */
			~DelegateChainWrapper() = default;

			DelegateChainWrapper() = default;

			/*
			 @	Empty
			 */
			Bool HasHandler() const noexcept
				{
					return ! this->dlgtChain.Empty();
				}

			/*
			 @	Notify handlers
			 */
			auto NotifyHandlers( Args ...args ) NOEXCEPT
				{
					return this->dlgtChain.Notify( args... );
				}

		private:
			/*	FunctionSlot container
			*/
			DelegateChain< Retval, Args... > dlgtChain;
		};


	/*
	 !	Invoker chain wrapper

			return Void
	 */
	template< typename ...Context >

	using InvokerChainWrapper = DelegateChainWrapper
								< Void, Context... >;

	/*
	 !	Filter chain wrapper

			return Bool
	 */
	template< typename ...Context >

	using FilterChainWrapper = DelegateChainWrapper
							   < Bool, Context... >;
}