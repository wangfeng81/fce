/*
**
**	Windows Clipboard.
**
		Windows platform implementation

	Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::win
{
	/*
		���� ����������� ��ʱ���� Win_Window::on_WM_CREATE
		�������Ƿŵ� AddHandler ����

			AddClipboardFormatListener( s_Win_Window );
	*/

	/*
	 @	ע�� �Զ���� ����������
	 */
	const UINT Win_Clipboard::CF_BUFFER
		{
			RegisterClipboardFormatA( typeid( Buffer ).name() )
		};


	/*
	 @	��� ������
	 */
	FC_INLINE BOOL Win_Clipboard::Clear() noexcept
		{
			BOOL res{ FALSE };

			// �� ������
			if( OpenClipboard( s_Win_Window ) )
				{
					// ��� ������
					res = EmptyClipboard();

					// �ر� ������
					res &= CloseClipboard();
				}

			return res;
		}


	/*
	 @	��ȡ ������ ������� �Ľ����װ
	 */
	BOOL Win_Clipboard::Get( Variant obj, const Decoder decoder,
										  const UINT format ) noexcept
		{
			BOOL res{ FALSE };

			// �� ������
			if( OpenClipboard( s_Win_Window ) )
				{
					// ��ȡ �ڴ��
					if( HANDLE hBlock{ GetClipboardData( format ) } )
						{
							// ���� �ڴ��
							if( LPCVOID p{ GlobalLock( hBlock ) } )
								{
									// ��ȡ �ߴ�
									const SIZE_T size
										{
											GlobalSize( hBlock )
										};

									// ���� �ڴ��
									decoder( obj, p, size );

									// ���� �ڴ��
									GlobalUnlock( hBlock );

									res = TRUE;
								}
						}

					// �ر� ������
					res &= CloseClipboard();
				}

			return res;
		}


	/*
	 @	���� ������ ������� �ı����װ
	 */
	BOOL Win_Clipboard::Set( const_Variant obj, const SIZE_T size,
												const Encoder encoder,
												const UINT format ) noexcept
		{
			BOOL res{ FALSE };

			// �� ������
			if( OpenClipboard( s_Win_Window ) )
				{
					// ��� ������
					EmptyClipboard();

					// ���� �ڴ��
					if( HGLOBAL hBlock{ GlobalAlloc( GMEM_MOVEABLE,
													 size ) } )
						{
							// ���� �ڴ��
							if( LPVOID p{ GlobalLock( hBlock ) } )
								{
									// ���� �ڴ��
									encoder( obj, p );

									// ���� �ڴ��
									GlobalUnlock( hBlock );

									// ���� �ڴ�� �� ������
									res = SetClipboardData( format, hBlock )
										  == hBlock;
								}

							// ������óɹ���������ڴ�� ת���������й�
							if( res == FALSE )
								{
									// ʧ�� ����Ҫ�ֶ��ͷ�
									GlobalFree( hBlock );
								}
						}

					// �ر� ������
					res &= CloseClipboard();
				}

			return res;
		}
}



/**
 !	���� object �� ������
 */
namespace fce::inner::win
{
	/*
	 @	���� Text
	 */
	FC_INLINE BOOL Win_Clipboard::Set( const stl::wstring& wstr ) noexcept
		{
			// wstring ��Ҫ�� �ڴ��С ���� \'0'
			const UInt size
				{
					String::Capacity( wstr )
				};

			return Set( &wstr, size,
						EncodeString,
						CF_UNICODETEXT );
		}


	/*
	 @	���� Image
	 */
	FC_INLINE BOOL Win_Clipboard::Set( const Image& img ) noexcept
		{
			// 24��32λ ��ֱ������
			if( img->color > COLOR::_2 )
				{
					// �ļ�ͷ ������� ͼ�����У�Ŀ��Ҫ�� 32λ����
					const UInt size
						{
							Bitmap::Buf_size_for_img_to_dibv5( img )
						};

					return Set( &img, size,
								EncodeImage,
								CF_DIBV5 );
				}

			// 8��16λ ���ת���� 24λ ��������
			return Set( img.Convert( COLOR::_3 ) );
		}


	/*
	 @	���� Buffer
	 */
	FC_INLINE BOOL Win_Clipboard::Set( const Buffer& buf ) noexcept
		{
			return Set( &buf, buf.Size(),
						EncodeBuffer,
						CF_BUFFER );
		}


	/*
	 @	���� HBITMAP
	 */
	BOOL Win_Clipboard::Set( const HBITMAP hBitmap ) noexcept
		{
			FC_ASSERT( hBitmap );

			BOOL res{ FALSE };

			// �� ������
			if( OpenClipboard( s_Win_Window ) )
				{
					// ��� ������
					EmptyClipboard();

					// ���� bitmap �� ������
					res = SetClipboardData( CF_BITMAP, hBitmap )
						  == hBitmap;

					// �ر� ������
					res &= CloseClipboard();
				}

			return res;
		}
}



/**
 !	��ȡ object �� ������
 */
namespace fce::inner::win
{
	/*
	 @	��ȡ String
	 */
	FC_INLINE BOOL Win_Clipboard::Get( String& str ) noexcept
		{
			return Get( &str,
						DecodeString,
						CF_UNICODETEXT );
		}


	/*
	 @	��ȡ Image
	 */
	FC_INLINE BOOL Win_Clipboard::Get( Image& img ) noexcept
		{
			// ������ CF_DIBV5 => ʧ������ CF_BITMAP
			return Get( &img,
						DecodeImage,
						CF_DIBV5 );
		}


	/*
	 @	��ȡ data
	 */
	FC_INLINE BOOL Win_Clipboard::Get( Buffer& buf ) noexcept
		{
			return Get( &buf,
						DecodeBuffer,
						CF_BUFFER );
		}


	/*
	 @	��ȡ HBITMAP
	 */
	HBITMAP Win_Clipboard::GetBitmap() noexcept
		{
			HBITMAP hBitmap{ NULL };

			// �� ������
			if( OpenClipboard( s_Win_Window ) )
				{
					// ��ȡ HBITMAP
					hBitmap = HBITMAP( GetClipboardData( CF_BITMAP ) );

					// �ر� ������
					CloseClipboard();
				}

			return hBitmap;
		}
}



/**
 !	���� object �� �ڴ��	�� callbacks ��
 */
namespace fce::inner::win
{
	/*
	 @	���� String
	 */
	VOID Win_Clipboard::EncodeString( const_Variant obj,
											  VOID* data ) noexcept
		{
			// ǿ��ת�� obj => wstr
			const stl::wstring* wstr
				{
					reinterpret_cast< const stl::wstring* >( obj )
				};

			// �ڴ�� �洢���ַ��� WCHAR
			lstrcpyW( LPWSTR( data ),
					  wstr->c_str() );
		}


	/*
	 @	���� Image to DIBV5
	 */
	VOID Win_Clipboard::EncodeImage( const_Variant obj,
											 VOID* data ) noexcept
		{
			// ǿ��ת�� obj => img
			const Image* img
				{
					reinterpret_cast< const Image* >( obj )
				};

			// ǿ��ת�� data
			BITMAPV5HEADER* bm5Header
				{
					reinterpret_cast< BITMAPV5HEADER* >( data )
				};

			// DIBV5 ͼ������ ���� BITMAPV5HEADER ����
			Bitmap::Image_to_DIBV5( *img, *bm5Header,
										   bm5Header + 1 );
		}


	/*
	 @	���� Buffer
	 */
	VOID Win_Clipboard::EncodeBuffer( const_Variant obj,
											  VOID* data ) noexcept
		{
			// ǿ��ת�� obj => buf
			const Buffer* buf
				{
					reinterpret_cast< const Buffer* >( obj )
				};

			// ���� Buffer �� ������
			std::memcpy( data, buf->Data(),
							   buf->Size() );
		}
}



/**
 !	���� object �� �ڴ��	�� callbacks ��
 */
namespace fce::inner::win
{
	/*
	 @	���� String
	 */
	VOID Win_Clipboard::DecodeString( Variant obj, const VOID* data,
												   const SIZE_T size ) noexcept
		{
			// ǿ��ת�� obj => str
			String* str
				{
					reinterpret_cast< String* >( obj )
				};

			// wstring �ĳ��� ( ȥ��\'0' )
			const UInt len
				{
					( size >> 1 ) - 1
				};

			// �ڴ�� �洢���ַ��� WCHAR
			str->Assign( LPCWCH( data ), len );
		}


	/*
	 @	���� Image

			�ȳ��� CF_DIBV5 ���룬���ʧ����ת�� CF_BITMAP

			CF_BITMAP	���Խ������ʽ ��ת��Ϊ Image ֧�ֵĸ�ʽ
						�ܱ������еĽ��붼�ɹ�

			CF_DIBV5	�ٶȸ��� ����ʽ���� û��Ҫȫ��֧��
						���� CF_BITMAP ������� ���е�ת��
						Ŀǰֻ�������õ� BI_RGB 24 32 ��ʽ
	 */
	VOID Win_Clipboard::DecodeImage( Variant obj, const VOID* data,
												  const SIZE_T size ) noexcept
		{
			// ǿ��ת�� obj => img
			Image* img
				{
					reinterpret_cast< Image* >( obj )
				};

			// ǿ��ת�� data
			const BITMAPV5HEADER* bm5Header
				{
					reinterpret_cast< const BITMAPV5HEADER* >( data )
				};

			// ��ѡ CF_DIBV5 ���룬���ʧ��ת�� CF_BITMAP ȷ���ɹ�
			if( ! Bitmap::DIBV5_to_Image( *img, *bm5Header,
												 bm5Header + 1 ) )
				{
					// �Ӽ����������ȡ HBITMAP
					const HBITMAP hBitmap
						{
							HBITMAP( GetClipboardData( CF_BITMAP ) )
						};

					// hBitmap ת img
					if( hBitmap )
						{
							Bitmap::Convert( hBitmap, *img );
						}
				}
		}


	/*
	 @	���� Buffer
	 */
	VOID Win_Clipboard::DecodeBuffer( Variant obj, const VOID* data,
												   const SIZE_T size ) noexcept
		{
			// ǿ��ת�� obj => buf
			Buffer* buf
				{
					reinterpret_cast< Buffer* >( obj )
				};

			// ������ֵ �ڴ�� �� Buffer
			buf->Set( data, size );
		}
}



/**
 !	���� �¼���Ϣ
 */
namespace fce::inner::win
{
	/*
	 @	���� ��������Ϣ
	 */
	VOID Win_Clipboard::on_WndProc( const UINT uMsg,
									const WPARAM wParam,
									const LPARAM lParam ) noexcept
		{
			// ת�� win ��Ϣ
			EVENT& e{ s_Clipboard.message.e };

			switch( uMsg )
				{
					// ������ ���ݱ��
					case WM_CLIPBOARDUPDATE		: { e = EVENT::UPDATE;	} break;

					// ������ �������
					case WM_DESTROYCLIPBOARD	: { e = EVENT::CLEAR;	} break;

					// ��Ӧ�� δ�������Ϣ������� EVENT �ظ� Dispatch
					default: std::unreachable();
				}

			// �ַ���Ϣ�� ����
			s_Clipboard.Dispatch();
		}
}