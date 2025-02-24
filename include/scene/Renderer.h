/*
**
**	Renderer.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Node.h"

#include "../resource/Material.h"



namespace fce
{
	/*
	 !	Renderer
	 */
	class FC_API Renderer
		:
		public SceneNodeComponent
		{
			/*	Friends
			*/
			friend Camera;

		public:
			/*	Set & Get Material
			*/
			auto* GetMaterial( this auto& ) noexcept;

			Void SetMaterial( Material* const ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			~Renderer() noexcept;

			Renderer( Entity&,
					  const Buffer& );

			Renderer( Node&,
					  Material* const ) NOEXCEPT;

			/*	Update WVP
			*/
			Void UpdateWVP( const Matrix4& ) noexcept;

			/*	Present
			*/
			virtual Void Present( inner::window::Surface& ) const noexcept = 0;

		protected:
			/*	Matrix4
			*/
			Matrix4 matWVP;

			/*	Material
			*/
			Material* material;
		};
}



namespace fce
{
	/*
	 @	Set & Get Material
	 */
	FC_INLINE auto* Renderer::GetMaterial( this auto& self ) noexcept
		{
			return &std::forward_like< decltype( self ) >
					(
						*self.material
					);
		}

	FC_INLINE Void Renderer::SetMaterial( Material* const material ) NOEXCEPT
		{
			FC_ASSERT( material );

			this->material = material;
		}
}


/**
 !	includes
 */
#include "Renderer/PrimitiveRenderer.h"
#include "Renderer/QuadRenderer.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/IndexMeshRenderer.h"