/*
**
**	Window surface.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "RenderPipeline.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_WindowSurface.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_WindowSurface.h"
#elif defined  FC_METAL
	#include "metal/MT_WindowSurface.h"
#elif defined  FC_GLES
	#include "gles/ES_WindowSurface.h"
#endif


namespace fce::inner::window
{
	/*
	 !	Surface
	 */
	class FC_API Surface final
		{
			/*	Friends
			*/
			friend Window;

			friend surface::Impl;

		public:
			/*	Attached window
			*/
			Window& window;

			/*	Implementation
			*/
			using Impl = surface::Impl;

			Impl impl;

		public:
			/*	Get bounds
			*/
			[[nodiscard]] Size GetSize() const noexcept;

			[[nodiscard]] Rect GetBounds() const noexcept;

			[[nodiscard]] Point GetPosition() const noexcept;

			/*	Set bounds
			*/
			Void SetSize( const Size& ) noexcept;

			Void SetPosition( const Point& ) noexcept;

			Void SetBounds( const Rect& ) noexcept;

			Void SetBounds( const Point&,
							const Size& ) noexcept;

			/*	Snapshot
			*/
			Bool Snapshot( Image& ) const noexcept;

			/*	Bind resource
			*/
			Void Bind( const IndexBuffer* const ) NOEXCEPT;

			Void Bind( const VertexBuffer* const ) NOEXCEPT;

			Void Bind( const DescriptorSet* const ) NOEXCEPT;

			Void Bind( const RenderPipeline* const ) NOEXCEPT;

			/*	Update dynamic state
			*/
			template< RenderPipeline::DYNAMIC_STATE >

			Void Update( auto&&... ) NOEXCEPT;

			/*	Set clear color
			*/
			Void SetClearColor( const Color4F& = 0 ) noexcept;

			/*	Push constant
			*/
			template< typename T >

			Void PushConstant( const T& value,
							   const Shader::TYPE,
							   const UInt32 offset = 0,
							   const UInt32 size = sizeof( T ) ) NOEXCEPT;

			/*	Draw
			*/
			Void Draw( const UInt32 vertCount,
					   const UInt32 instCount,
					   const UInt32 firstVert = 0,
					   const UInt32 firstInst = 0 ) NOEXCEPT;

			Void DrawIndex( const UInt32 indexCount,
							const UInt32 instanceCount,
							const UInt32 firstIndex = 0,
							const UInt32 firstInstance = 0 ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			~Surface() = default;

			Surface( Window& ) noexcept;

			/*	Begin & End
			*/
			Void End() noexcept;

			Void Begin() noexcept;

			/*	Create & Destroy
			*/
			Void Destroy() noexcept;

			RESULT Create() noexcept;

			RESULT Rebuild() noexcept;

			/*	On resize
			*/
			Bool on_Resize() noexcept;

		private:
			/*	Visible
			*/
			Bool visible;

			/*	Current states
			*/
			struct
				{
					const RenderPipeline* renderPipeline;
				}
			current;
		};
}


/**
 !	class WindowSurface
 */
namespace fce
{
	using WindowSurface = inner::window::Surface;
}


/**
 !	class Surface
 */
namespace fce::inner::window
{
	/*
	 @	Bind index buffer
	 */
	FC_INLINE Void Surface::Bind( const IndexBuffer* const ib ) NOEXCEPT
		{
			FC_ASSERT( ib );

			this->impl.Bind( ib->impl );
		}

	/*
	 @	Bind vertex buffer
	 */
	FC_INLINE Void Surface::Bind( const VertexBuffer* const vb ) NOEXCEPT
		{
			FC_ASSERT( vb );

			this->impl.Bind( vb->impl );
		}

	/*
	 @	Bind descriptor set
	 */
	FC_INLINE Void Surface::Bind( const DescriptorSet* const ds ) NOEXCEPT
		{
			FC_ASSERT( ds );

			this->impl.Bind( ds->impl );
		}
}



namespace fce::inner::window
{
	/*
	 @	Push constant
	 */
	template< typename T >

	FC_INLINE Void Surface::PushConstant( const T&			 value,
										  const Shader::TYPE type,
										  const UInt32		 offset,
										  const UInt32		 size ) NOEXCEPT
		{
			this->impl.PushConstant( &value,
									 Shader::Convert( type ),
									 offset, size );
		}

	/*
	 @	Update dynamic states
	 */
	template< RenderPipeline::DYNAMIC_STATE STATE >

	FC_INLINE Void Surface::Update( auto&& ...args ) NOEXCEPT
		{
			if constexpr( STATE == RenderPipeline::DYNAMIC_STATE::BLEND_CONSTANTS )
				{
					this->impl.Update_BlendConstants( args... );
				}
		}
}



namespace fce::inner::window
{
	/*
	 @	Draw
	 */
	FC_INLINE Void Surface::Draw( const UInt32 vertCount,
								  const UInt32 instCount,
								  const UInt32 firstVert,
								  const UInt32 firstInst ) NOEXCEPT
		{
			this->impl.Draw( vertCount,
							 instCount,
							 firstVert,
							 firstInst );
		}

	/*
	 @	DrawIndex
	 */
	FC_INLINE Void Surface::DrawIndex( const UInt32 indexCount,
									   const UInt32 instanceCount,
									   const UInt32 firstIndex,
									   const UInt32 firstInstance ) NOEXCEPT
		{
			this->impl.DrawIndex( indexCount,
								  instanceCount,
								  firstIndex,
								  firstInstance );
		}
}