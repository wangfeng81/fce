/*
**
**	Bitmap.
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce::inner::image
{
	/*
	 @	����
	 */
	Bitmap::~Bitmap() noexcept
		{
			this->Release();
		}

	/*
	 @	���� move
	 */
	Bitmap::Bitmap( Bitmap&& rv ) noexcept
		:
			hBitmap( rv.hBitmap )
		{
			rv.hBitmap = nullptr;
		}

	/*
	 @	���� copy
	 */
	Bitmap::Bitmap( const Bitmap& bmp ) NOEXCEPT
		:
			Bitmap( bmp.hBitmap )
		{
		}

	/*
	 @	���� HBITMAP
	 */
	Bitmap::Bitmap( const HBITMAP hBitmap ) NOEXCEPT
		:
			hBitmap( Clone( hBitmap ) )
		{
		}

	/*
	 @	���� Image
	 */
	Bitmap::Bitmap( const Image& img ) NOEXCEPT
		:
			hBitmap( Create( img ) )
		{
		}

	/*
	 @	���� Image with hdc
	 */
	Bitmap::Bitmap( const HDC hdc,
					const Image& img ) NOEXCEPT
		:
			hBitmap( Create( hdc, img ) )
		{
		}


	/*
	 @	�ͷ� bitmap
	 */
	BOOL Bitmap::Release() noexcept
		{
			// ��Ϊ�� ��ɾ��
			if( hBitmap )
				{
					return DeleteBitmap( hBitmap );
				}

			return TRUE;
		}


	/*
	 @	Image ��ֵ
	 */
	Bitmap& Bitmap::operator = ( const Image& img ) NOEXCEPT
		{
			// ���ͷ�
			this->Release();

			// �ٴ���
			this->hBitmap = Create( img );

			return *this;
		}


	/*
	 @	HBITMAP ��ֵ
	 */
	Bitmap& Bitmap::operator = ( const HBITMAP hBitmap ) NOEXCEPT
		{
			// ���ͷ�
			this->Release();

			// �ٿ�¡
			this->hBitmap = Clone( hBitmap );

			return *this;
		}


	/*
	 @	��¡ bitmap
	 */
	HBITMAP Bitmap::Clone( const HBITMAP srcBitmap ) NOEXCEPT
		{
			FC_ASSERT( srcBitmap );

			HBITMAP newBitmap{ NULL };

			// ��ȡ Դ BITMAP ��Ϣ�ṹ
			BITMAP srcInfo;

			if( GetObjectA( srcBitmap, sizeof( BITMAP ), &srcInfo ) )
				{
					// ���� ��λͼ
					if( newBitmap = CreateBitmapIndirect( &srcInfo ) )
						{
							// ���� HDC
							HDC hdcSrc = CreateCompatibleDC( NULL );

							HDC hdcDst = CreateCompatibleDC( NULL );

							// ѡȡ HBITMAP
							SelectBitmap( hdcSrc, srcBitmap );

							SelectBitmap( hdcDst, newBitmap );

							// ���� ����
							if( BitBlt( hdcDst, 0, 0, srcInfo.bmWidth,
													  srcInfo.bmHeight,
										hdcSrc, 0, 0, SRCCOPY ) == FALSE )
								{
									// ����ʧ�� ��ɾ�� newBitmap
									DeleteBitmap( newBitmap );

									newBitmap = NULL;
								}

							// ���� HDC
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
	 @	�� Image ���� bitmap with hdc
	 */
	HBITMAP Bitmap::Create( const HDC hdc,
							const Image& img ) NOEXCEPT
		{
			FC_ASSERT( img );

			// DIBV5 ����ߴ�
			const UInt size
				{
					Buf_size_for_img_to_dibv5( img )
				};

			// ���� DIBV5 ����
			Buffer buf( size );

			BITMAPV5HEADER* bm5Header
				{
					LPBITMAPV5HEADER( *buf )
				};

			// ת�� img �� DIBV5
			if( Image_to_DIBV5( img, *bm5Header,
									  bm5Header + 1 ) )
				{
					// ���� dib
					return CreateDIBitmap( hdc,
										   LPBITMAPINFOHEADER( bm5Header ),
										   CBM_INIT, bm5Header + 1,
										   LPBITMAPINFO( bm5Header ),
										   DIB_RGB_COLORS );
				}

			// ʧ������Ϊ 8��16λ ��֧�֣�ת���� 24λ ���´���
			return Create( hdc, img.Convert( COLOR::_3 ) );
		}


	/*
	 @	�� Image ���� bitmap
	 */
	HBITMAP Bitmap::Create( const Image& img ) NOEXCEPT
		{
			HBITMAP hBitmap{ NULL };

			// ��ȡĬ�� hdc
			if( HDC hdc = GetDC( NULL ) )
				{
					// ���� bitmap
					hBitmap = Create( hdc, img );

					// �ͷ� hdc
					ReleaseDC( NULL, hdc );
				}

			return hBitmap;
		}


	/*
	 @	HBITMAP ת Image
						
			HBITMAP ������ DIB �� DDB ��λͼ
				DIB ��ֱ��ת��
				DDB ��Ҫ����ת dib
	 */
	BOOL Bitmap::Convert( const HDC hdc,
						  const HBITMAP hBitmap, Image& img ) NOEXCEPT
		{
			FC_ASSERT( hdc && hBitmap );

			// ��ȡ BITMAP ��Ϣ
			BITMAP bmp;

			if( GetObjectA( hBitmap, sizeof( BITMAP ), &bmp ) )
				{
					/*
						��� BITMAP �� dib ����
						����ȴ���16 ��ֱ��ת��

							������ DIB �� HBITMAP
					*/
					if( bmp.bmBits && bmp.bmBitsPixel > 16 )
						{
							// Ŀǰֻ����� �������ݾ��㹻
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
						��� û��ֱ�ӵ� dib ����
						����ܴ� ��ɫ�� �Լ� ѹ������
						��ͨ�� GetDIBits ���� 24λDIB���ݽ�����ת
							
							������ DDB �� HBITMAP
					*/
					else
						{
							// ���� img ��Ϣ���̶� 24λɫ ��ȷ�� DIB ���ɳɹ�
							Info info( Int16( bmp.bmWidth ),
									   Int16( abs( bmp.bmHeight ) ),
									   COLOR::_3 );

							// ���� BITMAPV5HEADER
							BITMAPV5HEADER bm5Header
								{
									Info_to_DIBV5Header( info )
								};

							// ���� DIB ͼ������ ����
							Buffer buf( bm5Header.bV5SizeImage );

							// ��ȡ DIB ͼ������
							if( GetDIBits( hdc, hBitmap,
										   0, bm5Header.bV5Height,
										   *buf, LPBITMAPINFO( &bm5Header ),
										   DIB_RGB_COLORS ) )
								{
									// info �ļ�������abs�����߶ȿ���Ϊ�� ��Ҫ��ת
									bm5Header.bV5Height = bmp.bmHeight;

									return DIBV5_to_Image( img, bm5Header, *buf );
								}
						}
				}

			return FALSE;
		}


	/*
	 @	HBITMAP ת Image
	 */
	BOOL Bitmap::Convert( const HBITMAP hBitmap, Image& img ) NOEXCEPT
		{
			Bool res{ false };

			// ��ȡĬ�� hdc
			if( HDC hdc = GetDC( NULL ) )
				{
					// ת�� bitmap
					res = Convert( hdc,
								   hBitmap, img );

					// �ͷ� hdc
					ReleaseDC( NULL, hdc );
				}

			return res;
		}


	/*
	 @	DIBV5 ת Image

			Ŀǰ��֧�� ��ѹ���� BI_RGB 24 32 ��ʽ
	 */
	BOOL Bitmap::DIBV5_to_Image( Image& img, const BITMAPV5HEADER& bm5Header,
													   const VOID* bmBits ) NOEXCEPT
		{
			FC_ASSERT( bmBits );

			// ֻ���� ��ѹ���� 24 32 ��ʽ
			if( bm5Header.bV5BitCount < 24
			 || bm5Header.bV5Compression != BI_RGB )
				{
					return FALSE;
				}

			// ���� img ��ʽ�������ڴ�
			img.Set( Int16( bm5Header.bV5Width ),
					 Int16( abs( bm5Header.bV5Height ) ),
					 COLOR( Color::Size( bm5Header.bV5BitCount ) ) );

			// ��� �� 32λ 4�ֽڶ���
			Int32 pitch
				{
					img->Pitch( 4 )
				};

			// Ŀ�� ��ʼ��λ��
			const BYTE* startRow
				{
					reinterpret_cast< const BYTE* >( bmBits )
				};

			// bV5Height ����Ǹ��� ��ͼ���ǵ��õ�
			if( bm5Header.bV5Height < 0 )
				{
					// Ŀ��� ����ſ�ʼ
					startRow += pitch * ( img->height - 1 );

					// �� pitch ���ڻس�һ��
					pitch = -pitch;
				}

			// �� color ���� ת�� ͼ������
			switch( img->color )
				{
					// 24 λɫ
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

									// ��һ��
									startRow += pitch;
								}
						}
						break;

					// 32 λɫ
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

									// ��һ��
									startRow += pitch;
								}
						}
						break;

					// δ֧�ֵĸ�ʽ
					default:
						{
							return FALSE;
						}
				}

			return TRUE;
		}


	/*
	 @	Image ת DIBV5

			Ŀǰֻ�������õ� BI_RGB 24 32 ��ʽ
	 */
	BOOL Bitmap::Image_to_DIBV5( const Image& img, BITMAPV5HEADER& bm5Header,
															 VOID* bmBits ) NOEXCEPT
		{
			FC_ASSERT( bmBits );

			// ���� BITMAPV5HEADER
			bm5Header = Info_to_DIBV5Header( img );

			// Դ��ʼ��λ��
			BYTE* startRow
				{
					reinterpret_cast< BYTE* >( bmBits )
				};

			// �� color ���� ת�� ͼ������
			switch( img->color )
				{
					// 24 λɫ
					case COLOR::_3:
						{
							// Ҫ�� 32λ ����
							const Int32 pitch{ img->Pitch( 4 ) };

							const Color3* src{ img.Pixels< Color3 >() };

							for( Int16 col{ img->height }; col; --col )
								{
									Color3* dst{ ( Color3* )startRow };

									for( Int16 row{ img->width }; row; --row )
										{
											// rgb ����
											*dst++ = Color3( src->b,
															 src->g,
															 src->r );

											++src;
										}

									startRow += pitch;
								}
						}
						break;

					// 32 λɫ
					case COLOR::_4:
						{
							// ��Ȼ 32 λ����
							const Color4* src{ img.Pixels< Color4 >() };

							DWord* dst{ ( DWord* )startRow };

							// ���� ����
							DWORD count
								{
									bm5Header.bV5SizeImage >> 2
								};

							while( count )
								{
									// rgba ����
									*dst++ = DWord( Color4( src->b,
															src->g,
															src->r,
															src->a ) );

									++src, --count;
								}
						}
						break;

					// δ֧�ֵĸ�ʽ
					default:
						{
							return FALSE;
						}
				}

			return TRUE;
		}


	/*
	 @	Info ת BITMAPV5HEADER
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
	 @	Image ת���� DIBV5 ��ͼ���ڴ�ߴ�
	 */
	UInt Bitmap::Buf_size_for_img_to_dibv5( const Info& info ) noexcept
		{
			// DIBV5 Ҫ�� 32 λ�� 4 �ֽڶ���
			return info.Capacity( 4 )
				 + sizeof( BITMAPV5HEADER );
		}
}