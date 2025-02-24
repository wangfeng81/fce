/*
**
**	Asset pool.
**
		Never allocate a null pointer

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Asset pool

			stl::unordered_map
	 */
	template< class Frnd,
			  typename T,
			  typename Key = const T* >

	class AssetPool final
		{
			/*	Friends
			*/
			friend typename T;

			friend typename Frnd;

			/*	ObjectPool
			*/
			using ObjectPool = ObjectPool
							   < T, AssetPool >;

			/*	Container
			*/
			using Container = stl::unordered_map
							  < Key, T * const >;

		public:
			/*
			 @	Iterator
			 */
			[[nodiscard]] auto end() const noexcept
				{
					return this->container.cend();
				}

			[[nodiscard]] auto begin() const noexcept
				{
					return this->container.cbegin();
				}

			/*
			 @	Access
			 */
			[[nodiscard]] UInt Count() const noexcept
				{
					return this->container.size();
				}

			[[nodiscard]] auto* operator [] ( this auto& self,
											  const Key& key ) noexcept
				{
					return self.Find( key );
				}

			/*
			 @	Exists
			 */
			[[nodiscard]] Bool Exists( const Key& key ) const noexcept
				{
					return this->container.contains( key );
				}

			[[nodiscard]] Bool Exists( const T* const obj ) const noexcept
				{
					return this->Find( obj )
						!= this->container.end();
				}

			/*
			 @	Find an asset
			 */
			[[nodiscard]] T* Find( const Key& key ) const noexcept
				{
					const auto p{ this->container.find( key ) };

					if( p != this->end() )
						{
							return p->second;
						}

					return nullptr;
				}

			/*
			 @	Query key
			 */
			[[nodiscard]] const Key* Query( const T* const obj ) const noexcept
				{
					const auto p{ this->Find( obj ) };

					if( p != this->end() )
						{
							return &p->first;
						}

					return nullptr;
				}

			/*
			 @	Request an asset

					like constructor with no param
			 */
			template< typename ...Args >

			[[nodiscard]] T* Request( const Key& key,
											Args&& ...args ) noexcept
				{
					const auto p{ this->container.find( key ) };

					if( p != this->end() )
						{
							return p->second;
						}

					return this->Generate
							(
								key,
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Generate an asset
			 
					with constructor
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate( const Key& key,
											 Args&& ...args ) NOEXCEPT

			requires requires{ T( std::forward< Args >( args )... ); }
				{
					return this->Generate_with_constructor
							(
								key,
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Generate an asset
			 
					with a result value
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate( const Key& key,
											 Args&& ...args ) NOEXCEPT
				{
					return this->Generate_with_result
							(
								key,
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Destroy an asset
			 
					with object pointer
			 */
			Void Destroy( const T* const obj ) NOEXCEPT
				{
					this->Destroy
						(
							this->Find( obj )
						);
				}

			/*
			 @	Destroy an asset
			 
					with key
			 */
			Void Destroy( const Key& key ) NOEXCEPT
				{
					this->Destroy
						(
							this->container.find( key )
						);
				}

			/*
			 @	Clear all assets
			 */
			Void Clear() noexcept
				{
					for( auto& p : this->container )
						{
							this->pool.Destroy( p.second );
						}

					this->container.clear();
				}

			/*
			 @	Rebuild an asset

					Non-Thread-Safe
					Failed return null
			 */
			template< typename ...Args >

			T* Rebuild( T* const obj,
						Args&& ...args ) NOEXCEPT
				{
					FC_ASSERT( this->Exists( obj ) );

					return this->pool.Rebuild
							(
								obj,
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Modify the key of the asset
			 */
			Void Rename( const Key& src,
						 const Key& dst ) NOEXCEPT
				{
					FC_ASSERT( ! this->Exists( dst ) );

					const auto p{ this->container.find( src ) };

					FC_ASSERT( p != this->end() );

					this->container.emplace( dst, p->second );

					this->container.erase( p );
				}

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( AssetPool );

			~AssetPool() noexcept
				{
					this->Clear();
				}

			AssetPool() = default;

			/*
			 @	Destruct all assets
			 */
			Void DestructAll() noexcept
				{
					for( auto& p : this->container )
						{
							p.second->~T();
						}

					this->container.clear();
				}

			/*
			 @	Destroy an asset with iterator
			 */
			Void Destroy( typename Container::const_iterator p ) NOEXCEPT
				{
					FC_ASSERT( p != this->end() );

					this->pool.Destroy( p->second );

					this->container.erase( p );
				}

			/*
			 @	Find an asset with object pointer
			 */
			auto Find( const T* const obj ) const noexcept
				{
					auto f
						{
							[ obj ]( Container::template
									  value_type p )
								{
									return p.second == obj;
								}
						};

					return std::find_if( this->begin(),
										 this->end(), f );
				}

		protected:
			/*
			 @	Generate an asset
			 
					with exception capture
			 */
			template< typename ...Args >

			T* Generate_with_constructor( const Key& key,
												Args&& ...args ) NOEXCEPT
				{
					FC_ASSERT( ! this->Exists( key ) );

					Handle p{ this->pool.Malloc() };

					try {
							T* const obj
								{
									::new( p ) T
										(
											std::forward< Args >( args )...
										)
								};

							this->container.emplace( key, obj );

							return obj;
						}
					catch( ... )
						{
							this->pool.Free( p );

							return nullptr;
						}
				}

			/*
			 @	Generate an asset
			 
					with no noexcept
			 */
			template< typename ...Args >

			T* Generate_with_constructor( const Key& key,
												Args&& ...args ) NOEXCEPT

			requires requires{ { T( std::forward< Args >( args )... ) } noexcept; }
				{
					FC_ASSERT( ! this->Exists( key ) );

					T* const obj
						{
							::new( this->pool.Malloc() ) T
								(
									std::forward< Args >( args )...
								)
						};

					this->container.emplace( key, obj );

					return obj;
				}

			/*
			 @	Generate an asset
			 
					with a result value
			 */
			template< typename ...Args >

			requires requires( RESULT& res,
							   Args&& ...args )
				{
					T( res, std::forward< Args >( args )... );
				}

			T* Generate_with_result( const Key& key,
										   Args&& ...args ) NOEXCEPT
				{
					RESULT res{ RESULT::SUCCESS };

					T* const obj
						{
							this->Generate_with_constructor
								(
									key,
									res,
									std::forward< Args >( args )...
								)
						};

					if( res == RESULT::SUCCESS )
						{
							return obj;
						}
					else
						{
							// The constructor of the T should return immediately when RESULT error...
							FC_ASSERT( obj );

							this->Destroy( obj );

							return nullptr;
						}
				}

		private:
			/*	Object pool
			*/
			ObjectPool pool;

			/*	Asset container
			*/
			Container container;
		};


	/*
	 @	Requirements of asset pool
	 */
	#define ASSET_POOL( F, T, K )	friend class F;										\
																						\
									friend ::fce::AssetPool< class F, T, K >;			\
																						\
									friend ::fce::ObjectPool< T,						\
											::fce::AssetPool< class F, T, K > >
}



namespace fce
{
	/*
	 !	Asset pool

			stl::forward_list
	 */
	template< class Frnd,
			  class T >

	class AssetPool< Frnd, T, const T* > final
		{
			/*	Friends
			*/
			friend typename T;

			friend typename Frnd;

			/*	ObjectPool
			*/
			using ObjectPool = ObjectPool
							   < T, AssetPool >;

			/*	Container
			*/
			using Container = stl::forward_list< T* >;

		public:
			/*
			 @	Iterator
			 */
			[[nodiscard]] auto end() const noexcept
				{
					return this->container.cend();
				}

			[[nodiscard]] auto begin() const noexcept
				{
					return this->container.cbegin();
				}

			/*
			 @	Exists
			 */
			[[nodiscard]] Bool Exists( const T* const obj ) const noexcept
				{
					for( const T* p : this->container )
						{
							if( p == obj )
								{
									return true;
								}
						}

					return false;
				}

			/*
			 @	Generate an object
			 
					with constructor
					
						( highest priority )
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate( Args&& ...args ) noexcept

			requires requires{ T( std::forward< Args >( args )... ); }
				{
					return this->Generate_with_constructor
							(
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Generate an object
			 
					with a result value

						( secondary priority )
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate( Args&& ...args ) NOEXCEPT
				{
					return this->Generate_with_result
							(
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Rebuild an object

					Non-Thread-Safe
					Failed return null
			 */
			template< typename ...Args >

			T* Rebuild( T* const obj,
						Args&& ...args ) NOEXCEPT
				{
					FC_ASSERT( this->Exists( obj ) );

					T* const obj2
						{
							this->pool.Rebuild
								(
									obj,
									std::forward< Args >( args )...
								)
						};

					if( ! obj2 )
						{
							this->pool.Free( obj );

							this->container.remove( obj );
						}

					return obj2;
				}

			/*
			 @	Destroy an object
			 */
			Void Destroy( T* const obj ) NOEXCEPT
				{
					FC_ASSERT( this->Exists( obj ) );

					this->container.remove( obj );

					this->pool.Destroy( obj );
				}

			/*
			 @	Clear all objects
			 */
			Void Clear() noexcept
				{
					for( T* obj : this->container )
						{
							this->pool.Destroy( obj );
						}

					this->container.clear();
				}

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( AssetPool );

			~AssetPool() noexcept
				{
					this->Clear();
				}

			AssetPool() = default;

			/*
			 @	Destruct all objects
			 */
			Void DestructAll() noexcept
				{
					for( T* obj : this->container )
						{
							obj->~T();
						}

					this->container.clear();
				}

			/*
			 @	Generate an object
			 
					with constructor ( noexcept )
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate_with_constructor( Args&& ...args ) noexcept

			requires requires{ { T( std::forward< Args >( args )... ) } noexcept; }
				{
					T* const obj
						{
							::new( this->pool.Malloc() ) T
								(
									std::forward< Args >( args )...
								)
						};

					this->container.emplace_front( obj );

					return obj;
				}

			/*
			 @	Try to generate an object
			 
					with constructor ( except )
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate_with_constructor( Args&& ...args ) noexcept
				{
					Handle p{ this->pool.Malloc() };

					try {
							T* const obj
								{
									::new( p ) T
										(
											std::forward< Args >( args )...
										)
								};

							this->container.emplace_front( obj );

							return obj;
						}
					catch( ... )
						{
							this->pool.Free( p );

							return nullptr;
						}
				}

			/*
			 @	Generate an object
			 
					with a result value
			 */
			template< typename ...Args >

			requires requires( RESULT& res,
							   Args&& ...args )
				{
					T( res, std::forward< Args >( args )... );
				}

			[[nodiscard]] T* Generate_with_result( Args&& ...args ) NOEXCEPT
				{
					RESULT res{ RESULT::SUCCESS };

					T* const obj
						{
							this->Generate_with_constructor
								(
									res,
									std::forward< Args >( args )...
								)
						};

					if( res == RESULT::SUCCESS )
						{
							return obj;
						}
					else
						{
							// The constructor of the T should return immediately when RESULT error...
							FC_ASSERT( obj );

							this->Destroy( obj );

							return nullptr;
						}
				}

		private:
			/*	Object pool
			*/
			ObjectPool pool;

			/*	Asset container
			*/
			Container container;
		};


	/*
	 @	Requirements of asset pool
	 */
	#define ASSET_POOL2( F, T )	friend class F;							\
																		\
								friend fce::AssetPool< class F, T >;	\
																		\
								friend fce::ObjectPool< T,				\
										fce::AssetPool< class F, T > >
}