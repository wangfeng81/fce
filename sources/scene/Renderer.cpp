/*
**
**	Renderer.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/platform/Application.h"


namespace fce
{
	/*
	 @	析构
	 */
	Renderer::~Renderer() noexcept
		{
			// 移除 scene render 表
			this->node.GetScene().Remove( this );
		}

	/*
	 @	构造 Node
	 */
	Renderer::Renderer( Entity& entity,
						const Buffer& buf )
		:
			Renderer( static_cast< Node& >( entity ),
					  Material::Reflect( buf ) )
		{
			FC_ASSERT( typeid( entity ) == typeid( Node ) );
		}

	/*
	 @	构造 material
	 */
	Renderer::Renderer( Node& node,
						Material* const material ) NOEXCEPT
		:
			NodeComponent( node ),
			material( material )
		{
			FC_ASSERT( material );

			// 加入 scene render 表
			node.GetScene().Add( this );
		}


	/*
	 @	更新 wvp
	 */
	Void Renderer::UpdateWVP( const Matrix4& matCamera ) noexcept
		{
			this->matWVP = matCamera
						 * this->node.GlobalMatrix();
		}
}


/**
 !	includes
 */
#include "Renderer/PrimitiveRenderer.cxx"
#include "Renderer/QuadRenderer.cxx"
#include "Renderer/MeshRenderer.cxx"
#include "Renderer/IndexMeshRenderer.cxx"