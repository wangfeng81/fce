/*
**
**	Allocator.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::stl
{
	/*
	 !	STL	custom allocator
	 */
	template< class T >

	class allocator
		{
			static_assert
				(
					! std::is_const_v< T >
				);

			/*	Friends
			*/
			template< class U >

			friend class allocator;

		public:
			/*	Types
			*/
			using value_type = T;

			using size_type = std::size_t;

			using difference_type = std::ptrdiff_t;

			using is_always_equal = std::true_type;

			using propagate_on_container_move_assignment = std::true_type;

			/*
			 @	Constructor & Destructor
			 */
			~allocator() = default;

			allocator() noexcept
				:
					pool( s_MemMgmt.GetPool< sizeof( T ) >() )
				{
				}

			allocator( const allocator& a ) noexcept
				:
					pool( a.pool )
				{
				}

			allocator( allocator&& a ) noexcept
				:
					pool( a.pool )
				{
				}

			template< class U >

			allocator( const allocator< U >& other ) noexcept
				:
					pool( sizeof( T ) == sizeof( U )
						? other.pool : s_MemMgmt.GetPool< sizeof( T ) >() )
				{
				}

			template< class U >

			allocator( allocator< U >&& other ) noexcept
				:
					pool( sizeof( T ) == sizeof( U )
						? other.pool : s_MemMgmt.GetPool< sizeof( T ) >() )
				{
				}

			/*
			 @	Assignment
			 */
			allocator& operator = ( const allocator& a ) noexcept
				{
					this->pool = a.pool;

					return *this;
				}

			allocator& operator = ( allocator&& a ) noexcept
				{
					this->pool = a.pool;

					return *this;
				}

			/*
			 @	Free an array
			 */
			void deallocate( T* const p,
							 const size_type n ) NOEXCEPT
				{
					if( n == 1 )
						{
							this->pool->Free( p );
						}
					else
						{
							s_MemMgmt.Free( p, n );
						}
				}

			/*
			 @	Malloc an array
			 */
			[[nodiscard]] T* allocate( const size_type n ) noexcept
				{
					Handle p;

					if( n == 1 )
						{
							p = this->pool->Malloc();
						}
					else
						{
							p = s_MemMgmt.Malloc< T >( n );
						}

					return static_cast< T* >( p );
				}

			/*
			 @	Malloc an array at least
			 */
			[[nodiscard]] std::allocation_result< T* > allocate_at_least( const size_type n ) noexcept
				{
					constexpr size_type unit
						{
							sizeof( T )
						};

					constexpr size_type lg2
						{
							Log2< unit >::value
						};

					constexpr size_type shift
						{
							unit > ( 1 << lg2 )
							? lg2 + 1
							: lg2
						};

					const size_type size
						{
							unit * n
						};

					MemoryPool* pool_at_least
						{
							s_MemMgmt.GetPool( size )
						};

					return { reinterpret_cast< T* >( pool_at_least->Malloc() ),
							 ( ( pool_at_least->GetCellSize() - size ) >> shift ) + n };
				}

		private:
			/*	Memory pool
			*/
			MemoryPool* pool;
		};
}



/**
 !	Custom allocator
 */
namespace fce::stl
{
	/*
	 @	Operator ==
	 */
	template< class L, class R >

	[[nodiscard]] constexpr Bool operator == ( const allocator< L >&,
											   const allocator< R >& ) noexcept
		{
			return sizeof( L ) == sizeof( R );
		}

	/*
	 @	Operator !=
	 */
	template< class L, class R >

	[[nodiscard]] constexpr Bool operator != ( const allocator< L >&,
											   const allocator< R >& ) noexcept
		{
			return sizeof( L ) != sizeof( R );
		}
}



/**
 !	Configure STL allocator
 */
namespace fce::stl
{
	/*	vector
	*/
	template< class T, class A = allocator< T > >

	using vector = std::vector< T, A >;

	/*	deque
	*/
	template< class T, class A = allocator< T > >

	using deque = std::deque< T, A >;

	/*	list
	*/
	template< class T, class A = allocator< T > >

	using list = std::list< T, A >;

	/*	forward_list
	*/
	template< class T, class A = allocator< T > >

	using forward_list = std::forward_list< T, A >;

	/*	stack
	*/
	template< class T, class Container = deque< T > >

	using stack = std::stack< T, Container >;

	/*	queue
	*/
	template< class T, class Container = deque< T > >

	using queue = std::queue< T, Container >;

	/*	priority_queue
	*/
	template< class T, class Container = vector< T >,
					   class Less = std::less< typename Container::value_type > >

	using priority_queue = std::priority_queue< T, Container, Less >;

	/*	set
	*/
	template< class Key, class Less = std::less< Key > >

	using set = std::set< Key, Less, allocator< Key > >;

	/*	multiset
	*/
	template< class Key, class Less = std::less< Key > >

	using multiset = std::multiset< Key, Less, allocator< Key > >;

	/*	map
	*/
	template< class Key, class Val, class Less = std::less< Key > >

	using map = std::map< Key, Val, Less,
						  allocator< std::pair< const Key, Val > > >;

	/*	multimap
	*/
	template< class Key, class Val, class Less = std::less< Key > >

	using multimap = std::multimap< Key, Val, Less,
									allocator< std::pair< const Key, Val > > >;

	/*	unordered_set
	*/
	template< class Key, class Hash = std::hash< Key >,
						 class Equal = std::equal_to< Key > >

	using unordered_set = std::unordered_set< Key, Hash, Equal,
											  allocator< Key > >;

	/*	unordered_multiset
	*/
	template< class Key, class Hash = std::hash< Key >,
						 class Equal = std::equal_to< Key > >

	using unordered_multiset = std::unordered_multiset< Key, Hash, Equal,
														allocator< Key > >;

	/*	unordered_map
	*/
	template< class Key, class Val, class Hash = std::hash< Key >,
									class Equal = std::equal_to< Key > >

	using unordered_map = std::unordered_map< Key, Val, Hash, Equal,
											  allocator< std::pair< const Key, Val > > >;

	/*	unordered_multimap
	*/
	template< class Key, class Val, class Hash = std::hash< Key >,
									class Equal = std::equal_to< Key > >

	using unordered_multimap = std::unordered_multimap< Key, Val, Hash, Equal,
														allocator< std::pair< const Key, Val > > >;

	/*	strings
	*/
	template< Character T >

	using basic_string = std::basic_string< T, std::char_traits< T >, allocator< T > >;

	using string = basic_string< Char >;

	using wstring = basic_string< WChar >;

	using u8string = basic_string< Char8 >;

	using u16string = basic_string< Char16 >;

	using u32string = basic_string< Char32 >;
}