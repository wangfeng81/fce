/*
**
**	Viewport
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../math/Ray.h"


/**
 !	Frustum inner kit
 */
namespace fce::inner::frustum
{
	/*	PLANE
	 */
	enum struct PLANE
		:
			Byte
		{
			LEFT,
			RIGHT,
			TOP,
			BOTTOM,
			_NEAR,
			_FAR,

			MAXIMUM
		};

	/*	PROJECTION
	 */
	enum struct PROJECTION
		:
			Byte
		{
			PERSPECTIVE,
			ORTHOGRAPHIC,

			MAXIMUM
		};
}



namespace fce
{
	/*
	 !	Scissor
	 */
	using Scissor = Rect;
}


namespace fce
{
	 /*
	 !	Viewport

			Axis-aligned cuboid
	 */
	struct FC_API Viewport final
		{
			/*	Model
			 */
			union
				{
					Int16 components[ 6 ];

					struct
						{
							Int16 x, y,
								  width,
								  height,
								  z, depth;
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( Viewport,
							ARCHIVE_UNIT( x ),
							ARCHIVE_UNIT( y ),
							ARCHIVE_UNIT( z ),
							ARCHIVE_UNIT( width ),
							ARCHIVE_UNIT( height ),
							ARCHIVE_UNIT( depth ) )

		public:
			/*	Constructor
			*/
			Viewport() = default;

			Viewport( const Int16 x, const Int16 y,
					  const Int16 w, const Int16 h,
					  const Int16 n = -1,
					  const Int16 f = +1 ) noexcept;

			Viewport( const Point pos,
					  const Point size,
					  const Range = { -1, 1 } ) noexcept;

			Viewport( const inner::window::Surface&,
					  const Range = { -1, 1 } ) noexcept;

		public:
			/*	Set
			*/
			Void Set( const Int16 l, const Int16 t,
					  const Int16 r, const Int16 b,
					  const Int16 n = -1,
					  const Int16 f = +1 ) noexcept;

			Void SetRect( const Int16 l, const Int16 t,
						  const Int16 r, const Int16 b ) noexcept;

			Void SetDepth( const Int16 n, const Int16 f ) noexcept;

			/*	Get
			*/
			[[nodiscard]] Size3 Size() const noexcept;

			[[nodiscard]] Float Aspect() const noexcept;

			[[nodiscard]] Size3 HalfSize() const noexcept;

			[[nodiscard]] Vector3 Position() const noexcept;

			/*	Mapping matrix
			*/
			[[nodiscard]] Affine3 operator * () const noexcept;

		public:
			/**
			 !	Cross platform
			 */
			#ifdef FC_VULKAN

				/*	VkViewport
				*/
				operator VkViewport () const noexcept;

				Viewport( const VkViewport& ) noexcept;

				Viewport& operator = ( const VkViewport& ) noexcept;

			#endif
		};
}



namespace fce
{
	/*
	 !	Frustum

			Map to normalized
			axis-aligned cube
	 */
	struct FC_API Frustum final
		{
			/*	Using inner kit
			*/
			using PLANE = inner::frustum::PLANE;

			using PROJECTION = inner::frustum::PROJECTION;

			/*	Model
			*/
			AABox2 aabox;

			Float zNear, zFar;

			PROJECTION projection;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Frustum,
							ARCHIVE_UNIT( zFar ),
							ARCHIVE_UNIT( zNear ),
							ARCHIVE_UNIT( projection ),
							ARCHIVE_INFO( aabox, "AABox2" ) )

		public:
			/*	Constructor
			*/
			Frustum() = default;

			Frustum( const PROJECTION,
					 const Float l, const Float t,
					 const Float r, const Float b,
					 const Float n = 1e-2F,
					 const Float f = 1e4F ) noexcept;

			Frustum( const PROJECTION,
					 const AABox2,
					 const Float zNear = 1e-2F,
					 const Float zFar = 1e4F ) noexcept;

			/*	Constructor orthographic
			*/
			Frustum( const Size2,
					 const Float zNear = 1e-2F,
					 const Float zFar = 1e4F ) noexcept;

			/*	Constructor perspective
			*/
			Frustum( const Float fovy,
					 const Float aspect,
					 const Float zNear = 1e-2F,
					 const Float zFar = 1e4F ) noexcept;

			/*	Common
			*/
			[[nodiscard]] Float Depth() const noexcept;

			[[nodiscard]] Float Aspect() const noexcept;

			[[nodiscard]] Float GetFovH() const noexcept;

			[[nodiscard]] Float GetFovV() const noexcept;

			/*	Projection
			*/
			[[nodiscard]] Matrix4 operator * () const noexcept;

			[[nodiscard]] Matrix4 Perspective() const noexcept;

			[[nodiscard]] Matrix4 Orthographic() const noexcept;
		};
}



/**
 !	Viewport
 */
namespace fce
{
	/*
	 @	Aspect
	 */
	FC_INLINE Float Viewport::Aspect() const noexcept
		{
			return ( Float )this->width
						  / this->height;
		}
}



/**
 !	Frustum
 */
namespace fce
{
	/*
	 @	Depth
	 */
	FC_INLINE Float Frustum::Depth() const noexcept
		{
			return this->zFar
				 - this->zNear;
		}

	/*
	 @	Aspect
	 */
	FC_INLINE Float Frustum::Aspect() const noexcept
		{
			return this->aabox.Aspect();
		}
}