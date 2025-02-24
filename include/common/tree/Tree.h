/*
**
**	Tree.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	template< class T >

	class FC_API Tree final
		:
		public TreeNode< Tree< T >, false >
		{
			/*	Allocated by object pool
			*/
			using Node = TreeNode< Tree, false >;

			OBJECT_POOL( Tree, Node )

		public:
			/*
			 @	Assignment copy & move
			 */
			Tree& operator = ( const Tree& tree ) NOEXCEPT
				{
					this->element = tree.element;

					Node::operator = ( tree );

					return *this;
				}

			Tree& operator = ( Tree&& tree ) noexcept
				{
					this->element = std::move( tree.element );

					Node::operator = ( std::move( tree ) );

					return *this;
				}

			/*
			 @	Serialize & Deserialize
			 */
			const Buffer& operator << ( const Buffer& buf )
				{
					buf >> this->element;

					return this->DeserializeChildren( buf );
				}

			Buffer& operator >> ( Buffer& buf ) const NOEXCEPT
				{
					buf << this->element;

					return this->SerializeChildren( buf );
				}

		public:
			/*
			 @	Get element
			 */
			FC_INLINE [[nodiscard]] auto&& operator * ( this auto&& self ) noexcept
				{
					return self.element;
				}

			/*
			 @	Direct access element
			 */
			FC_INLINE [[nodiscard]] auto* operator -> ( this auto& self ) noexcept
				{
					if constexpr( std::is_pointer_v< T > )
						{
							return &std::forward_like< decltype( self ) >
									(
										*self.element
									);
						}
					else
						{
							return &self.element;
						}
				}

		public:
			/*
			 @	Constructor
			 */
			Tree() = default;

			/*
			 @	Constructor root
			 */
			Tree( T&& element ) noexcept
							  ( noexcept( T( std::forward< T >( element ) ) ) )
				:
					element( std::forward< T >( element ) )
				{
				}

			/*
			 @	Constructor child
			 */
			Tree( Tree* const parent ) noexcept
				:
					Node( parent )
				{
				}

			Tree( Tree* const parent,
				  T&& element ) noexcept
							  ( noexcept( T( std::forward< T >( element ) ) ) )
				:
					Node( parent ),
					element( element )
				{
				}

		public:
			/*
			 @	Constructor buffer
			 */
			explicit Tree( const Buffer& buf )
				:
					Tree( nullptr, buf )
				{
				}

			Tree( Tree* const parent,
				  const Buffer& buf )
				:
					Node( parent ),
					element( buf )
				{
					this->DeserializeChildren( buf );
				}

			/*
			 @	Constructor move
			 */
			Tree( Tree&& tree ) noexcept
				:
					Tree( nullptr, std::move( tree ) )
				{
				}

			Tree( Tree* const parent,
				  Tree&& tree ) noexcept
				:
					Node( parent ),
					element( std::move( tree.element ) )
				{
					this->MoveChildren( std::move( tree ) );
				}

			/*
			 @	Constructor copy
			 */
			Tree( const Tree& tree ) noexcept
								   ( noexcept( T( tree.element ) ) )
				:
					Tree( nullptr, tree )
				{
				}

			Tree( Tree* const parent,
				  const Tree& tree ) noexcept
								   ( noexcept( T( tree.element ) ) )
				:
					Node( parent ),
					element( tree.element )
				{
					this->CopyChildren( tree );
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
				 @	Constructor xml
				 */
				Tree( const Xml::Deserialization& des ) NOEXCEPT
					:
						Tree( nullptr, des )
					{
					}

				Tree( Tree* const parent,
					  const Xml::Deserialization& des ) NOEXCEPT
					:
						Node( parent )
					{
						*this << des;
					}

				/*
				 @	Deserialize xml
				 */
				Void operator << ( const Xml::Deserialization& des ) NOEXCEPT
					{
						des.Deserialize( this->element, "Element" );

						this->DeserializeChildren( des );
					}

				/*
				 @	Serialize xml
				 */
				Void operator >> ( Xml::Serialization& ser ) const NOEXCEPT
					{
						ser.Serialize( this->element, "Element" );

						this->SerializeChildren( ser );
					}

			#endif

		protected:
			/*	Element
			*/
			T element;

			/*	Archive name
			*/
			static constexpr inline char ARCHIVE_NAME[] = "Tree";
		};
}