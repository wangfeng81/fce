/*
**
**	Raw object.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Raw object
	 */
	template< class T,
			  typename Frnd = MemoryManagement >

	class RawObject final
		{
			using Value = T;

			/*	Friends
			*/
			friend Buffer;

			friend typename T;

			friend typename Frnd;

		public:
			/*
			 @	Get object pointer
			 */
			[[nodiscard]] auto* Get( this auto& self ) noexcept
				{
					return self.pointer;
				}

			/*
			 @	Get object reference
			 */
			[[nodiscard]] auto& GetRef( this auto& self ) noexcept
				{
					FC_ASSERT( self.pointer );

					return *self;
				}

			/*
			 @	Conversion object pointer
			 */
			[[nodiscard]] operator T*( this auto& self ) noexcept
				{
					return this->Get();
				}

			[[nodiscard]] operator const T*() const noexcept
				{
					return this->Get();
				}

			/*
			 @	Conversion object reference
			 */
			[[nodiscard]] operator T&() noexcept
				{
					return this->GetRef();
				}

			[[nodiscard]] operator const T&() const noexcept
				{
					return this->GetRef();
				}

			/*
			 @	Direct access object
			 */
			[[nodiscard]] T* operator -> () noexcept
				{
					return this->Get();
				}

			[[nodiscard]] const T* operator -> () const noexcept
				{
					return this->Get();
				}

			/*
			 @	Direct access raw object reference with no assert
			 */
			[[nodiscard]] T& operator * () noexcept
				{
					return reinterpret_cast< T& >( this->data );
				}

			[[nodiscard]] const T& operator * () const noexcept
				{
					return reinterpret_cast< const T& >( this->data );
				}

			/*	Is valid
			*/
			[[nodiscard]] explicit operator Bool() const noexcept
				{
					return this->pointer;
				}

		public:
			/*
			 @	Destruct
			 */
			Void Destruct() noexcept
				{
					if( this->pointer )
						{
							this->pointer->~T();

							this->pointer = nullptr;
						}
				}

			/*
			 @	Construct
			 */
			template< typename ...Args >

			T* Construct( Args&& ...args ) noexcept

			requires requires{ { T( std::forward< Args >( args )... ) } noexcept; }
				{
					//FC_ASSERT( ! this->pointer );

					return this->pointer = ::new( &this->data ) T
											(
												std::forward< Args >( args )...
											);
				}

			/*
			 @	Try to construct
			 */
			template< typename ...Args >

			T* Construct( Args&& ...args ) noexcept
				{
					try {
							//FC_ASSERT( ! this->pointer );

							return this->pointer = ::new( &this->data ) T
													(
														std::forward< Args >( args )...
													);
						}
					catch( ... )
						{
							return this->pointer = nullptr;
						}
				}

			/*
			 @	Rebuild
			 */
			template< typename ...Args >

			T* Rebuild( Args&& ...args ) noexcept
				{
					this->Destruct();

					return this->Construct
								(
									std::forward< Args >( args )...
								);
				}

		protected:
			/*
			 @	Destructor
			 */
			~RawObject() noexcept
				{
					#ifndef FC_RELEASE
						{
							Destruct();
						}
					#else
						{
							if( this->pointer )
								{
									this->pointer->~T();
								}
						}
					#endif
				}

			/*
			 @	Constructor
			 */
			RawObject() noexcept
				:
					pointer( nullptr )
				{
				}

			/*
			 @	Constructor	object
			 */
			RawObject( const T& obj ) noexcept
									( noexcept( T( obj ) ) )
				{
					this->Construct( obj );
				}

			RawObject( T&& obj ) noexcept
				{
					this->Construct( std::move( obj ) );
				}

			/*
			 @	Constructor move
			 */
			RawObject( Template< RawObject > auto&& rawObj ) noexcept
				{
					if( rawObj )
						{
							this->Construct( std::move( *rawObj ) );
						}
					else
						{
							this->pointer = nullptr;
						}
				}

			/*
			 @	Constructor copy
			 */
			RawObject( const Template< RawObject > auto& rawObj ) noexcept
																( noexcept( T( *rawObj ) ) )
				{
					if( rawObj )
						{
							this->Construct( *rawObj );
						}
					else
						{
							this->pointer = nullptr;
						}
				}

			/*
			 @	Constructor range
			 */
			template< std::ranges::range Range >

			RawObject( Range&& range ) noexcept
									 ( noexcept( T( range ) ) )
				{
					if( range.empty() )
						{
							this->pointer = nullptr;
						}
					else
						{
							this->Construct( std::forward< Range >( range ) );
						}
				}

			/*	Assignment from object
			*/
			RawObject& operator = ( const T& obj ) noexcept
												 ( noexcept( **this = obj ) )
				{
					this->GetRef() = obj;

					return *this;
				}

			RawObject& operator = ( T&& obj ) noexcept
				{
					this->GetRef() = std::move( obj );

					return *this;
				}

			/*	Assignment from raw object
			*/
			RawObject& operator = ( const RawObject& rawObj ) noexcept
															( noexcept( **this = *rawObj ) )
				{
					this->GetRef() = rawObj.GetRef();

					return *this;
				}

			RawObject& operator = ( RawObject&& rawObj ) noexcept
				{
					this->GetRef() = std::move( rawObj.GetRef() );

					return *this;
				}

		private:
			/*	Object memory
			*/
			Byte data[ sizeof( T ) ];

			/*	Object pointer
			*/
			T* pointer;
		};


	/*	concept		Raw_object
	 */
	template< typename T >

	concept Raw_object = Template< T, RawObject >;


	/*
	 @	Requirements of raw object
	 */
	#define RAW_OBJECT( T, F )	friend class F;						\
																	\
								friend fce::RawObject< T, class F >
}