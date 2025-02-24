/*
**
**	Viewport.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/platform/Window.h"


/**
 !	Viewport
 */
namespace fce
{
	/*
	 @	构造 模型
	 */
	Viewport::Viewport( const Int16 x, const Int16 y,
						const Int16 w, const Int16 h,
						const Int16 n,
						const Int16 f ) noexcept
		:
			components
				{
					x, y,
					w, h,
					n, f - n
				}
		{
		}

	/*
	 @	构造 模型
	 */
	Viewport::Viewport( const Point pt,
						const fce::Size size,
						const Range range ) noexcept
		:
			Viewport( pt.x, pt.y,
					  size.width,
					  size.height,
					  range.min,
					  range.max )
		{
		}

	/*
	 @	构造 模型
	 */
	Viewport::Viewport( const inner::window::Surface& surface,
						const Range range ) noexcept
		:
			Viewport( 0,
					  surface.GetSize(),
					  range )
		{
		}

	/*
	 @	Viewport matrix
	
			w/2		0		0		x + w/2
			0		-h/2	0		y + h/2
			0		0		d/2		z + d/2
			0		0		0		1
	 */
	Affine3 Viewport::operator * () const noexcept
		{
			const Size3 scale( this->HalfSize() );

			const Vector3 centre( this->Position() + scale );

			return Affine3( scale.x, 0,		  0,	   centre.x,
							0,		-scale.y, 0,	   centre.y,
							0,		 0,		  scale.z, centre.z );
		}
}



namespace fce
{
	/*
	 @	获取 size
	 */
	Size3 Viewport::Size() const noexcept
		{
			return Size3( this->width,
						  this->height,
						  this->depth );
		}

	/*
	 @	获取 Half size
	 */
	Size3 Viewport::HalfSize() const noexcept
		{
			return math::Half( this->Size() );
		}

	/*
	 @	获取 pos
	 */
	Vector3 Viewport::Position() const noexcept
		{
			return Vector3( this->x,
							this->y,
							this->z );
		}

	/*
	 @	设置 范围
	 */
	Void Viewport::Set( Int16 l, Int16 t,
						Int16 r, Int16 b,
						Int16 n, Int16 f ) noexcept
		{
			this->SetRect( l, t, r, b );

			this->SetDepth( n, f );
		}

	/*
	 @	设置 rect
	 */
	Void Viewport::SetRect( Int16 l, Int16 t,
							Int16 r, Int16 b ) noexcept
		{
			this->x = l;
			this->y = t;

			this->width = r - l;
			this->height = t - b;
		}

	/*
	 @	设置 depth
	 */
	Void Viewport::SetDepth( Int16 n, Int16 f ) noexcept
		{
			this->z = n;
			this->depth = f - n;
		}

	/**
	 !	Cross platform
	 */
	#ifdef FC_VULKAN

		/*
		 @	VkRect2D
		 */
		Viewport::Viewport( const VkViewport& vp ) noexcept
			:
				Viewport( static_cast< Int16 >( vp.x ),
						  static_cast< Int16 >( vp.y ),
						  static_cast< Int16 >( vp.width ),
						  static_cast< Int16 >( vp.height ),
						  static_cast< Int16 >( vp.minDepth ),
						  static_cast< Int16 >( vp.maxDepth - vp.minDepth ) )
			{
			}

		Viewport::operator VkViewport () const noexcept
			{
				return VkViewport{ .x		 = float( this->x ),
								   .y		 = float( this->y ),
								   .width	 = float( this->width ),
								   .height	 = float( this->height ),
								   .minDepth = float( this->z ),
								   .maxDepth = float( this->z + this->depth ) };
			}

		Viewport& Viewport::operator = ( const VkViewport& vp ) noexcept
			{
				this->x		 = static_cast< Int16 >( vp.x );
				this->y		 = static_cast< Int16 >( vp.y );
				this->z		 = static_cast< Int16 >( vp.minDepth );
				this->width  = static_cast< Int16 >( vp.width );
				this->height = static_cast< Int16 >( vp.height );
				this->depth  = static_cast< Int16 >( vp.maxDepth - vp.minDepth );

				return *this;
			}

	#endif
}



/**
 !	Frustum
 */
namespace fce
{
	/*
	 @	Constructor 基本
	 */
	Frustum::Frustum( PROJECTION projection,
					  Float l, Float t,
					  Float r, Float b,
					  Float n, Float f ) noexcept
		:
			Frustum( projection,
				   { l, t, r, b },
					 n, f )
		{
		}

	/*
	 @	Constructor AABox
	 */
	Frustum::Frustum( PROJECTION projection,
					  AABox2 aabox,
					  Float zNear,
					  Float zFar ) noexcept
		:
			projection( projection ),
			aabox( aabox ),
			zNear( zNear ),
			zFar ( zFar )
		{
		}

	/*
	 @	Constructor

			projection by size
	 */
	Frustum::Frustum( Size2 size,
					  Float zNear,
					  Float zFar ) noexcept
		:
			zNear( zNear ),
			zFar( zFar ),
			projection
				{
					PROJECTION::ORTHOGRAPHIC
				}
		{
			const Size2 half( math::Half( size ) );

			this->aabox = { -half, half };
		}


	/*
	 @	Constructor

			projection by filed of view
	 */
	Frustum::Frustum( Float fovy,
					  Float aspect,
					  Float zNear,
					  Float zFar ) noexcept
		:
			zNear( zNear ),
			zFar( zFar ),
			projection
				{
					PROJECTION::PERSPECTIVE
				}
		{
			const Float height
				{
					math::Tan( math::Half( fovy ) )
				  * zFar
				};

			const Float width
				{
					height * aspect
				};

			const Size2 half( width, height );

			this->aabox = { -half, half };
		}
}



namespace fce
{
	/*
	 @	Horizontal field of view
	 */
	Float Frustum::GetFovH() const noexcept
		{
			return math::Double
				   (
						math::ATan
						(
							this->aabox.Width()
						  / math::Double( this->zFar )
						)
				   );
		}


	/*
	 @	Vertical field of view
	 */
	Float Frustum::GetFovV() const noexcept
		{
			return math::Double
				   (
						math::ATan
						(
							this->aabox.Height()
						  / math::Double( this->zFar )
						)
				   );
		}


	/*
	 @	Frustum matrix4
	 */
	Matrix4 Frustum::operator * () const noexcept
		{
			return PROJECTION::PERSPECTIVE
				== this->projection
				 ? this->Perspective()
				 : this->Orthographic();
		}


	/*
	 @	Perspective
	
			2*f/(r-l)	0			(r+l)/(r-l)		0
			0			2*f/(t-b)	(t+b)/(t-b)		0
			0			0			(n+f)/(n-f)		2*n*f/(n-f)
			0			0			-1				0
	 */
	Matrix4 Frustum::Perspective() const noexcept
		{
			// z scale & offset
			const Float zFar2
				{
					math::Double( this->zFar )
				};

			const Float negDepth
				{
					this->zNear - this->zFar
				};

			const Float zScale
				{
					( this->zNear + this->zFar )
					/ negDepth
				};

			const Float zOffst
				{
					( this->zNear * zFar2 )
					/ negDepth
				};

			// xy scale
			const Size2 size( this->aabox.Size() );

			const Vector2 scale( size.Inverse( zFar2 ) );

			// offcenter
			const Vector2 ratio( ( this->aabox.min
								 + this->aabox.max ) / size );

			return Matrix4( scale.x, 0,		  ratio.x, 0,
							0,		 scale.y, ratio.y, 0,
							0,		 0,		  zScale,  zOffst,
							0,		 0,		  -1,	   0	  );
		}


	/*
	 @	Orthographic
	
			2/(r-l)	0		0		 (l+r)/(l-r)
			0		2/(t-b)	0		 (b+t)/(b-t)
			0		0		2/(n-f)	 (n+f)/(n-f)
			0		0		0		 1
	 */
	Matrix4 Frustum::Orthographic() const noexcept
		{
			// z scale & offset
			const Float negDepth
				{
					this->zNear - this->zFar
				};

			const Float zScale
				{
					Float_TWO / negDepth
				};

			const Float zOffst
				{
					( this->zNear + this->zFar )
					/ negDepth
				};

			// xy scale
			const Size2 size( this->aabox.Size() );

			const Vector2 scale( size.Inverse( Float_TWO ) );

			// offcenter
			const Vector2 ratio( ( this->aabox.min
								 + this->aabox.max ) / -size );

			return Matrix4( scale.x, 0,		  0,	  ratio.x,
							0,		 scale.y, 0,	  ratio.y,
							0,		 0,		  zScale, zOffst,
							0,		 0,		  0,	  1		  );
		}
}