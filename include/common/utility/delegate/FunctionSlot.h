/*
**
**	Function slot.
**
		Free function slot
		Member function slot
		General function slot

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Predefine
	 */
	template< typename Retval,
			  typename ...Args >
	class Delegate;

	template< typename Retval,
			  typename ...Args >
	class DelegateChain;

	template< class Chain,
			  typename Index,
			  UInt Maximum >
	class DelegateGroup;

	namespace inner::delegate
	{
		template< typename Retval,
				  typename ...Args >
		class FunctionSlotPool;
	}
}


namespace fce
{
	/*
	 !	 Base of function slot
	 */
	template< typename Retval,
			  typename ...Args >

	class FunctionSlot
		:
		public Object
		{
			/*	Friends
			*/
			using FunctionSlotPool = inner::delegate::
									 FunctionSlotPool
									 < Retval, Args... >;

			POLYMORPHIC_POOL( FunctionSlot,
							  FunctionSlotPool );

		protected:
			/*
			 @	Constructor & Destructor
			 */
			FunctionSlot() = default;

			virtual ~FunctionSlot() = default;

			/*
			 @	Clone
			 */
			virtual FunctionSlot* Clone() const noexcept = 0;

		public:
			/*
			 @	Invoking interface
			 */
			virtual Retval operator() ( Args... ) NOEXCEPT = 0;
		};
}



namespace fce
{
	/*
	 !	General function slot
	 */
	template< typename Func,
			  typename Retval,
			  typename ...Args >

	class GeneralFunctionSlot final
		:
		public FunctionSlot
			   < Retval, Args... >
		{
			/*	Friends
			*/
			using FunctionSlotPool = inner::delegate::
									 FunctionSlotPool
									 < Retval, Args... >;

			POLYMORPHIC_POOL( GeneralFunctionSlot,
							  FunctionSlotPool );

		public:
			/*
			 @	Call
			 */
			Retval operator() ( Args ...args ) NOEXCEPT override
				{
					return this->func( args... );
				}
			
		protected:
			/*
			 @	Destructor & Constructor
			 */
			~GeneralFunctionSlot() override = default;

			template< typename ...ArgsX >

			explicit GeneralFunctionSlot( ArgsX&& ...argsX ) NOEXCEPT

			requires requires
				{
					Func( std::forward< ArgsX >( argsX )... );
				}
				:
					func( std::forward< ArgsX >( argsX )... )
				{
				}

			/*
			 @	Clone
			 */
			GeneralFunctionSlot* Clone() const noexcept override
				{
					return ::new( this->memoryPool->Malloc() )
							GeneralFunctionSlot( this->func );
				}

		private:
			/*	function object
			*/
			Func func;
		};
}



namespace fce
{
	/*
	 !	Free function slot
	 */
	template< typename Retval,
			  typename ...Args >

	class FreeFunctionSlot final
		:
		public FunctionSlot
			   < Retval, Args... >
		{
			/*	Friends
			*/
			using FunctionSlotPool = inner::delegate::
									 FunctionSlotPool
									 < Retval, Args... >;

			POLYMORPHIC_POOL( FreeFunctionSlot,
							  FunctionSlotPool );

			/*	Func
			*/
			using Func = Retval( * )( Args... );

		public:
			/*
			 @	Call
			 */
			Retval operator() ( Args ...args ) NOEXCEPT override
				{
					return this->func( args... );
				}
			
		protected:
			/*
			 @	Destructor & Constructor
			 */
			~FreeFunctionSlot() override = default;

			explicit FreeFunctionSlot( Func func ) noexcept
				:
					func( func )
				{
					FC_ASSERT( func );
				}

			/*
			 @	Clone
			 */
			FreeFunctionSlot* Clone() const noexcept override
				{
					return ::new( this->memoryPool->Malloc() )
							FreeFunctionSlot( this->func );
				}
			
		private:
			/*	Function pointer
			*/
			Func func;
		};
}



namespace fce
{
	/*
	 !	Member function slot
	 */
	template< class Obj,
			  typename Retval,
			  typename ...Args >

	class MemberFunctionSlot final
		:
		public FunctionSlot
			   < Retval, Args... >
		{
			/*	Friends
			*/
			using FunctionSlotPool = inner::delegate::
									 FunctionSlotPool
									 < Retval, Args... >;

			POLYMORPHIC_POOL( MemberFunctionSlot,
							  FunctionSlotPool );

			/*	Func
			*/
			using Func = Retval( Obj::* )( Args... );

		public:
			/*
			 @	Call
			 */
			Retval operator() ( Args ...args ) NOEXCEPT override
				{
					return ( obj->*func )( args... );
				}
			
		protected:
			/*
			 @	Destructor & Constructor
			 */
			~MemberFunctionSlot() override = default;

			explicit MemberFunctionSlot( Obj* obj, Func func ) noexcept
				:
					obj( obj ), func( func )
				{
					FC_ASSERT( obj && func );
				}

			/*
			 @	Clone
			 */
			MemberFunctionSlot* Clone() const noexcept override
				{
					return ::new( this->memoryPool->Malloc() )
							MemberFunctionSlot( this->obj,
												this->func );
				}

		private:
			/*	Object and function pointer
			*/
			Obj* obj; Func func;
		};
}



namespace fce::inner::delegate
{
	/*
	 !	Function slot pool

			Lower-layer development
			Not for application layer
	 */
	template< typename Retval,
			  typename ...Args >

	class FunctionSlotPool final
		{
			/*	Friends
			*/
			friend Delegate
				   < Retval, Args... >;

			friend DelegateChain
				   < Retval, Args... >;

			template< class Chain,
					  typename Index,
					  UInt Maximum >

			friend class DelegateGroup;

			/*	using FunctionSlot
			*/
			using FunctionSlot = FunctionSlot
								 < Retval, Args... >;

			/*	using PolymorphicPool
			*/
			using PolymorphicPool = PolymorphicPool
									< FunctionSlotPool >;

		protected:
			/*
			 @	Clone
			 */
			static FunctionSlot* Clone( const FunctionSlot* slot ) noexcept
				{
					if( slot )
						{
							FunctionSlot* s{ slot->Clone() };

							s->memoryPool = slot->memoryPool;

							return s;
						}

					return nullptr;
				}

			/*
			 @	Remove
			 */
			static Void Remove( FunctionSlot*& slot ) noexcept
				{
					PolymorphicPool::Destroy( slot );

					slot = nullptr;
				}

			/*
			 @	Destroy
			 */
			static Void Destroy( FunctionSlot* slot ) noexcept
				{
					if( slot )
						{
							PolymorphicPool::Destroy( slot );
						}
				}

			/*
			 @	Generate  GeneralFunction
			 */
			template< class Func >

			static auto Generate( Func func ) NOEXCEPT
				{
					return PolymorphicPool::template Generate
							<
								GeneralFunctionSlot
								< Func, Retval, Args... >
							>
							( func );
				}
		
			/*
			 @	Generate  Functor in-place
			 */
			template< class T, typename ...ArgsX >

			static auto Generate( ArgsX&& ...argsX ) NOEXCEPT
				{
					return PolymorphicPool::template Generate
							<
								GeneralFunctionSlot
								< T, Retval, Args... >
							>
							( std::forward< ArgsX >( argsX )... );
				}
		
			/*
			 @	Generate  FreeFunction
			 */
			static auto Generate( Retval( *func )( Args... ) ) noexcept
				{
					return PolymorphicPool::template Generate
							<
								FreeFunctionSlot
								< Retval, Args... >
							>
							( func );
				}
		
			/*
			 @	Generate  MemberFunction
			 */
			template< class Obj >

			static auto Generate( Obj* obj, Retval( Obj::*func )
												  ( Args... ) ) noexcept
				{
					return PolymorphicPool::template Generate
							<
								MemberFunctionSlot
								< Obj, Retval, Args... >
							>
							( obj, func );
				}
		
			/*
			 @	Generate  MemberFunction with the object reference
			 */
			template< class Obj >

			static auto Generate( Obj& obj, Retval( Obj::*func )
												  ( Args... ) ) noexcept
				{
					return this->Generate( &obj, func );
				}
		
			/*
			 @	Compare Handle
			 */
			static Bool Compare( FunctionSlot* slot,
								 const Handle handle ) NOEXCEPT
				{
					FC_ASSERT( handle );

					return slot == handle;
				}
		
			/*
			 @	Compare GeneralFunctionSlot
			 */
			template< class Func >

			static Bool Compare( FunctionSlot* slot,
								 const Func& func ) NOEXCEPT
				{
					if( auto s = dynamic_cast
								< GeneralFunctionSlot
								< Func, Retval, Args... > * >
								( slot ) )
						{
							return Equal( s->func, func );
						}

					return false;
				}
		
			/*
			 @	Compare FreeFunctionSlot
			 */
			static Bool Compare( FunctionSlot* slot,
								 Retval( *func )( Args... ) ) NOEXCEPT
				{
					FC_ASSERT( func );

					if( auto s = dynamic_cast
								< FreeFunctionSlot
								< Retval, Args... > * >
								( slot ) )
						{
							return s->func == func;
						}

					return false;
				}
		
			/*
			 @	Compare MemberFunctionSlot
			 */
			template< class Obj >

			static Bool Compare( FunctionSlot* slot,
								 Obj* obj, Retval( Obj::*func )
												 ( Args... ) ) NOEXCEPT
				{
					FC_ASSERT( obj && func );

					if( auto s = dynamic_cast
								< MemberFunctionSlot
								< Obj, Retval, Args... > * >
								( slot ) )
						{
							return s->obj == obj
								&& s->func == func;
						}

					return false;
				}
		};
}