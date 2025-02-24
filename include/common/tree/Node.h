/*
**
**	Node.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce::inner::tree
{
	/*
	 !	Tree node
	 */
	template< class T,
			  Bool EVENT = true >

	class FC_API Node
		{
			/*	Node iterator
			*/
			using NodeGroup = stl::vector< T* >;

			using Iterator = NodeGroup::const_iterator;

		public:
			/*
			 @	Access
			 */
			FC_INLINE [[nodiscard]] auto end() const noexcept
				{
					return this->children.cend();
				}

			FC_INLINE [[nodiscard]] auto rend() const noexcept
				{
					return this->children.crend();
				}

			FC_INLINE [[nodiscard]] auto begin() const noexcept
				{
					return this->children.cbegin();
				}

			FC_INLINE [[nodiscard]] auto rbegin() const noexcept
				{
					return this->children.crbegin();
				}

			FC_INLINE [[nodiscard]] auto& operator [] ( this auto& self,
														const UInt idx ) NOEXCEPT
				{
					FC_ASSERT( idx < self.children.size() );

					return std::forward_like< decltype( self ) >
							(
								*self.children[ idx ]
							);
				}

			/*
			 @	Travel
			 */
			template< Bool RECURSIVE = false >

			auto Travel( this auto&& self,
							  auto&& func ) NOEXCEPT
				->
					decltype( func( self ) )
				{
					using Retval = decltype( func( self ) );

					constexpr bool boolRetval
						{
							std::is_convertible_v< Retval, Bool >
						};

					for( T* const child : self.children )
						{
							if constexpr( RECURSIVE )
								{
									if constexpr( boolRetval )
										{
											Retval retval
												{
													child->Travel< true >( func )
												};

											if( retval )
												{
													return retval;
												}
										}
									else
										{
											child->Travel< true >( func );
										}
								}
							else
								{
									if constexpr( boolRetval )
										{
											if( Retval retval{ func( *child ) } )
												{
													return retval;
												}
										}
									else
										{
											func( *child );
										}
								}
						}

					return func( self );
				}

		public:
			/*
			 @	Is root
			 */
			FC_INLINE [[nodiscard]] Bool isRoot() const noexcept
				{
					return ! this->parent;
				}

			/*
			 @	Get root
			 */
			[[nodiscard]] auto& GetRoot( this auto& self ) noexcept
				{
					auto* p{ &self };

					while( p->parent )
						{
							p = p->parent;
						}

					return *p;
				}

			/*
			 @	Get Parent
			 */
			FC_INLINE [[nodiscard]] auto* GetParent( this auto& self ) noexcept
				{
					return &std::forward_like< decltype( self ) >
							(
								*self.parent
							);
				}

			/*
			 @	Get child index
			 */
			[[nodiscard]] UInt GetIndex( const T* const child ) const NOEXCEPT
				{
					const auto p{ this->QueryChild( child ) };

					FC_ASSERT( p != this->end() );

					return p - this->begin();
				}

			FC_INLINE [[nodiscard]] UInt GetIndex( const T& child ) const NOEXCEPT
				{
					return this->GetIndex( &child );
				}

		public:
			/*
			 @	Clear children
			 */
			Void ClearChildren() noexcept
				{
					for( T* const child : this->children )
						{
							pool.Destroy( child );
						}

					this->children.clear();
				}

			/*
			 @	Sort children
			 */
			FC_INLINE Void SortChildren( auto&& compare ) NOEXCEPT
				{
					stl::sort( this->children, compare );
				}

			/*
			 @	Count children
			 */
			template< Bool RECURSIVE = false >

			[[nodiscard]] UInt CountChildren() const noexcept
				{
					UInt count{ this->children.size() };

					if constexpr( RECURSIVE )
						{
							for( const T* const child : this->children )
								{
									count += child->CountChildren< true >();
								}
						}

					return count;
				}

			/*
			 @	Has child
			 */
			template< Bool RECURSIVE = false >

			[[nodiscard]] Bool hasChild( const T* const child ) const noexcept
				{
					if constexpr( RECURSIVE )
						{
							for( const T* p{ child->parent }; p; p = p->parent )
								{
									if( p == this )
										{
											return true;
										}
								}

							return false;
						}
					else
						{
							return child->parent == this;
						}
				}

			template< Bool RECURSIVE = false >

			FC_INLINE [[nodiscard]] Bool hasChild( const T& child ) const noexcept
				{
					return this->hasChild< RECURSIVE >( &child );
				}

		public:
			/*
			 @	Set parent
			 */
			FC_INLINE Void SetParent( this auto& self,
									  T& parent,
									  T& child ) NOEXCEPT
				{
					self.SetParent( &parent, &child );
				}

			Void SetParent( this auto& self,
							T* const parent,
							T* const child ) NOEXCEPT
				{
					FC_ASSERT( parent && child );

					self.EraseChild( child );

					parent->PushChild( child );
				}

			Void SetParent( this auto& self,
							T* const parent,
							const UInt child ) NOEXCEPT
				{
					self.SetParent( parent,
									&self[ child ] );
				}

			/*
			 @	Move begin
			 */
			FC_INLINE Void MoveBegin( this auto& self,
									  const T& child ) NOEXCEPT
				{
					self.MoveBegin( &child );
				}

			FC_INLINE Void MoveBegin( this auto& self,
									  const UInt idx ) NOEXCEPT
				{
					self.MoveChild( 0, idx );
				}

			Void MoveBegin( this auto& self,
							const T* const child ) NOEXCEPT
				{
					self.MoveBegin( self.GetIndex( child ) );
				}

			/*
			 @	Move end
			 */
			FC_INLINE Void MoveEnd( this auto& self,
									const T& child ) NOEXCEPT
				{
					self.MoveEnd( &child );
				}

			Void MoveEnd( this auto& self,
						  const T* const child ) NOEXCEPT
				{
					self.MoveEnd( self.GetIndex( child ) );
				}

			Void MoveEnd( this auto& self,
						  const UInt idx ) NOEXCEPT
				{
					self.MoveChild( self.children.size() - 1,
									idx );
				}

			/*
			 @	Move up
			 */
			FC_INLINE Void MoveUp( this auto& self,
								   const T& child ) NOEXCEPT
				{
					self.MoveUp( &child );
				}

			Void MoveUp( this auto& self,
						 const T* const child ) NOEXCEPT
				{
					self.MoveUp( self.GetIndex( child ) );
				}

			Void MoveUp( this auto& self,
						 const UInt idx ) NOEXCEPT
				{
					FC_ASSERT( idx < self.children.size() );

					const UInt idx2{ idx - 1 };

					if( idx )
						{
							std::swap( self.children[ idx ],
									   self.children[ idx2 ] );

							if constexpr( EVENT )
								{
									self.NotifyHandlers( T::EVENT::MOVE_CHILD,
														 self[ idx2 ] );
								}
						}
				}

			/*
			 @	Move down
			 */
			FC_INLINE Void MoveDown( this auto& self,
									 const T& child ) NOEXCEPT
				{
					self.MoveDown( &child );
				}

			Void MoveDown( this auto& self,
						   const T* const child ) NOEXCEPT
				{
					self.MoveDown( self.GetIndex( child ) );
				}

			Void MoveDown( this auto& self,
						   const UInt idx ) NOEXCEPT
				{
					FC_ASSERT( idx < self.children.size() );

					const UInt idx2{ idx + 1 };

					if( idx2 < self.children.size() )
						{
							std::swap( self.children[ idx ],
									   self.children[ idx2 ] );

							if constexpr( EVENT )
								{
									self.NotifyHandlers( T::EVENT::MOVE_CHILD,
														 self[ idx2 ] );
								}
						}
				}

			/*
			 @	Move child
			 */
			FC_INLINE Void MoveChild( this auto& self,
									  const T& dst,
									  const T& src ) NOEXCEPT
				{
					self.MoveChild( &dst, &src );
				}

			Void MoveChild( this auto& self,
							const T* const dst,
							const T* const src ) NOEXCEPT
				{
					self.MoveChild( this->GetIndex( dst ),
									this->GetIndex( src ) );
				}

			Void MoveChild( this auto& self,
							const UInt dstIdx,
							const UInt srcIdx ) NOEXCEPT
				{
					FC_ASSERT( dstIdx < self.children.size()
							&& srcIdx < self.children.size() );

					T** dst{ self.children.data() + dstIdx };

					T** src{ self.children.data() + srcIdx };

					T* const child{ *src };

					if( srcIdx > dstIdx )
						{
							while( src != dst )
								{
									*src-- = *( src - 1 );
								}
						}
					else
						{
							while( src != dst )
								{
									*src++ = *( src + 1 );
								}
						}

					self.children[ dstIdx ] = child;

					if constexpr( EVENT )
						{
							self.NotifyHandlers( T::EVENT::MOVE_CHILD,
												 *child );
						}
				}

		public:
			/*
			 @	Remove child
			 */
			Void RemoveChild( this auto& self,
							  T* const child ) NOEXCEPT
				{
					self.EraseChild( child );

					pool.Destroy( child );
				}

			FC_INLINE Void RemoveChild( this auto& self,
										T& child ) NOEXCEPT
				{
					self.RemoveChild( &child );
				}

			FC_INLINE Void RemoveChild( this auto& self,
										const UInt idx ) NOEXCEPT
				{
					self.RemoveChild( self[ idx ] );
				}

			/*
			 @	Add child
			 */
			template< typename ...Args >

			FC_INLINE T& AddChild( this auto& self,
								   Args&& ...args ) NOEXCEPT

			requires requires{ T( &self,
								  std::forward< Args >( args )... ); }
				{
					T* const child
						{
							self.NewChild
								(
									std::forward< Args >( args )...
								)
						};

					FC_ASSERT( child );

					return *child;
				}

			/*
			 @	New child
			 */
			template< typename ...Args >

			T* NewChild( this auto& self,
						 Args&& ...args ) NOEXCEPT

			requires requires{ T( &self,
								  std::forward< Args >( args )... ); }
				{
					T* const child
						{
							pool.Generate
								(
									&self,
									std::forward< Args >( args )...
								)
						};

					if constexpr( ! noexcept( T( &self,
												 std::forward< Args >( args )... ) ) )
						{
							if( ! child )
								{
									return nullptr;
								}
						}

					self.children.emplace_back( child );

					if constexpr( EVENT )
						{
							self.NotifyHandlers( T::EVENT::ADD_CHILD,
												 *child );
						}

					return child;
				}

		protected:
			/*
			 @	Assignment copy
			 */
			Node& operator = ( this auto& self,
							   const Node& node ) NOEXCEPT
				{
					self.ClearChildren();

					self.CopyChildren( node );

					return self;
				}

			/*
			 @	Assignment move
			 */
			Node& operator = ( this auto& self,
							   Node&& node ) noexcept
				{
					self.MoveChildren( std::move( node ) );

					return self;
				}

			/*
			 @	Move children
			 */
			Void MoveChildren( this auto& self,
							   Node&& node ) noexcept
				{
					self.children = std::move( node.children );

					for( T* const child : self.children )
						{
							child->parent = &self;
						}
				}

			/*
			 @	Copy children
			 */
			Void CopyChildren( this auto& self,
							   const Node& node ) NOEXCEPT
				{
					for( const T* const child : node.children )
						{
							self.NewChild( *child );
						}
				}

			/*
			 @	Deserialize children
			 */
			const Buffer& DeserializeChildren( this auto& self,
											   const Buffer& buf )
				{
					UInt32 count;

					buf >> count;

					self.children.reserve( count );

					while( count-- )
						{
							self.NewChild( buf );
						}

					return buf;
				}

			/*
			 @	Serialize children
			 */
			FC_INLINE Buffer& SerializeChildren( Buffer& buf ) const NOEXCEPT
				{
					return buf << this->children;
				}

		protected:
			/*
			 !	Editor mode
			 */
			#ifdef FC_EDITOR

				/*	Friends
				*/
				XML_ARCHIVE_DECLARATION

				/*
				 @	Deserialize children xml
				 */
				Void DeserializeChildren( this auto& self,
										  const Xml::Deserialization& des ) NOEXCEPT
					{
						auto f = [ &self ]( const Xml::Deserialization& deser,
											const std::size_t index ) NOEXCEPT
							{
								self.NewChild( deser );
							};

						des.Travel( f, "Child" );
					}

				/*
				 @	Serialize children xml
				 */
				FC_INLINE Void SerializeChildren( Xml::Serialization& ser ) const NOEXCEPT
					{
						ser.Serialize( this->children, "Child" );
					}

			#endif

		protected:
			/*
			 @	Query child
			 */
			FC_INLINE auto QueryChild( const T* const child ) const noexcept
				{
					return stl::find( this->children, child );
				}

			/*
			 @	Push child
			 */
			FC_INLINE Void PushChild( this auto& self,
									  T* const child ) NOEXCEPT
				{
					child->parent = &self;

					self.children.emplace_back( child );

					if constexpr( EVENT )
						{
							self.NotifyHandlers( T::EVENT::ADD_CHILD,
												 *child );
						}
				}

			/*
			 @	Erase child
			 */
			Void EraseChild( this auto& self,
							 T* const child ) NOEXCEPT
				{
					const auto p{ self.QueryChild( child ) };

					FC_ASSERT( p != self.end() );

					if constexpr( EVENT )
						{
							self.NotifyHandlers( T::EVENT::REMOVE_CHILD,
												 *child );
						}

					self.children.erase( p );
				}

		protected:
			/*
			 @	Destructor
			 */
			virtual ~Node() noexcept
				{
					this->ClearChildren();
				}

			/*
			 @	Constructor
			 */
			DISABLE_COPY_MOVE( Node );

			Node() noexcept
				:
					parent( nullptr )
				{
				}

			Node( T* const parent ) noexcept
				:
					parent( parent )
				{
				}

		protected:
			/*	Parent & children
			*/
			T* parent;

			NodeGroup children;

			/*	T pool
			*/
			static ObjectPool< T, Node > pool;

			/*	Archive name
			*/
			static constexpr inline char ARCHIVE_NAME[] = "TreeNode";
		};


	/*
	 @	Object pool
	 */
	template< class T, Bool E >

	ObjectPool< T, Node< T, E > > Node< T, E >::pool;
}



namespace fce
{
	/*
	 @	TreeNode
	 */
	template< class T, Bool E = true >

	using TreeNode = inner::tree::Node< T, E >;
}