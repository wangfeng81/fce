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
		// 初始化 freetype 库
		FT_Init_FreeType( &this->ftLibrary );

		// 初始化 freetype 管理器
		FTC_Manager_New( this->ftLibrary, 0, 0, 0,
						FTC_Face_Requester_Custom,
						NULL, &this->ftcManager );

		// 初始化 freetype 字符映射
		FTC_CMapCache_New( this->ftcManager,
						&this->ftcCMapCache );

		// 创建笔触
		FT_Stroker_New( this->ftLibrary,
						&this->ftStroker );

		// 默认为抗锯齿
		this->SetSmooth( True );
	}


	/*
	 !	Destructor.
	*/
	CFontCache::~CFontCache()
	{
		// 清空 Glyph
		this->Clear();

		// 释放 freetype 资源
		FT_Stroker_Done( this->ftStroker );
		FTC_Manager_Done( this->ftcManager );
		FT_Done_FreeType( this->ftLibrary );
	}


	/*
	 !	SetFTFont
	*/
	FT_Error CFontCache::SetFTFont()
	{
		// 设置 load_flags
		this->ftLoadFlags = this->smooth			|| 
							this->currFont->bold	|| 
							this->currFont->italic	|| 
							this->currFont->outline	?
							FT_LOAD_NO_BITMAP : FT_LOAD_DEFAULT;

		// 设置变换
		if( this->currFont->italic )
		{
			ftMatrix.xx = 0x10000L;
			ftMatrix.yy = 0x10000L;
			ftMatrix.yx = 0;
			ftMatrix.xy = (FT_Fixed)
						( 0x10000L * this->currFont->italic );
		}
		
		// 从缓存获取 face
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
		// 设置 FTStroker
		FT_Stroker_Set( this->ftStroker, outline,
					   FT_STROKER_LINECAP_ROUND,
					   FT_STROKER_LINEJOIN_ROUND,
					   0 );

		// 获取 ftGlyph
		FT_Glyph ftGlyph;

		if( FT_Get_Glyph( ftGlyphSlot, &ftGlyph ) )
			return NULL;

		// 绘制 外框轮廓线
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
		// 生成 Glyph
		if( Glyph* glyph = new Glyph() )
		{
			// 可能 空格 会是 空buff
			if( ftBitmap->buffer )
			{
				// 拷贝 mono位图
				if( FT_PIXEL_MODE_MONO == ftBitmap->pixel_mode )
				{
					// 设置 bitmap
					CBitmap bitmap( ftBitmap->width,
									ftBitmap->rows,
									COLOR_1 );

					Byte* bmpPtr = (Byte*)*bitmap;

					Byte* rowPtr = ftBitmap->buffer;
					Int16 cols = ftBitmap->width >> 3;
					Int8 mod = ftBitmap->width & 7;

					// 按 row 拷贝
					for( Int16 row = 0; row < ftBitmap->rows; ++row )
					{
						// 按Byte对齐 循环处理 one row
						Byte* colPtr = rowPtr;

						for( Int16 col = 0; col < cols; ++col )
						{
							// 处理一个 Byte内 的点阵
							for( Int8 c = 0; c < 8; ++c )
								{
									*bmpPtr++ = (Byte)( ( *colPtr & ( 0x80 >> c ) )
															? 255 : 0 );
								}

							++colPtr;
						}

						// 处理余下的不足一个 Byte 的点阵
						for( Int8 c = 0; c < mod; ++c )
							{
								*bmpPtr++ = (Byte)( ( *colPtr & ( 0x80 >> c ) )
														? 255 : 0 );
							}

						// 跨到下一行
						rowPtr += ftBitmap->pitch;
					}

					// 生成 纹理
					glyph->tex2D = g_Graphics.CreateTexture2D( &bitmap );
				}
				// 拷贝 抗锯齿位图
				else
				{
					// 生成 纹理
					glyph->tex2D = g_Graphics.CreateTexture2D(
										Size( ftBitmap->width, ftBitmap->rows ),
										COLOR_1, ftBitmap->buffer );
				}
			}

			// 设置 布局数据
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
		// 用于后面 freetype 的 font 设置
		this->ftSize = NULL;

		// 先在 map 内查找
		FontMap::iterator p =
			this->mapFont.find( font );

		// 找不到则添加
		if( p == this->mapFont.end() )
		{
			this->mapFont[ font ] = new GlyphMap();
			p = this->mapFont.find( font );
		}

		// 保存当前 的 字形 map
		this->currGlyphMap = p->second;

		return this->currFont = &p->first;
	}


	/*
	 !	SetFont
	*/
	Void CFontCache::SetSmooth( Bool smooth )
	{
		this->smooth = smooth;

		// 设置 render_mode
		this->ftRenderMode = smooth
							? FT_RENDER_MODE_NORMAL
							: FT_RENDER_MODE_MONO;

		// 清空之前的
		this->Clear();
	}


	/*
	 !	Add Glyph
	*/
	const Glyph* CFontCache::AddGlyph( WChar code )
	{
		// 设置 freetype 的 font
		if( ! this->ftSize )
			if( this->SetFTFont() )
				return NULL;

		// 初始化 glyph
		const Glyph* glyph = NULL;

		// 通过 unicode 获取 索引
		if( FT_UInt index =
				FTC_CMapCache_Lookup( this->ftcCMapCache,
									(FTC_FaceID)this->currFont,
									-1, code ) )
		{
			// 加载 字形，如果存在变换 则 不能使用位图字形
			if( ! FT_Load_Glyph( this->ftSize->face,
								index, this->ftLoadFlags ) )
			{
				// 字形槽
				FT_GlyphSlot ftGlyphSlot = this->ftSize->face->glyph;

				// 轮廓线
				FT_Outline* ftOutline = &ftGlyphSlot->outline;

				// 设置 斜体
				if( this->currFont->italic )
					FT_Outline_Transform( ftOutline,
										&this->ftMatrix );

				// 设置 粗体
				if( this->currFont->bold )
					FT_Outline_EmboldenXY( ftOutline,
										this->currFont->bold,
										this->currFont->bold );

				// 渲染 轮廓线
				if( this->currFont->outline )
				{
					// 获取 外框轮廓线
					if( FT_Glyph ftGlyph =
							this->GenerateFTOutline( ftGlyphSlot,
													this->currFont->outline ) )
					{
						// 渲染 轮廓线为 bitmap
						if( ! FT_Glyph_To_Bitmap( &ftGlyph,
												this->ftRenderMode, NULL, True ) )
						{
							// 转换 ftGlyph 为 FT_BitmapGlyph
							FT_BitmapGlyph ftBitmapGlyph =
								reinterpret_cast<FT_BitmapGlyph>( ftGlyph );

							// 生成 glyph
							glyph = this->GenerateGlyph( &ftBitmapGlyph->bitmap,
														ftBitmapGlyph->left,
														ftBitmapGlyph->top,
														ftGlyph->advance.x >> 16 );
						}

						// 释放 临时的 ftGlyph
						FT_Done_Glyph( ftGlyph );
					}
				}
				// 渲染 字形
				else
				{
					// 如果是 位图格式 则不用渲染
					if( FT_GLYPH_FORMAT_BITMAP != ftGlyphSlot->format )
					{
						// 渲染 字形为 bitmap
						if( FT_Render_Glyph( ftGlyphSlot,
											this->ftRenderMode ) )
							return NULL;
					}

					// 生成 glyph
					glyph = this->GenerateGlyph( &ftGlyphSlot->bitmap,
												ftGlyphSlot->bitmap_left,
												ftGlyphSlot->bitmap_top,
												ftGlyphSlot->advance.x >> 6 );
				}

				// 保存glyph到 map
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
		// 先在 map 内查找
		GlyphMap::iterator p =
			this->currGlyphMap->find( code );

		if( p != this->currGlyphMap->end() )
		{
			return p->second;
		}
		// 找不到则添加
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
		// 先在 map 内查找
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
		// 尺寸初始化为 0
		size = 0;

		// 计算 最大 Ascent 和 Descent
		Int16 maxAscent = 0;
		Int16 maxDescent = 0;

		// 遍历 字符串
		for( const WChar* const s = str; *s; ++s )
		{
			// 获取 glyph
			if( const Glyph* glyph = this->GetGlyph( *s ) )
			{
				// 宽度 等于 所有字符 步进之和
				size.width += glyph->advance;

				// 计算最大 ascent 和 descent
				if( maxAscent < glyph->ascent )
					maxAscent = glyph->ascent;
				if( maxDescent < glyph->descent )
					maxDescent = glyph->descent;
			}
		}

		// 高度 等于 最大 ascent 和 descent 之和
		size.height = maxAscent + maxDescent;

		// baseLine 基线位置为 maxAscent
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

		// 绘制文字
		Int16 advance = 0;
		const Glyph* glyph;

		for( const WChar* const s = str; *s; ++s )
		{
			if( glyph = this->GetGlyph( *s ) )
			{
				// 空格可能没有 位图
				if( glyph->tex2D )
				{
					//Point pt( advance + glyph->offset,
					//		-glyph->ascent );

					//((CTexture2D*)glyph->tex2D)->Render( pos + pt );
				}

				// 步进
				advance += glyph->advance;
			}
		}

		// 下滑线
		//if( underline.a )
		//{
		//	g_ShaderCache.Use(g_ShaderCache.VS_WVP,
		//					g_ShaderCache.PS_COLOR );

		//	g_ShaderCache.SetVal( "u_wvpMatrix",
		//						g_Graphics.GetOrthoMatrix() );
		//	g_ShaderCache.SetVal( "u_color", underline );

		//	// 设置 厚度度
		//	GLfloat lineWidth = ( this->currFont->bold + this->currFont->outline ) * 0.02f;
		//	glLineWidth( Max( lineWidth, 1.0f ) );

		//	// 计算 线段顶点坐标
		//	Vector2< GLshort > vertices[ 2 ];
		//	{
		//		vertices[0].Set( pos.x + this->GetGlyph( *str )->offset,
		//						pos.y + (Int16)( this->currFont->bold * 0.02f ) );

		//		vertices[1].Set( vertices[0].x + advance + glyph->tex2D->GetWidth() - glyph->advance,
		//						vertices[0].y );
		//	}

		//	// 传入GPU渲染
		//	glVertexAttribPointer( g_ShaderCache.SLOT_POSITION, 2, GL_SHORT, GL_FALSE, 0, vertices );

		//	glDrawArrays( GL_LINES, 0, 2 );
		//}

		// 删除线
		if( strikethrough.a )
		{
		}
	}
}