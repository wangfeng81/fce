/*
**
**	CAnimation.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#include <stdio.h>
#include "../../interface/graphics/Texture2DCache.h"
#include "../../interface/graphics/ShaderCache.h"
#include "../../interface/graphics/Graphics.h"

#include "Animation.h"


namespace fce
{
	/*
	 !	GetTexture
	*/
	ITexture2D* CAnimation::GetTexture( UInt8 frame ) const
	{
		return Texture2DCache.Get(
				this->frmFiles[ frame ] );
	}


	/*
	 !	Load
	*/
	Void CAnimation::Load( const stl::string dir )
	{
		for( UInt32 c = 0; ; ++c )
		{
			Char str[ 255 ];
			sprintf( str, "%s/%05d.png", dir.c_str(), c );

			if( ! Texture2DCache.Add( str ) )
				break;

			this->frmFiles.emplace_back( str );
		}

		this->Reset();
	}


	/*
	 !	Reset
	*/
	Void CAnimation::Reset()
	{
		this->frmCur = 0;
		this->timer.Startup();
	}


	/*
	 !	Render
	*/
	Void CAnimation::Render()
	{
		if( ! this->paused )
		{
			if( this->timer.isTimeOut( this->interval ) )
			{
				if( this->isEnd() )
				{
					if( this->looped )
						this->frmCur = 0;
				}
				else
				{
					++this->frmCur;
				}

				this->timer.Startup();
			}
		}

		// äÖÈ¾
		if( this->alpha >= 1.0f )
		{
			ShaderCache.Use( ShaderCache.VS_WVP_TEXTURE2D,
							ShaderCache.FS_TEXTURE2D_ALPHA_TEST );
		}
		else
		{
			ShaderCache.Use( ShaderCache.VS_WVP_TEXTURE2D,
							ShaderCache.FS_TEXTURE2D_COLOR );

			ShaderCache.SetVal( "u_color",
								Color4F( 1.0f, 1.0f ,1.0f, this->alpha ) );
		}

		ShaderCache.SetVal( "u_wvpMatrix",
							Graphics.GetOrthoMatrix() );

		// »ñÈ¡ÎÆÀí
		if( ITexture2D* tex2D =
			Texture2DCache.Get( this->frmFiles[ this->frmCur ] ) )
		{
			if( this->mirrored )
				tex2D->MirrorRender( this->pos );
			else
				tex2D->Render( this->pos );
		}
	}
}