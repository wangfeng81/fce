/*
**
**	Texture.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../resource/image/Image.h"
#include "Sampler.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Texture.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_Texture.h"
#elif defined  FC_METAL
	#include "metal/MT_Texture.h"
#elif defined  FC_GLES
	#include "gles/ES_Texture.h"
#endif


namespace fce
{
	/*
	 !	Texture2
	 */
	class FC_API Texture2 final
		:
		public ObjectContainerSugar
			   < Texture2 >
		{
			/*	Friends
			*/
			friend Graphics;

			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						Texture2, Path );

			/*	Allocated by object container
			*/
			OBJECT_CONTAINER_SUGAR( Texture2 );

		public:
			/*	Asset ( Syntax sugar )
			*/
			ASSET_SYNTAX_SUGAR_DECL;

			/*	Implementation
			*/
			using Impl = inner::texture::Impl;

			friend Impl;

			Impl impl;

		public:
			/*	Get info
			*/
			using Info = Image::Info;

			const Info* operator -> () const noexcept;

			const Info& GetInfo() const noexcept;

			/*	Attributes
			*/
			[[nodiscard]] Int16 Height() const noexcept;

			[[nodiscard]] Int16 Width() const noexcept;

			[[nodiscard]] COLOR Color() const noexcept;

			[[nodiscard]] Int16 Pitch() const noexcept;

			[[nodiscard]] Int32 Area() const noexcept;

			[[nodiscard]] Size Size() const noexcept;

			/*	Get native handle
			*/
			[[nodiscard]] const auto& GetHandle() const noexcept;

			/*	Convert color format
			*/
			CONVERT_IMPL_ENUM_VALUE( COLOR );

		public:
			/*	Update
			*/
			RESULT Update( const Image&,
						   const Point& dstOffset = 0 );

			RESULT Update( const Image&,
						   const fce::Size&,
						   const Point& srcOffset,
						   const Point& dstOffset = 0 );

		protected:
			/*	Constructor & Destructor
			*/
			~Texture2() noexcept;

			Texture2( RESULT&,
					  const Buffer& );

			Texture2( RESULT&,
					  const Info& );

			Texture2( RESULT&,
					  const fce::Size&,
					  const COLOR );

			Texture2( RESULT&,
					  const UInt16 width,
					  const UInt16 height,
					  const COLOR );

			Texture2( RESULT&,
					  const Image& );

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize( const Info& );

		private:
			/*	Info
			*/
			Info info;
		};
}



/**
 !	class Texture2
 */
namespace fce
{
	/*
	 @	Get Info
	 */
	FC_INLINE const Texture2::Info* Texture2::operator -> () const noexcept
		{
			return &this->info;
		}

	FC_INLINE const Texture2::Info& Texture2::GetInfo() const noexcept
		{
			return this->info;
		}

	/*
	 @	Attributes
	 */
	FC_INLINE Int16 Texture2::Height() const noexcept
		{
			return this->info.height;
		}

	FC_INLINE Int16 Texture2::Width() const noexcept
		{
			return this->info.width;
		}

	FC_INLINE COLOR Texture2::Color() const noexcept
		{
			return this->info.color;
		}

	FC_INLINE Int16 Texture2::Pitch() const noexcept
		{
			return this->info.Pitch();
		}

	FC_INLINE Int32 Texture2::Area() const noexcept
		{
			return this->info.Area();
		}

	FC_INLINE Size Texture2::Size() const noexcept
		{
			return { this->info.width,
					 this->info.height };
		}

	/*
	 @	Get native handle
	 */
	FC_INLINE const auto& Texture2::GetHandle() const noexcept
		{
			return this->impl.GetHandle();
		}
}