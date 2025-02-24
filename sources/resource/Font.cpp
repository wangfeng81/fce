/*
**
**	CFontCache class.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/
#include "FontCache.h"

#include "../../include/graphics/Graphics.h"
#include "../../include/graphics/ShaderCache.h"

#include "Texture2D.h"


namespace fce
{
	/*
	 !	GlyphCache instance.
	 */
	FontCache& g_FontCache = SHARED( CFontCache );


	/*
	 !	Constructor.
	*/
	CFontCache::CFontCache()
	{
		// ��ʼ�� freetype ��
		FT_Init_FreeType( &this->ftLibrary );

		// ��ʼ�� freetype ������
		FTC_Manager_New( this->ftLibrary, 0, 0, 0,
						FTC_Face_Requester_Custom,
						NULL, &this->ftcManager );

		// ��ʼ�� freetype �ַ�ӳ��
		FTC_CMapCache_New( this->ftcManager,
						&this->ftcCMapCache );

		// �����ʴ�
		FT_Stroker_New( this->ftLibrary,
						&this->ftStroker );

		// Ĭ��Ϊ�����
		this->SetSmooth( True );
	}


	/*
	 !	Destructor.
	*/
	CFontCache::~CFontCache()
	{
		// ��� Glyph
		this->Clear();

		// �ͷ� freetype ��Դ
		FT_Stroker_Done( this->ftStroker );
		FTC_Manager_Done( this->ftcManager );
		FT_Done_FreeType( this->ftLibrary );
	}


	/*
	 !	SetFTFont
	*/
	FT_Error CFontCache::SetFTFont()
	{
		// ���� load_flags
		this->ftLoadFlags = this->smooth			|| 
							this->currFont->bold	|| 
							this->currFont->italic	|| 
							this->currFont->outline	?
							FT_LOAD_NO_BITMAP : FT_LOAD_DEFAULT;

		// ���ñ任
		if( this->currFont->italic )
		{
			ftMatrix.xx = 0x10000L;
			ftMatrix.yy = 0x10000L;
			ftMatrix.yx = 0;
			ftMatrix.xy = (FT_Fixed)
						( 0x10000L * this->currFont->italic );
		}
		
		// �ӻ����ȡ face
		FTC_ScalerRec ftcScalerRec =
		{
			(FTC_FaceID)this->currFont,
			(FT_UInt)this->currFont->size.width,
			(FT_UInt)this->currFont->size.height,
			1, 0, 0
		};

		return FTC_Manager_LookupSize( this->ftcManager,
							&ftcScalerRec, &this->ftSize );
	}


	/*
	 !	Generate FTOutline
	*/
	FT_Glyph CFontCache::GenerateFTOutline( FT_GlyphSlot ftGlyphSlot,
											FT_Fixed outline )
	{
		// ���� FTStroker
		FT_Stroker_Set( this->ftStroker, outline,
					   FT_STROKER_LINECAP_ROUND,
					   FT_STROKER_LINEJOIN_ROUND,
					   0 );

		// ��ȡ ftGlyph
		FT_Glyph ftGlyph;

		if( FT_Get_Glyph( ftGlyphSlot, &ftGlyph ) )
			return NULL;

		// ���� ���������
		if( FT_Glyph_StrokeBorder( &ftGlyph,
									this->ftStroker,
									False, True ) )
		{
			FT_Done_Glyph( ftGlyph );
			return NULL;
		}

		return ftGlyph;
	}


	/*
	 !	Generate Glyph
	*/
	const Glyph* CFontCache::GenerateGlyph( const FT_Bitmap* ftBitmap,
											FT_Int left, FT_Int top,
											FT_Pos advance )
	{
		// ���� Glyph
		if( Glyph* glyph = new Glyph() )
		{
			// ���� �ո� ���� ��buff
			if( ftBitmap->buffer )
			{
				// ���� monoλͼ
				if( FT_PIXEL_MODE_MONO == ftBitmap->pixel_mode )
				{
					// ���� bitmap
					CBitmap bitmap( ftBitmap->width,
									ftBitmap->rows,
									COLOR_1 );

					Byte* bmpPtr = (Byte*)*bitmap;

					Byte* rowPtr = ftBitmap->buffer;
					Int16 cols = ftBitmap->width >> 3;
					Int8 mod = ftBitmap->width & 7;

					// �� row ����
					for( Int16 row = 0; row < ftBitmap->rows; ++row )
					{
						// ��Byte���� ѭ������ one row
						Byte* colPtr = rowPtr;

						for( Int16 col = 0; col < cols; ++col )
						{
							// ����һ�� Byte�� �ĵ���
							for( Int8 c = 0; c < 8; ++c )
								{
									*bmpPtr++ = (Byte)( ( *colPtr & ( 0x80 >> c ) )
															? 255 : 0 );
								}

							++colPtr;
						}

						// �������µĲ���һ�� Byte �ĵ���
						for( Int8 c = 0; c < mod; ++c )
							{
								*bmpPtr++ = (Byte)( ( *colPtr & ( 0x80 >> c ) )
														? 255 : 0 );
							}

						// �絽��һ��
						rowPtr += ftBitmap->pitch;
					}

					// ���� ����
					glyph->tex2D = g_Graphics.CreateTexture2D( &bitmap );
				}
				// ���� �����λͼ
				else
				{
					// ���� ����
					glyph->tex2D = g_Graphics.CreateTexture2D(
										Size( ftBitmap->width, ftBitmap->rows ),
										COLOR_1, ftBitmap->buffer );
				}
			}

			// ���� ��������
			glyph->advance = (Int16)advance;
			glyph->offset = (Int16)left;
			glyph->ascent = (Int16)top;
			glyph->descent = ftBitmap->rows - top;

			return glyph;
		}

		return NULL;
	}


	/*
	 !	FTC_Face_Requester_Custom
	*/
	FT_Error CFontCache::FTC_Face_Requester_Custom( FTC_FaceID face_id,
													FT_Library library,
													FT_Pointer request_data,
													FT_Face *aface )
	{
		FTC_FaceID_Custom* ftcFaceID =
				(FTC_FaceID_Custom*)face_id;

		return FT_New_Face( library,
							ftcFaceID->name.c_str(),
							0, aface );
	}


	/*
	 !	SetFont
	*/
	const Font* CFontCache::SetFont( const Font& font )
	{
		// ���ں��� freetype �� font ����
		this->ftSize = NULL;

		// ���� map �ڲ���
		FontMap::iterator p =
			this->mapFont.find( font );

		// �Ҳ��������
		if( p == this->mapFont.end() )
		{
			this->mapFont[ font ] = new GlyphMap();
			p = this->mapFont.find( font );
		}

		// ���浱ǰ �� ���� map
		this->currGlyphMap = p->second;

		return this->currFont = &p->first;
	}


	/*
	 !	SetFont
	*/
	Void CFontCache::SetSmooth( Bool smooth )
	{
		this->smooth = smooth;

		// ���� render_mode
		this->ftRenderMode = smooth
							? FT_RENDER_MODE_NORMAL
							: FT_RENDER_MODE_MONO;

		// ���֮ǰ��
		this->Clear();
	}


	/*
	 !	Add Glyph
	*/
	const Glyph* CFontCache::AddGlyph( WChar code )
	{
		// ���� freetype �� font
		if( ! this->ftSize )
			if( this->SetFTFont() )
				return NULL;

		// ��ʼ�� glyph
		const Glyph* glyph = NULL;

		// ͨ�� unicode ��ȡ ����
		if( FT_UInt index =
				FTC_CMapCache_Lookup( this->ftcCMapCache,
									(FTC_FaceID)this->currFont,
									-1, code ) )
		{
			// ���� ���Σ�������ڱ任 �� ����ʹ��λͼ����
			if( ! FT_Load_Glyph( this->ftSize->face,
								index, this->ftLoadFlags ) )
			{
				// ���β�
				FT_GlyphSlot ftGlyphSlot = this->ftSize->face->glyph;

				// ������
				FT_Outline* ftOutline = &ftGlyphSlot->outline;

				// ���� б��
				if( this->currFont->italic )
					FT_Outline_Transform( ftOutline,
										&this->ftMatrix );

				// ���� ����
				if( this->currFont->bold )
					FT_Outline_EmboldenXY( ftOutline,
										this->currFont->bold,
										this->currFont->bold );

				// ��Ⱦ ������
				if( this->currFont->outline )
				{
					// ��ȡ ���������
					if( FT_Glyph ftGlyph =
							this->GenerateFTOutline( ftGlyphSlot,
													this->currFont->outline ) )
					{
						// ��Ⱦ ������Ϊ bitmap
						if( ! FT_Glyph_To_Bitmap( &ftGlyph,
												this->ftRenderMode, NULL, True ) )
						{
							// ת�� ftGlyph Ϊ FT_BitmapGlyph
							FT_BitmapGlyph ftBitmapGlyph =
								reinterpret_cast<FT_BitmapGlyph>( ftGlyph );

							// ���� glyph
							glyph = this->GenerateGlyph( &ftBitmapGlyph->bitmap,
														ftBitmapGlyph->left,
														ftBitmapGlyph->top,
														ftGlyph->advance.x >> 16 );
						}

						// �ͷ� ��ʱ�� ftGlyph
						FT_Done_Glyph( ftGlyph );
					}
				}
				// ��Ⱦ ����
				else
				{
					// ����� λͼ��ʽ ������Ⱦ
					if( FT_GLYPH_FORMAT_BITMAP != ftGlyphSlot->format )
					{
						// ��Ⱦ ����Ϊ bitmap
						if( FT_Render_Glyph( ftGlyphSlot,
											this->ftRenderMode ) )
							return NULL;
					}

					// ���� glyph
					glyph = this->GenerateGlyph( &ftGlyphSlot->bitmap,
												ftGlyphSlot->bitmap_left,
												ftGlyphSlot->bitmap_top,
												ftGlyphSlot->advance.x >> 6 );
				}

				// ����glyph�� map
				if( glyph )
				{
					(*this->currGlyphMap)[ code ] = glyph;
				}
			}
		}

		return glyph;
	}


	/*
	 !	Get Glyph
	*/
	const Glyph* CFontCache::GetGlyph( WChar code )
	{
		// ���� map �ڲ���
		GlyphMap::iterator p =
			this->currGlyphMap->find( code );

		if( p != this->currGlyphMap->end() )
		{
			return p->second;
		}
		// �Ҳ��������
		else
		{
			return this->AddGlyph( code );
		}
	}


	/*
	 !	Del Glyph
	*/
	Bool CFontCache::DelGlyph( WChar code )
	{
		// ���� map �ڲ���
		GlyphMap::iterator p =
			this->currGlyphMap->find( code );

		if( p != this->currGlyphMap->end() )
		{
			delete p->second;
			this->currGlyphMap->erase( p );

			return True;
		}

		return False;
	}


	/*
	 !	Clear
	*/
	Void CFontCache::Clear()
	{
		for( FontMap::iterator
			p = this->mapFont.begin();
			p != this->mapFont.end();
			++p )
		{
			for( GlyphMap::iterator
				p2 = p->second->begin();
				p2 != p->second->end();
				++p2 )
			{
				delete p2->second;
			}

			delete p->second;
		}

		this->mapFont.clear();
	}


	/*
	 !	GetBox
	*/
	Int16 CFontCache::GetBox( const WChar* const str, Size& size )
	{
		// �ߴ��ʼ��Ϊ 0
		size = 0;

		// ���� ��� Ascent �� Descent
		Int16 maxAscent = 0;
		Int16 maxDescent = 0;

		// ���� �ַ���
		for( const WChar* const s = str; *s; ++s )
		{
			// ��ȡ glyph
			if( const Glyph* glyph = this->GetGlyph( *s ) )
			{
				// ��� ���� �����ַ� ����֮��
				size.width += glyph->advance;

				// ������� ascent �� descent
				if( maxAscent < glyph->ascent )
					maxAscent = glyph->ascent;
				if( maxDescent < glyph->descent )
					maxDescent = glyph->descent;
			}
		}

		// �߶� ���� ��� ascent �� descent ֮��
		size.height = maxAscent + maxDescent;

		// baseLine ����λ��Ϊ maxAscent
		return maxAscent;
	}


	/*
	 !	Render
	*/
	Void CFontCache::Render( const WChar* const str,
							const Color4F& color,
							const Point& pos,
							const Rect* clip,
							const Color4F& underline,
							const Color4F& strikethrough )
	{
		g_ShaderCache.Use(g_ShaderCache.VS_WVP_TEXTURE2D,
						g_ShaderCache.PS_TEXT2D_AA );

		g_ShaderCache.SetVal( "u_wvpMatrix",
			g_Graphics.GetOrthoMatrix() );
		g_ShaderCache.SetVal( "u_color", color );

		// ��������
		Int16 advance = 0;
		const Glyph* glyph;

		for( const WChar* const s = str; *s; ++s )
		{
			if( glyph = this->GetGlyph( *s ) )
			{
				// �ո����û�� λͼ
				if( glyph->tex2D )
				{
					//Point pt( advance + glyph->offset,
					//		-glyph->ascent );

					//((CTexture2D*)glyph->tex2D)->Render( pos + pt );
				}

				// ����
				advance += glyph->advance;
			}
		}

		// �»���
		//if( underline.a )
		//{
		//	g_ShaderCache.Use(g_ShaderCache.VS_WVP,
		//					g_ShaderCache.PS_COLOR );

		//	g_ShaderCache.SetVal( "u_wvpMatrix",
		//						g_Graphics.GetOrthoMatrix() );
		//	g_ShaderCache.SetVal( "u_color", underline );

		//	// ���� ��ȶ�
		//	GLfloat lineWidth = ( this->currFont->bold + this->currFont->outline ) * 0.02f;
		//	glLineWidth( Max( lineWidth, 1.0f ) );

		//	// ���� �߶ζ�������
		//	Vector2< GLshort > vertices[ 2 ];
		//	{
		//		vertices[0].Set( pos.x + this->GetGlyph( *str )->offset,
		//						pos.y + (Int16)( this->currFont->bold * 0.02f ) );

		//		vertices[1].Set( vertices[0].x + advance + glyph->tex2D->GetWidth() - glyph->advance,
		//						vertices[0].y );
		//	}

		//	// ����GPU��Ⱦ
		//	glVertexAttribPointer( g_ShaderCache.SLOT_POSITION, 2, GL_SHORT, GL_FALSE, 0, vertices );

		//	glDrawArrays( GL_LINES, 0, 2 );
		//}

		// ɾ����
		if( strikethrough.a )
		{
		}
	}
}