/*
**
**	Camera.
**
		�۲�����ϵ Observer
			eye(0,0,0) �۾�λ��Ϊԭ��
			dir(0,0,-1) ����z��

		�Ӿ��� Frustum
			Զ���ü��� �������� ���ɵ���̨
			ֻ���Ӿ����ڵ�ʵ��ɼ� ���ڲü�

		Զ�ü��� far clip plan
			Plane( normal(0,0,-1), zFar )
			AABox2( l, r, b, t ) ������offset��size

		���ü��� near clip plan
			Plane( normal(0,0,-1), zNear )
			���� AABox2 ��Զ�ü������
			������near AABox2 = far AABox2
			͸�ӣ�eye �� Զ�ü��� �ĸ�������������
				  �� ���ü��� �ཻ����Ϊ near AABox2
			
		�Ӿ� d
			eye�� ͶӰ�� �ľ��� d(-��,��)
			d > 0 ����ͶӰ
			d < 0 ��תͶӰ
			d = 0 ԭ��ͶӰ

			ȡ d = 1 �ɼ�ͶӰ����
				�������ڽ� ͶӰ�� ӳ�䵽 �ӿ�
				��Ϊ����Ҫ���� ͶӰ���size
			ȡ d = zNear �ɷ���ӳ��
				�� ������͸�� �л� ������Ծ
			ȡ d = zFar ����ѡ��ֵ
				��ʱ ͶӰ�� = Զ�ü���
				���� ������͸�� ��ƽ���л�

		ͶӰ�� plane of projection
			���Ӿ����ڵ�����ʵ�� ͶӰ�����ƽ��
			͸�ӣ�eye��p(x,y,z) ������ͶӰ��Ľ��� p`ΪͶӰ��
				  ͨ������������ x`=d*x/z  y`=d*y/z �������ű任

			ͶӰ���� �� �Ӿ���任Ϊ
			AABox3( (-1,-1,-1), (1,1,1) )
			�����淶�������� ����ӳ�䵽�ӿ�

		�ӿ� Viewport
			��ͶӰ�����ŵ��ӿ� AABox3( ( x, y, z ), ( x+w, y+h, z+d ) )

			�ӿھ��� ��[-1, 1] ӳ�䵽 [0, size] + offset
			y�ᷭת

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
	 @	���� ģ��
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
	 @	��ȡ ��λ��Ԫ��
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
	 @	����
	 */
	Camera::~Camera() noexcept
		{
			// �Ƴ� Scene �б�
			scene->Remove( this );

			// ȡ��Transform��Ϣ
			this->node.RemoveHandler
				(
					this, &Camera::on_Transformed
				);
		}

	/*
	 @	���� Buffer
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
	 @	���� Ĭ��
	 */
	Camera::Camera( Node& node ) noexcept
		:
			NodeComponent( node )
		{
			this->on_Constructed();
		}

	/*
	 @	���� Model
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
	 @	���� Copy
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
	 @	��ֵ ����
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
	 @	���� Lookat
	 */
	Void Camera::Lookat( const Vector3& eye,
						 const Vector3& at,
						 const Vector3& up ) noexcept
		{
			this->Lookat( LookAt( eye, at, up ) );
		}

	Void Camera::Lookat( const LookAt& lookat ) noexcept
		{
			// ���� node transform
			Transform& transform{ this->node.transform };

			transform.orientation = lookat.Orientation();

			transform.position = lookat.eye;

			// ���� node transform
			this->node.Update();
		}

	/*
	 @	���� ����
	 */
	Void Camera::Update() noexcept
		{
			// �ӿ� ����ͼ�ι��� �Ѿ����ù�
			this->matViewport = *this->viewport;

			// ͶӰ ����
			this->matProjection = *this->frustum;

			// �۲��� ����
			this->UpdateObserver();
		}
	

	/*
	 @	UpdateObserver
	 */
	Void Camera::UpdateObserver() noexcept
		{
			// ������
			const Matrix3 uvn( this->node.GlobalOrientation() );

			// �۲����
			Vector3& xAxis{ this->matObserver[ 0 ].xyz };
			Vector3& yAxis{ this->matObserver[ 1 ].xyz };
			Vector3& zAxis{ this->matObserver[ 2 ].xyz };

			xAxis = -uvn.Column( 0 );
			yAxis = -uvn.Column( 1 );
			zAxis = -uvn.Column( 2 );

			// λ��
			const Vector3& pos
				{
					this->node.GlobalPosition()
				};

			this->matObserver._14 = -pos.Dot( xAxis );
			this->matObserver._24 = -pos.Dot( yAxis );
			this->matObserver._34 = -pos.Dot( zAxis );

			// �����������
			this->matCamera = this->matProjection
							* this->matObserver;
		}


	/*
	 @	On constructed
	 */
	Void Camera::on_Constructed() noexcept
		{
			// ��ʱ���� �����ڵ� surface
			surface = &s_Window.surface;

			// ��� Scene �б�
			scene = &this->node.GetScene();

			scene->Add( this );

			// ���� Transform ��Ϣ
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
			// ��Ҫ���� ���߳�
			for( Renderer* const renderer : scene->listRenderers )
				{
					renderer->UpdateWVP( this->matCamera );

					renderer->Present( *this->surface );
				}
		}
}