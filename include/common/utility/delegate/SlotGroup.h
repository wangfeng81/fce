/*
**
**	Function slot group.
**
		Invoker group  |  Filter group

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Delegate group
	 */
	template< class Chain,
			  typename Index,
			  UInt Maximum >

	class DelegateGroup final
		{
		public:
			/*
			 @	Constructor & Destructor
			 */
			DISABLE_COPY_MOVE( DelegateGroup );

			~DelegateGroup() = default;

			DelegateGroup() = default;

			/*
			 @	Empty
			 */
			Bool Empty() const noexcept
				{
					for( const auto& chain : this->listChains )
						{
							if( ! chain.Empty() )
								{
									return false;
								}
						}

					return true;
				}

			/*
			 @	Remove function
			 */
			template< typename ...FuncX >

			Void Remove( const Index idx, FuncX ...funcX ) noexcept
				{
					this->GetChain( idx ).Remove( funcX... );
				}

			/*
			 @	Remove function for all index
			 */
			template< typename ...FuncX >

			Void Remove( FuncX ...funcX ) noexcept
				{
					for( auto& chain : this->listChains )
						{
							if( auto slot{ chain.Find( funcX... ) } )
								{
									Chain::FunctionSlotPool::Remove( *slot );
								}
						}
				}

			/*
			 @	Add function
			 */
			template< typename ...FuncX >

			Handle Add( const Index idx, FuncX ...funcX ) NOEXCEPT
				{
					return this->GetChain( idx ).Add( funcX... );
				}

			/*
			 @	Add function for all index
			 */
			template< typename ...FuncX >

			Void Add( FuncX ...funcX ) NOEXCEPT
				{
					for( auto& chain : this->listChains )
						{
							chain.Add( funcX... );
						}
				}

			/*
			 @	Add functor in-place
			 */
			template< class T, typename ...ArgsX >

			Handle Add( const Index idx, ArgsX&& ...argsX ) NOEXCEPT
				{
					return this->GetChain( idx ).Add< T >
							( std::forward< ArgsX >( argsX )... );
				}

			/*
			 @	Add functor in-place for all index
			 */
			template< class T, typename ...ArgsX >

			Void Add( ArgsX&& ...argsX ) NOEXCEPT
				{
					for( auto& chain : this->listChains )
						{
							chain.Add< T >( std::forward
											< ArgsX >( argsX )... );
						}
				}
		
			/*
			 @	Notify
			 */
			template< typename ...Context >

			auto Notify( const Index idx, Context&& ...ctx ) NOEXCEPT
				{
					return this->GetChain( idx ).Notify( std::forward
														 < Context >( ctx )... );
				}

		private:
			/*
			 @	Get chain with index
			 */
			Chain& GetChain( const Index idx ) NOEXCEPT
				{
					return this->listChains[ UInt( idx ) ];
				}

		private:
			/*	Delegate chain container
			*/
			std::array< Chain, Maximum > listChains;
		};


	/*
	 !	Invoker group

			return Void
	 */
	template< UInt Maximum,
			  typename Index,
			  typename ...Context >

	using InvokerGroup = DelegateGroup< InvokerChain
										< Context... >,
										Index, Maximum >;

	/*
	 !	Filter group

			return Bool
	 */
	template< UInt Maximum,
			  typename Index,
			  typename ...Context >

	using FilterGroup = DelegateGroup< FilterChain
									   < Context... >,
									   Index, Maximum >;
}



namespace fce
{
	/*
	 !	Delegate group wrapper
	 */
	template< class Chain,
			  typename Index,
			  UInt Maximum >

	class DelegateGroupWrapper
		{
		public:
			/*
			 @	Remove handler
			 */
			template< typename ...Handler >

			Void RemoveHandler( Handler ...handler ) noexcept
				{
					this->dlgtGroup.Remove( handler... );
				}

			/*
			 @	Add handler
			 */
			template< typename ...Handler >

			auto AddHandler( Handler ...handler ) NOEXCEPT
				{
					return this->dlgtGroup.Add( handler... );
				}

			/*
			 @	Add functor handler in-place
			 */
			template< class T, typename ...ArgsX >

			auto AddHandler( ArgsX&& ...argsX ) NOEXCEPT
				{
					return this->dlgtGroup.Add< T >(
							std::forward< ArgsX >( argsX )... );
				}
		
		protected:
			/*
			 @	Constructor & Destructor
			 */
			~DelegateGroupWrapper() = default;

			DelegateGroupWrapper() = default;

			/*
			 @	Empty
			 */
			Bool HasHandler() const noexcept
				{
					return ! this->dlgtGroup.Empty();
				}

			/*
			 @	Notify handlers
			 */
			template< typename ...Context >

			auto NotifyHandlers( const Index idx, Context&& ...ctx ) NOEXCEPT
				{
					return this->dlgtGroup.Notify( idx, std::forward
														< Context >( ctx )... );
				}

		private:
			/*	Delegate chain container
			*/
			DelegateGroup< Chain, Index, Maximum > dlgtGroup;
		};


	/*
	 !	Invoker group wrapper

			return Void
	 */
	template< UInt Maximum,
			  typename Index,
			  typename ...Context >

	using InvokerGroupWrapper = DelegateGroupWrapper< InvokerChain
													  < Context... >,
													  Index, Maximum >;

	/*
	 !	Filter group wrapper

			return Bool
	 */
	template< UInt Maximum,
			  typename Index,
			  typename ...Context >

	using FilterGroupWrapper = DelegateGroupWrapper< FilterChain
													 < Context... >,
													 Index, Maximum >;
}