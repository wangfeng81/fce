/*
**
**	Quad renderer.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::renderer
{
	/*
	 @	析构
	 */
	Quad::~Quad() noexcept
		{
			// 取消Transform消息
			this->node.RemoveHandler
				(
					this, &Quad::on_Transformed
				);
		}

	/*
	 @	构造 Buffer
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
	 @	构造 模型
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
			// 监听 Transform 消息
			this->node.AddHandler
				(
					this,
					&Quad::on_Transformed
				);

			// 临时 绑定 相机 Transform 消息
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
	 @	更新
	 */
	Void Quad::Update() noexcept
		{
			// 相机矩阵
			auto& camera = Camera::Get( "Camera" );

			const Matrix4& matCamera{ *camera };

			Matrix4 matWVP = matCamera * this->node.GlobalMatrix();

			// 获取 纹理 尺寸
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

			// 更新 vertex set
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

			// 更新 vertex buffer
			this->vertexBuffer->Update( this->vertexSet );
		}
	
	/*
	 @	Use
	 */
	Void Quad::Use( inner::window::Surface& surface ) const noexcept
		{
			// 应用 材质
			this->material->Apply( surface );

			// 绑定 顶点缓冲
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