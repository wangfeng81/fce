/*
**
**	Camera.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Renderer.h"


/**
 !	Camera inner kit
 */
namespace fce::inner::camera
{
	/*	MODE
	 */
	enum struct MODE
		:
			Byte
		{
			CUSTOM,
			WINDOW,

			MAXIMUM
		};
}



namespace fce
{
	/*
	 !	LookAt
	 */
	struct FC_API LookAt final
		{
			/*	Model
			*/
			Vector3 eye;

			Vector3 at;

			Vector3 up;

		public:
			/*	Constructor
			*/
			LookAt() = default;

			LookAt( const Vector3& eye,
					const Vector3& at = Vector3::ZERO,
					const Vector3& up = Vector3::UNIT_Y ) noexcept;

			/*	Axes
			*/
			Void Axes( Matrix3& ) const noexcept;

			Void Axes( Vector3& xAxis,
					   Vector3& yAxis,
					   Vector3& zAxis ) const noexcept;

			[[nodiscard]] Matrix3 Axes() const noexcept;

			/*	Common
			*/
			[[nodiscard]] Affine3 operator * () const noexcept;

			[[nodiscard]] Quaternion Orientation() const noexcept;
		};
}



namespace fce
{
	/*
	 !	Camera
	 */
	class FC_API Camera final
		:
		public SceneNodeComponent
		{
			/*	Friends
			*/
			friend Scene;

			/*
			 @	Archive
			 */
			COMPONENT_ARCHIVE( Camera,
							   NodeComponent,
							   ARCHIVE_UNIT( frustum ),
							   ARCHIVE_UNIT( viewport ) );

		public:
			/*	Using inner kit
			*/
			using MODE = inner::camera::MODE;

			/*	Model
			*/
			Frustum frustum;

			Viewport viewport;

			/*	Query by ID
			*/
			QUERY_ID( Camera )

		public:
			/*	Update
			*/
			Void Update() noexcept;

			/*	Lookat
			*/
			Void Lookat( const LookAt& ) noexcept;

			Void Lookat( const Vector3& eye,
						 const Vector3& at,
						 const Vector3& up ) noexcept;

			/*	Matrix4
			*/
			const Matrix4& operator * () const noexcept;

			/*	Assignment
			*/
			Camera& operator = ( const Camera& ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~Camera() noexcept;

			Camera( Entity&,
					const Buffer& );

			Camera( Node& ) noexcept;

			Camera( Node&,
					const Camera& ) noexcept;

			Camera( Node&,
					const Frustum&,
					const Viewport& ) noexcept;

		private:
			/*	Update observer
			*/
			Void UpdateObserver() noexcept;

			/*	On
			*/
			Void on_Constructed() noexcept;

			Void on_Transformed( Node::EVENT,
								 Node& ) noexcept;

			/*	Render
			*/
			Void Render( const Scene* const ) const noexcept;

		private:
			/*	Matrix
			*/
			Matrix4 matCamera;

			Affine3 matViewport;

			Affine3 matObserver;

			Matrix4 matProjection;

			/*	Scene & Surface
			*/
			Scene* scene;

			WindowSurface* surface;
		};
}



/**
 !	Camera
 */
namespace fce
{
	/*
	 @	Matrix4
	 */
	FC_INLINE const Matrix4& Camera::operator * () const noexcept
		{
			return this->matCamera;
		}
}