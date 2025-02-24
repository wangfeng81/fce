/*
**
**	Bitmap.
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce::inner::image
{
	/*
	 @	析构
	 */
	Bitmap::~Bitmap() noexcept
		{
			this->Release();
		}

	/*
	 @	构造 move
	 */
	Bitmap::Bitmap( Bitmap&& rv ) noexcept
		:
			hBitmap( rv.hBitmap )
		{
			rv.hBitmap = nullptr;
		}

	/*
	 @	构造 copy
	 */
	Bitmap::Bitmap( const Bitmap& bmp ) NOEXCEPT
		:
			Bitmap( bmp.hBitmap )
		{
		}

	/*
	 @	构造 HBITMAP
	 */
	Bitmap::Bitmap( const HBITMAP hBitmap ) NOEXCEPT
		:
			hBitmap( Clone( hBitmap ) )
		{
		}

	/*
	 @	构造 Image
	 */
	Bitmap::Bitmap( const Image& img ) NOEXCEPT
		:
			hBitmap( Create( img ) )
		{
		}

	/*
	 @	构造 Image with hdc
	 */
	Bitmap::Bitmap( const HDC hdc,
					const Image& img ) NOEXCEPT
		:
			hBitmap( Create( hdc, img ) )
		{
		}


	/*
	 @	释放 bitmap
	 */
	BOOL Bitmap::Release() noexcept
		{
			// 不为空 则删除
			if( hBitmap )
				{
					return DeleteBitmap( hBitmap );
				}

			return TRUE;
		}


	/*
	 @	Image 赋值
	 */
	Bitmap& Bitmap::operator = ( const Image& img ) NOEXCEPT
		{
			// 先释放
			this->Release();

			// 再创建
			this->hBitmap = Create( img );

			return *this;
		}


	/*
	 @	HBITMAP 赋值
	 */
	Bitmap& Bitmap::operator = ( const HBITMAP hBitmap ) NOEXCEPT
		{
			// 先释放
			this->Release();

			// 再克隆
			this->hBitmap = Clone( hBitmap );

			return *this;
		}


	/*
	 @	克隆 bitmap
	 */
	HBITMAP Bitmap::Clone( const HBITMAP srcBitmap ) NOEXCEPT
		{
			FC_ASSERT( srcBitmap );

			HBITMAP newBitmap{ NULL };

			// 获取 源 BITMAP 信息结构
			BITMAP srcInfo;

			if( GetObjectA( srcBitmap, sizeof( BITMAP ), &srcInfo ) )
				{
					// 创建 新位图
					if( newBitmap = CreateBitmapIndirect( &srcInfo ) )
						{
							// 创建 HDC
							HDC hdcSrc = CreateCompatibleDC( NULL );

							HDC hdcDst = CreateCompatibleDC( NULL );

							// 选取 HBITMAP
							SelectBitmap( hdcSrc, srcBitmap );

							SelectBitmap( hdcDst, newBitmap );

							// 复制 数据
							if( BitBlt( hdcDst, 0, 0, srcInfo.bmWidth,
													  srcInfo.bmHeight,
										hdcSrc, 0, 0, SRCCOPY ) == FALSE )
								{
									// 复制失败 则删除 newBitmap
									DeleteBitmap( newBitmap );

									newBitmap = NULL;
								}

							// 销毁 HDC
							DeleteDC( hdcSrc );

							DeleteDC( hdcDst );
						}
				}

			return newBitmap;
		}
}



namespace fce::inner::image
{
	/*
	 @	用 Image 创建 bitmap with hdc
	 */
	HBITMAP Bitmap::Create( const HDC hdc,
							const Image& img ) NOEXCEPT
		{
			FC_ASSERT( img );

			// DIBV5 缓冲尺寸
			const UInt size
				{
					Buf_size_for_img_to_dibv5( img )
				};

			// 分配 DIBV5 缓冲
			Buffer buf( size );

			BITMAPV5HEADER* bm5Header
				{
					LPBITMAPV5HEADER( *buf )
				};

			// 转换 img 到 DIBV5
			if( Image_to_DIBV5( img, *bm5Header,
									  bm5Header + 1 ) )
				{
					// 创建 dib
					return CreateDIBitmap( hdc,
										   LPBITMAPINFOHEADER( bm5Header ),
										   CBM_INIT, bm5Header + 1,
										   LPBITMAPINFO( bm5Header ),
										   DIB_RGB_COLORS );
				}

			// 失败是因为 8和16位 不支持，转换到 24位 重新创建
			return Create( hdc, img.Convert( COLOR::_3 ) );
		}


	/*
	 @	用 Image 创建 bitmap
	 */
	HBITMAP Bitmap::Create( const Image& img ) NOEXCEPT
		{
			HBITMAP hBitmap{ NULL };

			// 获取默认 hdc
			if( HDC hdc = GetDC( NULL ) )
				{
					// 创建 bitmap
					hBitmap = Create( hdc, img );

					// 释放 hdc
					ReleaseDC( NULL, hdc );
				}

			return hBitmap;
		}


	/*
	 @	HBITMAP 转 Image
						
			HBITMAP 可能是 DIB 或 DDB 的位图
				DIB 可直接转换
				DDB 需要先中转 dib
	 */
	BOOL Bitmap::Convert( const HDC hdc,
						  const HBITMAP hBitmap, Image& img ) NOEXCEPT
		{
			FC_ASSERT( hdc && hBitmap );

			// 获取 BITMAP 信息
			BITMAP bmp;

			if( GetObjectA( hBitmap, sizeof( BITMAP ), &bmp ) )
				{
					/*
						如果 BITMAP 有 dib 数据
						且深度大于16 可直接转换

							这大概是 DIB 的 HBITMAP
					*/
					if( bmp.bmBits && bmp.bmBitsPixel > 16 )
						{
							// 目前只需填充 以下数据就足够
							BITMAPV5HEADER bm5Header
									{
										.bV5Width		= bmp.bmWidth,
										.bV5Height		= bmp.bmHeight,
										.bV5BitCount	= bmp.bmBitsPixel,
										.bV5Compression	= BI_RGB
									};

							return DIBV5_to_Image( img, bm5Header, bmp.bmBits );
						}
					/*
						如果 没有直接的 dib 数据
						或可能带 调色板 以及 压缩数据
						则通过 GetDIBits 生成 24位DIB数据进行中转
							
							这大概是 DDB 的 HBITMAP
					*/
					else
						{
							// 设置 img 信息，固定 24位色 可确保 DIB 生成成功
							Info info( Int16( bmp.bmWidth ),
									   Int16( abs( bmp.bmHeight ) ),
									   COLOR::_3 );

							// 设置 BITMAPV5HEADER
							BITMAPV5HEADER bm5Header
								{
									Info_to_DIBV5Header( info )
								};

							// 分配 DIB 图像阵列 缓冲
							Buffer buf( bm5Header.bV5SizeImage );

							// 获取 DIB 图像阵列
							if( GetDIBits( hdc, hBitmap,
										   0, bm5Header.bV5Height,
										   *buf, LPBITMAPINFO( &bm5Header ),
										   DIB_RGB_COLORS ) )
								{
									// info 的计算用了abs，而高度可能为负 需要翻转
									bm5Header.bV5Height = bmp.bmHeight;

									return DIBV5_to_Image( img, bm5Header, *buf );
								}
						}
				}

			return FALSE;
		}


	/*
	 @	HBITMAP 转 Image
	 */
	BOOL Bitmap::Convert( const HBITMAP hBitmap, Image& img ) NOEXCEPT
		{
			Bool res{ false };

			// 获取默认 hdc
			if( HDC hdc = GetDC( NULL ) )
				{
					// 转换 bitmap
					res = Convert( hdc,
								   hBitmap, img );

					// 释放 hdc
					ReleaseDC( NULL, hdc );
				}

			return res;
		}


	/*
	 @	DIBV5 转 Image

			目前仅支持 非压缩的 BI_RGB 24 32 格式
	 */
	BOOL Bitmap::DIBV5_to_Image( Image& img, const BITMAPV5HEADER& bm5Header,
													   const VOID* bmBits ) NOEXCEPT
		{
			FC_ASSERT( bmBits );

			// 只解析 非压缩的 24 32 格式
			if( bm5Header.bV5BitCount < 24
			 || bm5Header.bV5Compression != BI_RGB )
				{
					return FALSE;
				}

			// 设置 img 格式，分配内存
			img.Set( Int16( bm5Header.bV5Width ),
					 Int16( abs( bm5Header.bV5Height ) ),
					 COLOR( Color::Size( bm5Header.bV5BitCount ) ) );

			// 跨距 按 32位 4字节对齐
			Int32 pitch
				{
					img->Pitch( 4 )
				};

			// 目标 起始行位置
			const BYTE* startRow
				{
					reinterpret_cast< const BYTE* >( bmBits )
				};

			// bV5Height 如果是负数 则图像是倒置的
			if( bm5Header.bV5Height < 0 )
				{
					// 目标从 最底排开始
					startRow += pitch * ( img->height - 1 );

					// 负 pitch 用于回撤一排
					pitch = -pitch;
				}

			// 按 color 类型 转换 图像阵列
			switch( img->color )
				{
					// 24 位色
					case COLOR::_3:
						{
							Color3* dst{ img.Pixels< Color3 >() };

							for( Int16 col{ img->height }; col; --col )
								{
									const Color3* src{ ( const Color3* )startRow };

									for( Int16 row{ img->width }; row; --row )
										{
											*dst++ = Color3( src->b,
															 src->g,
															 src->r );

											++src;
										}

									// 下一行
									startRow += pitch;
								}
						}
						break;

					// 32 位色
					case COLOR::_4:
						{
							DWord* dst{ img.Pixels< DWord >() };

							for( Int16 col{ img->height }; col; --col )
								{
									const Color4* src{ ( const Color4* )startRow };

									for( Int16 row{ img->width }; row; --row )
										{
											*dst++ = DWord( Color4( src->b,
																	src->g,
																	src->r,
																	src->a ) );

											++src;
										}

									// 下一行
									startRow += pitch;
								}
						}
						break;

					// 未支持的格式
					default:
						{
							return FALSE;
						}
				}

			return TRUE;
		}


	/*
	 @	Image 转 DIBV5

			目前只解析常用的 BI_RGB 24 32 格式
	 */
	BOOL Bitmap::Image_to_DIBV5( const Image& img, BITMAPV5HEADER& bm5Header,
															 VOID* bmBits ) NOEXCEPT
		{
			FC_ASSERT( bmBits );

			// 设置 BITMAPV5HEADER
			bm5Header = Info_to_DIBV5Header( img );

			// 源起始行位置
			BYTE* startRow
				{
					reinterpret_cast< BYTE* >( bmBits )
				};

			// 按 color 类型 转换 图像阵列
			switch( img->color )
				{
					// 24 位色
					case COLOR::_3:
						{
							// 要求 32位 对齐
							const Int32 pitch{ img->Pitch( 4 ) };

							const Color3* src{ img.Pixels< Color3 >() };

							for( Int16 col{ img->height }; col; --col )
								{
									Color3* dst{ ( Color3* )startRow };

									for( Int16 row{ img->width }; row; --row )
										{
											// rgb 反序
											*dst++ = Color3( src->b,
															 src->g,
															 src->r );

											++src;
										}

									startRow += pitch;
								}
						}
						break;

					// 32 位色
					case COLOR::_4:
						{
							// 自然 32 位对齐
							const Color4* src{ img.Pixels< Color4 >() };

							DWord* dst{ ( DWord* )startRow };

							// 像素 数量
							DWORD count
								{
									bm5Header.bV5SizeImage >> 2
								};

							while( count )
								{
									// rgba 反序
									*dst++ = DWord( Color4( src->b,
															src->g,
															src->r,
															src->a ) );

									++src, --count;
								}
						}
						break;

					// 未支持的格式
					default:
						{
							return FALSE;
						}
				}

			return TRUE;
		}


	/*
	 @	Info 转 BITMAPV5HEADER
	 */
	BITMAPV5HEADER Bitmap::Info_to_DIBV5Header( const Info& info ) noexcept
		{
			return BITMAPV5HEADER
					{
						.bV5Size			= sizeof( BITMAPV5HEADER ),
						.bV5Width			= info.width,
						.bV5Height			= info.height,
						.bV5Planes			= 1,
						.bV5BitCount		= info.Depth(),
						.bV5Compression		= BI_RGB, //BI_BITFIELDS,
						.bV5SizeImage		= DWORD( info.Capacity( 4 ) ),
						.bV5XPelsPerMeter	= 0,
						.bV5YPelsPerMeter	= 0,
						.bV5ClrUsed			= 0,
						.bV5ClrImportant	= 0,
						.bV5RedMask			= 0, // 0x000000FF,
						.bV5GreenMask		= 0, // 0x0000FF00,
						.bV5BlueMask		= 0, // 0x00FF0000,
						.bV5AlphaMask		= 0, // 0xFF000000,
						.bV5CSType			= 0,
						.bV5Endpoints		= 0,
						.bV5GammaRed		= 0,
						.bV5GammaGreen		= 0,
						.bV5GammaBlue		= 0,
						.bV5Intent			= 0,
						.bV5ProfileData		= 0,
						.bV5ProfileSize		= 0,
						.bV5Reserved		= 0
					};
		}


	/*
	 @	Image 转换到 DIBV5 的图像内存尺寸
	 */
	UInt Bitmap::Buf_size_for_img_to_dibv5( const Info& info ) noexcept
		{
			// DIBV5 要求 32 位的 4 字节对齐
			return info.Capacity( 4 )
				 + sizeof( BITMAPV5HEADER );
		}
}