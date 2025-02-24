/*
**
**	Camera.
**
		观察坐标系 Observer
			eye(0,0,0) 眼睛位置为原点
			dir(0,0,-1) 看向负z轴

		视景体 Frustum
			远近裁剪面 顶点连线 构成的棱台
			只有视景体内的实体可见 用于裁剪

		远裁剪面 far clip plan
			Plane( normal(0,0,-1), zFar )
			AABox2( l, r, b, t ) 决定其offset和size

		近裁剪面 near clip plan
			Plane( normal(0,0,-1), zNear )
			它的 AABox2 由远裁剪面决定
			正交：near AABox2 = far AABox2
			透视：eye 与 远裁剪面 四个顶点连成射线
				  与 近裁剪面 相交连线为 near AABox2
			
		视距 d
			eye到 投影面 的距离 d(-∞,∞)
			d > 0 正常投影
			d < 0 翻转投影
			d = 0 原点投影

			取 d = 1 可简化投影计算
				但不便于将 投影面 映射到 视口
				因为还需要计算 投影面的size
			取 d = zNear 可方便映射
				但 正交和透视 切换 存在跳跃
			取 d = zFar 我们选此值
				此时 投影面 = 远裁剪面
				并且 正交和透视 可平滑切换

		投影面 plane of projection
			将视景体内的所有实体 投影到这个平面
			透视：eye与p(x,y,z) 连线与投影面的交点 p`为投影点
				  通过相似三角形 x`=d*x/z  y`=d*y/z 进行缩放变换

			投影矩阵 将 视景体变换为
			AABox3( (-1,-1,-1), (1,1,1) )
			轴对齐规范化立方体 便于映射到视口

		视口 Viewport
			将投影面缩放到视口 AABox3( ( x, y, z ), ( x+w, y+h, z+d ) )

			视口矩阵 将[-1, 1] 映射到 [0, size] + offset
			y轴翻转

	Copyright (c) WangFeng since 2006...
*/

#include "../../include/platform/Window.h"
#include "../../include/resource/Asset.h"


/**
 !	LookAt
 */
namespace fce
{
	/*
	 @	构造 模型
	 */
	LookAt::LookAt( const Vector3& eye,
					const Vector3& at,
					const Vector3& up ) noexcept
		:
			eye( eye ),
			at( at ),
			up( up )
		{
		}

	/*
	 @	获取 方位四元数
	 */
	Quaternion LookAt::Orientation() const noexcept
		{
			return this->Axes().Transpose();
		}

	/*
	 @	Axes  LH
	 */
	Matrix3 LookAt::Axes() const noexcept
		{
			Matrix3 mat3;

			this->Axes( mat3 );

			return mat3;
		}

	Void LookAt::Axes( Matrix3& mat3 ) const noexcept
		{
			this->Axes( mat3[ 0 ],
						mat3[ 1 ],
						mat3[ 2 ] );
		}

	Void LookAt::Axes( Vector3& xAxis,
					   Vector3& yAxis,
					   Vector3& zAxis ) const noexcept
		{
			zAxis = ( this->at - this->eye ).Normalized();

			xAxis = this->up.Cross( zAxis ).Normalized();

			yAxis = zAxis.Cross( xAxis ).Normalized();
		}

	/*
	 @	LookAt LH
	
			xAxis.x		xAxis.y		xAxis.z		-Dot( xAxis, eye )
			yAxis.x		yAxis.y		yAxis.z		-Dot( yAxis, eye )
			zAxis.x		zAxis.y		zAxis.z		-Dot( zAxis, eye )
			0			0			0			1
	 */
	Affine3 LookAt::operator * () const noexcept
		{
			const Vector3 zAxis
				{
					( this->at - this->eye ).Normalized()
				};

			const Vector3 xAxis
				{
					this->up.Cross( zAxis ).Normalized()
				};

			const Vector3 yAxis
				{
					zAxis.Cross( xAxis ).Normalized()
				};

			return Affine3( Vector4( xAxis, -this->eye.Dot( xAxis ) ),
							Vector4( yAxis, -this->eye.Dot( yAxis ) ),
							Vector4( zAxis, -this->eye.Dot( zAxis ) ) );
		}
}



/**
 !	Camera
 */
namespace fce
{
	/*
	 @	析构
	 */
	Camera::~Camera() noexcept
		{
			// 移除 Scene 列表
			scene->Remove( this );

			// 取消Transform消息
			this->node.RemoveHandler
				(
					this, &Camera::on_Transformed
				);
		}

	/*
	 @	构造 Buffer
	 */
	Camera::Camera( Entity& entity,
					const Buffer& buf )
		:
			NodeComponent( entity )
		{
			*this << buf;

			this->on_Constructed();
		}

	/*
	 @	构造 默认
	 */
	Camera::Camera( Node& node ) noexcept
		:
			NodeComponent( node )
		{
			this->on_Constructed();
		}

	/*
	 @	构造 Model
	 */
	Camera::Camera( Node& node,
					const Frustum& frustum,
					const Viewport& viewport ) noexcept
		:
			frustum( frustum ),
			viewport( viewport ),
			NodeComponent( node )
		{
			this->on_Constructed();
		}

	/*
	 @	构造 Copy
	 */
	Camera::Camera( Node& node,
					const Camera& camera ) noexcept
		:
			Camera( node,
					camera.frustum,
					camera.viewport )
		{
		}

	/*
	 @	赋值 拷贝
	 */
	Camera& Camera::operator = ( const Camera& camera ) noexcept
		{
			this->frustum = camera.frustum;

			this->viewport = camera.viewport;

			this->Update();

			return *this;
		}
}



namespace fce
{
	/*
	 @	设置 Lookat
	 */
	Void Camera::Lookat( const Vector3& eye,
						 const Vector3& at,
						 const Vector3& up ) noexcept
		{
			this->Lookat( LookAt( eye, at, up ) );
		}

	Void Camera::Lookat( const LookAt& lookat ) noexcept
		{
			// 设置 node transform
			Transform& transform{ this->node.transform };

			transform.orientation = lookat.Orientation();

			transform.position = lookat.eye;

			// 更新 node transform
			this->node.Update();
		}

	/*
	 @	更新 矩阵
	 */
	Void Camera::Update() noexcept
		{
			// 视口 矩阵，图形管线 已经设置过
			this->matViewport = *this->viewport;

			// 投影 矩阵
			this->matProjection = *this->frustum;

			// 观察者 矩阵
			this->UpdateObserver();
		}
	

	/*
	 @	UpdateObserver
	 */
	Void Camera::UpdateObserver() noexcept
		{
			// 坐标轴
			const Matrix3 uvn( this->node.GlobalOrientation() );

			// 观察矩阵
			Vector3& xAxis{ this->matObserver[ 0 ].xyz };
			Vector3& yAxis{ this->matObserver[ 1 ].xyz };
			Vector3& zAxis{ this->matObserver[ 2 ].xyz };

			xAxis = -uvn.Column( 0 );
			yAxis = -uvn.Column( 1 );
			zAxis = -uvn.Column( 2 );

			// 位置
			const Vector3& pos
				{
					this->node.GlobalPosition()
				};

			this->matObserver._14 = -pos.Dot( xAxis );
			this->matObserver._24 = -pos.Dot( yAxis );
			this->matObserver._34 = -pos.Dot( zAxis );

			// 最终相机矩阵
			this->matCamera = this->matProjection
							* this->matObserver;
		}


	/*
	 @	On constructed
	 */
	Void Camera::on_Constructed() noexcept
		{
			// 暂时设置 主窗口的 surface
			surface = &s_Window.surface;

			// 添加 Scene 列表
			scene = &this->node.GetScene();

			scene->Add( this );

			// 监听 Transform 消息
			this->node.AddHandler
				(
					this,
					&Camera::on_Transformed
				);

			this->Update();
		}


	/*
	 @	Respond transform
	 */
	Void Camera::on_Transformed( Node::EVENT,
								 Node& ) noexcept
		{
			this->UpdateObserver();
		}
}



namespace fce
{
	/*
	 @	Render
	 */
	Void Camera::Render( const Scene* const scene ) const noexcept
		{
			// 需要考虑 多线程
			for( Renderer* const renderer : scene->listRenderers )
				{
					renderer->UpdateWVP( this->matCamera );

					renderer->Present( *this->surface );
				}
		}
}