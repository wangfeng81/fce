/*
**
**	Quad renderer.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::renderer
{
	/*
	 @	����
	 */
	Quad::~Quad() noexcept
		{
			// ȡ��Transform��Ϣ
			this->node.RemoveHandler
				(
					this, &Quad::on_Transformed
				);
		}

	/*
	 @	���� Buffer
	 */
	Quad::Quad( Entity& entity,
				const Buffer& buf )
		:
			Primitive( entity, buf ),
			vertexSet( UInt16( 4 ),
					   UInt16( sizeof( Vertex ) ) )
		{
			*this << buf;

			this->on_Constructed();
		}

	/*
	 @	���� ģ��
	 */
	Quad::Quad( Node& node,
				Material* const material ) NOEXCEPT
		:
			Primitive( node, material,
					   VertexBuffer::Generate( UInt16( 4 ),
											   UInt16( sizeof( Vertex ) ) ) ),
						vertexSet( UInt16( 4 ),
								   UInt16( sizeof( Vertex ) ) )
		{
			this->on_Constructed();
		}
}



namespace fce::renderer
{
	/*
	 @	On constructed
	 */
	Void Quad::on_Constructed() noexcept
		{
			// ���� Transform ��Ϣ
			this->node.AddHandler
				(
					this,
					&Quad::on_Transformed
				);

			// ��ʱ �� ��� Transform ��Ϣ
			Camera::Get( "Camera" )->AddHandler
			(
				this,
				&Quad::on_Transformed
			);

			this->Update();
		}

	/*
	 @	Respond transform
	 */
	Void Quad::on_Transformed( Node::EVENT,
							   Node& ) noexcept
		{
			this->Update();
		}


	/*
	 @	����
	 */
	Void Quad::Update() noexcept
		{
			// �������
			auto& camera = Camera::Get( "Camera" );

			const Matrix4& matCamera{ *camera };

			Matrix4 matWVP = matCamera * this->node.GlobalMatrix();

			// ��ȡ ���� �ߴ�
			const Size size
				{
					static_cast< material::Texture* >( this->material )->GetTexture()->Size()
				};

			//
			const Vector4 vertices[]
				{
					Vector4( 0.0f, 1.0f ),
					Vector4( size.x, 0.0f, 0.0f, 1.0f ),
					Vector4( 0.0f, size.y, 0.0f, 1.0f ),
					Vector4( size, 0.0f, 1.0f )
				};

			// ���� vertex set
			Vector4* vertex{ this->vertexSet.Data< Vector4 >() };
			Vector4* vertex2{ this->vertexSet.Data< Vector4 >() };

			for( UInt8 count{ 0 }; count < 4; ++count )
				{
					vertex[ count ] = vertices[ count ] * matWVP;
					//vertex2[ count ].w = 1.0f;
				}

			Vector4& f0=vertex2[ 0];
			Vector4& f1=vertex2[ 1 ];
			Vector4& f2=vertex2[ 2 ];
			Vector4& f3=vertex2[ 3 ];

			// ���� vertex buffer
			this->vertexBuffer->Update( this->vertexSet );
		}
	
	/*
	 @	Use
	 */
	Void Quad::Use( inner::window::Surface& surface ) const noexcept
		{
			// Ӧ�� ����
			this->material->Apply( surface );

			// �� ���㻺��
			surface.Bind( this->vertexBuffer );
		}


	/*
	 @	Present
	 */
	Void Quad::Present( inner::window::Surface& surface ) const noexcept
		{
			this->Use( surface );

			surface.Draw( 4, 1 );
		}
}