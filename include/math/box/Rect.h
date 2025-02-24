/*
**
**	Rect
**
		Copyright (c) WangFeng since 2000...
*/

namespace fce
{
	/*
	 !	Rect

			For pixel coordinates
	 */
	struct FC_API Rect final
		{
			/*	Model
			 */
			union
				{
					QWord value;

					Point points[ 2 ];

					Int16 components[ 4 ];

					union
						{
							/*	min & max
							*/
							struct
								{
									Point min, max;
								};

							/*	l t r b
							*/
							struct
								{
									Int16 left, top,
										  right, bottom;
								};
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( Rect,
							ARCHIVE_UNIT( left ),
							ARCHIVE_UNIT( top ),
							ARCHIVE_UNIT( right ),
							ARCHIVE_UNIT( bottom ) )

			/*	Constructor
			*/
			constexpr Rect() noexcept;

			constexpr Rect( const Rect& ) noexcept;

			constexpr Rect( const Point&,
							const Point& ) noexcept;

			constexpr Rect( const Int16 l, const Int16 t,
							const Int16 r, const Int16 b ) noexcept;

			/*	Set
			*/
			Void Set( const Int16 x, const Int16 y,
					  const Int16 w, const Int16 h ) noexcept;

			/*	Conversion Vector2
			*/
			constexpr Rect( const AABox2& ) noexcept;

			constexpr operator AABox2 () const noexcept;

			/*	Operater =
			*/
			constexpr Rect& operator = ( const Rect& ) noexcept;

			/*	Combinatorial =
			*/
			constexpr Rect& operator += ( const Point& ) noexcept;

			constexpr Rect& operator -= ( const Point& ) noexcept;

			/*	Operation
			*/
			constexpr Rect operator + ( const Point& ) const noexcept;

			constexpr Rect operator - ( const Point& ) const noexcept;

			/*	Compare
			*/
			constexpr Bool operator == ( const Rect& ) const noexcept;

			constexpr Bool operator != ( const Rect& ) const noexcept;

			/*	Is it valid
			*/
			constexpr explicit operator Bool() const noexcept;

			/*	Common
			*/
			Void Width( const Int16 ) noexcept;

			Void Height( const Int16 ) noexcept;

			constexpr Int16 Width() const noexcept;

			constexpr Int16 Height() const noexcept;

			constexpr Size Size() const noexcept;

			constexpr Int16 Area() const noexcept;

			/**
			 !	Cross platform
			 */
			#ifdef FC_WINDOWS

				/*	RECT
				*/
				constexpr Rect( const RECT& ) noexcept;

				constexpr Rect& operator = ( const RECT& ) noexcept;

				constexpr operator RECT () const noexcept;

			#endif

			#ifdef FC_VULKAN

				/*	VkRect2D
				*/
				constexpr Rect( const VkRect2D& ) noexcept;

				constexpr Rect& operator = ( const VkRect2D& ) noexcept;

				constexpr operator VkRect2D () const noexcept;

			#endif
		};
}



/**
 !	struct Rect
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Rect::Rect() noexcept
		:
			value( 0 )
		{
		}

	FC_INLINE constexpr Rect::Rect( const Rect& rc ) noexcept
		:
			value( rc.value )
		{
		}

	FC_INLINE constexpr Rect::Rect( const Point& min,
									const Point& max ) noexcept
		:
			points{ min, max }
		{
		}

	FC_INLINE constexpr Rect::Rect( const Int16 l, const Int16 t,
									const Int16 r, const Int16 b ) noexcept
		:
			components{ l, t, r, b }
		{
		}

	/*
	 @	Set
	 */
	FC_INLINE Void Rect::Set( const Int16 x, const Int16 y,
							  const Int16 w, const Int16 h ) noexcept
		{
			this->left	 = x;
			this->top	 = y;
			this->right	 = x + w;
			this->bottom = y + h;
		}

	/*
	 @	Conversion AABox2
	 */
	FC_INLINE constexpr Rect::Rect( const AABox2& aab2 ) noexcept
		:
			components{ static_cast< Int16 >( aab2.l ),
						static_cast< Int16 >( aab2.t ),
						static_cast< Int16 >( aab2.r ),
						static_cast< Int16 >( aab2.b ) }
		{
		}

	FC_INLINE constexpr Rect::operator AABox2 () const noexcept
		{
			return AABox2( this->left,
						   this->top,
						   this->right,
						   this->bottom );
		}

	/*
	 @	Operater =
	 */
	FC_INLINE constexpr Rect& Rect::operator = ( const Rect& rc ) noexcept
		{
			this->value = rc.value;

			return *this;
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE constexpr Rect& Rect::operator += ( const Point& pt ) noexcept
		{
			return *this = *this + pt;
		}

	FC_INLINE constexpr Rect& Rect::operator -= ( const Point& pt ) noexcept
		{
			return *this = *this - pt;
		}

	/*
	 @	Operation
	 */
	FC_INLINE constexpr Rect Rect::operator + ( const Point& pt ) const noexcept
		{
			return Rect( this->min + pt,
						 this->max + pt );
		}

	FC_INLINE constexpr Rect Rect::operator - ( const Point& pt ) const noexcept
		{
			return Rect( this->min - pt,
						 this->max - pt );
		}

	/*
	 @	Compare
	 */
	FC_INLINE constexpr Bool Rect::operator == ( const Rect& rc ) const noexcept
		{
			return this->value == rc.value;
		}

	FC_INLINE constexpr Bool Rect::operator != ( const Rect& rc ) const noexcept
		{
			return this->value != rc.value;
		}

	/*
	 @	Is it valid
	 */
	FC_INLINE constexpr Rect::operator Bool() const noexcept
		{
			return this->left < this->right
				&& this->top < this->bottom;
		}

	/*
	 @	Common
	 */
	FC_INLINE Void Rect::Width( const Int16 w ) noexcept
		{
			this->right = this->left + w;
		}

	FC_INLINE Void Rect::Height( const Int16 h ) noexcept
		{
			this->bottom = this->top + h;
		}

	FC_INLINE constexpr Int16 Rect::Width() const noexcept
		{
			return this->right - this->left;
		}

	FC_INLINE constexpr Int16 Rect::Height() const noexcept
		{
			return this->bottom - this->top;
		}

	FC_INLINE constexpr Size Rect::Size() const noexcept
		{
			return { this->Width(), this->Height() };
		}

	FC_INLINE constexpr Int16 Rect::Area() const noexcept
		{
			return this->Size().Area();
		}

	/**
	 !	Cross platform
	 */
	#ifdef FC_WINDOWS

		/*
		 @	RECT
		 */
		FC_INLINE constexpr Rect::Rect( const RECT& rc ) noexcept
			:
				components{ static_cast< Int16 >( rc.left ),
							static_cast< Int16 >( rc.top ),
							static_cast< Int16 >( rc.right ),
							static_cast< Int16 >( rc.bottom ) }
			{
			}

		FC_INLINE constexpr Rect::operator RECT () const noexcept
			{
				return RECT{ .left	 = static_cast< LONG >( this->left ),
							 .top	 = static_cast< LONG >( this->top ),
							 .right	 = static_cast< LONG >( this->right ),
							 .bottom = static_cast< LONG >( this->bottom ) };
			}

		FC_INLINE constexpr Rect& Rect::operator = ( const RECT& rc ) noexcept
			{
				this->left	 = static_cast< Int16 >( rc.left );
				this->top	 = static_cast< Int16 >( rc.top );
				this->right	 = static_cast< Int16 >( rc.right );
				this->bottom = static_cast< Int16 >( rc.bottom );

				return *this;
			}

	#endif

	#ifdef FC_VULKAN

		/*
		 @	VkRect2D
		 */
		FC_INLINE constexpr Rect::Rect( const VkRect2D& rc ) noexcept
			:
				components{ static_cast< Int16 >( rc.offset.x ),
							static_cast< Int16 >( rc.offset.y ),
							static_cast< Int16 >( rc.offset.x + rc.extent.width ),
							static_cast< Int16 >( rc.offset.y + rc.extent.height ) }
			{
			}

		FC_INLINE constexpr Rect::operator VkRect2D () const noexcept
			{
				return VkRect2D
						{
							.offset
							{
								.x = this->left,
								.y = this->top
							},
							.extent
							{
								.width	= static_cast< uint32_t >( this->Width() ),
								.height	= static_cast< uint32_t >( this->Height() )
							}
						};
			}

		FC_INLINE constexpr Rect& Rect::operator = ( const VkRect2D& rc ) noexcept
			{
				this->left	 = static_cast< Int16 >( rc.offset.x );
				this->top	 = static_cast< Int16 >( rc.offset.y );
				this->right	 = static_cast< Int16 >( rc.offset.x + rc.extent.width );
				this->bottom = static_cast< Int16 >( rc.offset.y + rc.extent.height );

				return *this;
			}

	#endif
}