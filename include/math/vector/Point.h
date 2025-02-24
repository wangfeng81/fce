/*
**
**	Point
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Point

			For pixel coordinates
	 */
	struct FC_API Point final
		{
			/*	Model
			*/
			union
				{
					DWord value;

					Int16 components[ 2 ];

					struct
						{
							Int16 x, y;
						};

					struct
						{
							Int16 min,
								  max;
						};

					struct
						{
							Int16 width,
								  height;
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( Point,
							ARCHIVE_UNIT( x ),
							ARCHIVE_UNIT( y ) )

			/*	Constructor
			*/
			constexpr Point() noexcept;

			constexpr Point( const Point& ) noexcept;

			constexpr Point( const Int16 ) noexcept;

			constexpr Point( const Int16 x,
							 const Int16 y ) noexcept;

			/*	Conversion Vector2
			*/
			constexpr Point( const Vector2& ) noexcept;

			constexpr operator Vector2 () const noexcept;

			/*	Combinatorial =
			*/
			constexpr Point& operator = ( const Point& ) noexcept;

			constexpr Point& operator >>= ( const Int16 ) noexcept;

			constexpr Point& operator <<= ( const Int16 ) noexcept;

			constexpr Point& operator += ( const Point& ) noexcept;

			constexpr Point& operator -= ( const Point& ) noexcept;

			/*	Operation
			*/
			constexpr Point operator - () const noexcept;

			constexpr Point operator >> ( const Int16 ) const noexcept;

			constexpr Point operator << ( const Int16 ) const noexcept;

			constexpr Point operator + ( const Point& ) const noexcept;

			constexpr Point operator - ( const Point& ) const noexcept;

			/*	Compare
			*/
			constexpr Bool operator == ( const Point& ) const noexcept;

			constexpr Bool operator != ( const Point& ) const noexcept;

			constexpr Bool operator <= ( const Point& ) const noexcept;

			constexpr Bool operator >= ( const Point& ) const noexcept;

			/*	Check zero
			*/
			constexpr explicit operator Bool() const noexcept;

			/*	Common
			*/
			constexpr Int32 Area() const noexcept;

			constexpr Point Half() const noexcept;

			constexpr Point Double() const noexcept;

			constexpr Float Aspect() const noexcept;

			/**
			 !	Cross platform
			 */
			#ifdef FC_WINDOWS

				/*	POINT
				*/
				constexpr Point( const POINT& ) noexcept;

				constexpr Point& operator = ( const POINT& ) noexcept;

				constexpr operator POINT () const noexcept;

				/*	SIZE
				*/
				constexpr Point( const SIZE& ) noexcept;

				constexpr Point& operator = ( const SIZE& ) noexcept;

				constexpr operator SIZE () const noexcept;

			#endif

			#ifdef FC_VULKAN

				/*	VkExtent2D
				*/
				constexpr Point( const VkOffset2D& ) noexcept;

				constexpr operator VkOffset2D () const noexcept;

				constexpr Point& operator = ( const VkOffset2D& ) noexcept;

				/*	VkExtent2D
				*/
				constexpr Point( const VkExtent2D& ) noexcept;

				constexpr operator VkExtent2D () const noexcept;

				constexpr Point& operator = ( const VkExtent2D& ) noexcept;

			#endif
		};
}



/**
 !	class Point
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Point::Point() noexcept
		:
			value( 0 )
		{
		}

	FC_INLINE constexpr Point::Point( const Point& pt ) noexcept
		:
			value( pt.value )
		{
		}

	FC_INLINE constexpr Point::Point( const Int16 c ) noexcept
		:
			components{ c, c }
		{
		}

	FC_INLINE constexpr Point::Point( const Int16 x,
									  const Int16 y ) noexcept
		:
			components{ x, y }
		{
		}

	/*
	 @	Conversion Vector2
	 */
	FC_INLINE constexpr Point::Point( const Vector2& vec2 ) noexcept
		:
			components{ static_cast< Int16 >( vec2.x ),
						static_cast< Int16 >( vec2.y ) }
		{
		}

	FC_INLINE constexpr Point::operator Vector2 () const noexcept
		{
			return Vector2( this->x,
							this->y );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE constexpr Point& Point::operator = ( const Point& pt ) noexcept
		{
			this->value = pt.value;

			return *this;
		}

	FC_INLINE constexpr Point& Point::operator >>= ( const Int16 c ) noexcept
		{
			return *this = *this >> c;
		}

	FC_INLINE constexpr Point& Point::operator <<= ( const Int16 c ) noexcept
		{
			return *this = *this << c;
		}

	FC_INLINE constexpr Point& Point::operator += ( const Point& pt ) noexcept
		{
			return *this = *this + pt;
		}

	FC_INLINE constexpr Point& Point::operator -= ( const Point& pt ) noexcept
		{
			return *this = *this - pt;
		}

	/*
	 @	Operation
	 */
	FC_INLINE constexpr Point Point::operator - () const noexcept
		{
			return Point( -this->x,
						  -this->y );
		}

	FC_INLINE constexpr Point Point::operator >> ( const Int16 c ) const noexcept
		{
			return Point( this->x >> c,
						  this->y >> c );
		}

	FC_INLINE constexpr Point Point::operator << ( const Int16 c ) const noexcept
		{
			return Point( this->x << c,
						  this->y << c );
		}

	FC_INLINE constexpr Point Point::operator + ( const Point& pt ) const noexcept
		{
			return Point( this->x + pt.x,
						  this->y + pt.y );
		}

	FC_INLINE constexpr Point Point::operator - ( const Point& pt ) const noexcept
		{
			return Point( this->x - pt.x,
						  this->y - pt.y );
		}

	/*
	 @	Compare
	 */
	FC_INLINE constexpr Bool Point::operator == ( const Point& pt ) const noexcept
		{
			return this->value == pt.value;
		}

	FC_INLINE constexpr Bool Point::operator != ( const Point& pt ) const noexcept
		{
			return this->value != pt.value;
		}

	FC_INLINE constexpr Bool Point::operator <= ( const Point& pt ) const noexcept
		{
			return this->x <= pt.x
				&& this->y <= pt.y;
		}

	FC_INLINE constexpr Bool Point::operator >= ( const Point& pt ) const noexcept
		{
			return this->x >= pt.x
				&& this->y >= pt.y;
		}

	/*
	 @	Check zero
	 */
	FC_INLINE constexpr Point::operator Bool() const noexcept
		{
			return this->value;
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Int32 Point::Area() const noexcept
		{
			return this->x * this->y;
		}

	FC_INLINE constexpr Point Point::Half() const noexcept
		{
			return *this >> 1;
		}

	FC_INLINE constexpr Point Point::Double() const noexcept
		{
			return *this << 1;
		}

	FC_INLINE constexpr Float Point::Aspect() const noexcept
		{
			return Float( this->x )
				 / Float( this->y );
		}
}



/**
 !	WINDOWS
 */
namespace fce
{
	#ifdef FC_WINDOWS

		/*
		 @	POINT
		 */
		FC_INLINE constexpr Point::Point( const POINT& pt ) noexcept
			:
				components{ static_cast< Int16 >( pt.x ),
							static_cast< Int16 >( pt.y ) }
			{
			}

		FC_INLINE constexpr Point::operator POINT () const noexcept
			{
				return POINT{ .x = static_cast< LONG >( this->x ),
							  .y = static_cast< LONG >( this->y ) };
			}

		FC_INLINE constexpr Point& Point::operator = ( const POINT& pt ) noexcept
			{
				this->x = static_cast< Int16 >( pt.x );
				this->y = static_cast< Int16 >( pt.y );

				return *this;
			}

		/*
		 @	SIZE
		 */
		FC_INLINE constexpr Point::Point( const SIZE& sz ) noexcept
			:
				components{ static_cast< Int16 >( sz.cx ),
							static_cast< Int16 >( sz.cy ) }
			{
			}

		FC_INLINE constexpr Point::operator SIZE () const noexcept
			{
				return SIZE{ .cx = static_cast< LONG >( this->x ),
							 .cy = static_cast< LONG >( this->y ) };
			}

		FC_INLINE constexpr Point& Point::operator = ( const SIZE& sz ) noexcept
			{
				this->x = static_cast< Int16 >( sz.cx );
				this->y = static_cast< Int16 >( sz.cy );

				return *this;
			}

	#endif
}



/**
 !	VULKAN
 */
namespace fce
{
	#ifdef FC_VULKAN

		/*
		 @	VkOffset2D
		 */
		FC_INLINE constexpr Point::operator VkOffset2D () const noexcept
			{
				return VkOffset2D{ .x = static_cast< int32_t >( this->x ),
								   .y = static_cast< int32_t >( this->y ) };
			}

		FC_INLINE constexpr Point::Point( const VkOffset2D& offset2D ) noexcept
			:
				components{ static_cast< Int16 >( offset2D.x ),
							static_cast< Int16 >( offset2D.y ) }
			{
			}

		FC_INLINE constexpr Point& Point::operator = ( const VkOffset2D& offset2D ) noexcept
			{
				this->x = static_cast< Int16 >( offset2D.x );
				this->y = static_cast< Int16 >( offset2D.y );

				return *this;
			}

		/*
		 @	VkExtent2D
		 */
		FC_INLINE constexpr Point::operator VkExtent2D () const noexcept
			{
				return VkExtent2D{ .width  = static_cast< uint32_t >( this->x ),
								   .height = static_cast< uint32_t >( this->y ) };
			}

		FC_INLINE constexpr Point::Point( const VkExtent2D& extent2D ) noexcept
			:
				components{ static_cast< Int16 >( extent2D.width ),
							static_cast< Int16 >( extent2D.height ) }
			{
			}

		FC_INLINE constexpr Point& Point::operator = ( const VkExtent2D& extent2D ) noexcept
			{
				this->x = static_cast< Int16 >( extent2D.width );
				this->y = static_cast< Int16 >( extent2D.height );

				return *this;
			}

	#endif
}