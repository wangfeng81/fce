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
	 @	����
	 */
	Renderer::~Renderer() noexcept
		{
			// �Ƴ� scene render ��
			this->node.GetScene().Remove( this );
		}

	/*
	 @	���� Node
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
	 @	���� material
	 */
	Renderer::Renderer( Node& node,
						Material* const material ) NOEXCEPT
		:
			NodeComponent( node ),
			material( material )
		{
			FC_ASSERT( material );

			// ���� scene render ��
			node.GetScene().Add( this );
		}


	/*
	 @	���� wvp
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