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
		开启 剪贴板监听器 暂时放在 Win_Window::on_WM_CREATE
		后续考虑放到 AddHandler 里面

			AddClipboardFormatListener( s_Win_Window );
	*/

	/*
	 @	注册 自定义的 剪贴板类型
	 */
	const UINT Win_Clipboard::CF_BUFFER
		{
			RegisterClipboardFormatA( typeid( Buffer ).name() )
		};


	/*
	 @	清空 剪贴板
	 */
	FC_INLINE BOOL Win_Clipboard::Clear() noexcept
		{
			BOOL res{ FALSE };

			// 打开 剪贴板
			if( OpenClipboard( s_Win_Window ) )
				{
					// 清空 剪贴板
					res = EmptyClipboard();

					// 关闭 剪贴板
					res &= CloseClipboard();
				}

			return res;
		}


	/*
	 @	获取 剪贴板 各类对象 的解码封装
	 */
	BOOL Win_Clipboard::Get( Variant obj, const Decoder decoder,
										  const UINT format ) noexcept
		{
			BOOL res{ FALSE };

			// 打开 剪贴板
			if( OpenClipboard( s_Win_Window ) )
				{
					// 获取 内存块
					if( HANDLE hBlock{ GetClipboardData( format ) } )
						{
							// 锁定 内存块
							if( LPCVOID p{ GlobalLock( hBlock ) } )
								{
									// 获取 尺寸
									const SIZE_T size
										{
											GlobalSize( hBlock )
										};

									// 解码 内存块
									decoder( obj, p, size );

									// 解锁 内存块
									GlobalUnlock( hBlock );

									res = TRUE;
								}
						}

					// 关闭 剪贴板
					res &= CloseClipboard();
				}

			return res;
		}


	/*
	 @	设置 剪贴板 各类对象 的编码封装
	 */
	BOOL Win_Clipboard::Set( const_Variant obj, const SIZE_T size,
												const Encoder encoder,
												const UINT format ) noexcept
		{
			BOOL res{ FALSE };

			// 打开 剪贴板
			if( OpenClipboard( s_Win_Window ) )
				{
					// 清空 剪贴板
					EmptyClipboard();

					// 分配 内存块
					if( HGLOBAL hBlock{ GlobalAlloc( GMEM_MOVEABLE,
													 size ) } )
						{
							// 锁定 内存块
							if( LPVOID p{ GlobalLock( hBlock ) } )
								{
									// 编码 内存块
									encoder( obj, p );

									// 解锁 内存块
									GlobalUnlock( hBlock );

									// 设置 内存块 到 剪贴板
									res = SetClipboardData( format, hBlock )
										  == hBlock;
								}

							// 如果设置成功，分配的内存块 转交剪贴板托管
							if( res == FALSE )
								{
									// 失败 则需要手动释放
									GlobalFree( hBlock );
								}
						}

					// 关闭 剪贴板
					res &= CloseClipboard();
				}

			return res;
		}
}



/**
 !	设置 object 到 剪贴板
 */
namespace fce::inner::win
{
	/*
	 @	设置 Text
	 */
	FC_INLINE BOOL Win_Clipboard::Set( const stl::wstring& wstr ) noexcept
		{
			// wstring 需要的 内存大小 包括 \'0'
			const UInt size
				{
					String::Capacity( wstr )
				};

			return Set( &wstr, size,
						EncodeString,
						CF_UNICODETEXT );
		}


	/*
	 @	设置 Image
	 */
	FC_INLINE BOOL Win_Clipboard::Set( const Image& img ) noexcept
		{
			// 24和32位 可直接设置
			if( img->color > COLOR::_2 )
				{
					// 文件头 后面紧跟 图像阵列，目标要求 32位对齐
					const UInt size
						{
							Bitmap::Buf_size_for_img_to_dibv5( img )
						};

					return Set( &img, size,
								EncodeImage,
								CF_DIBV5 );
				}

			// 8和16位 间接转换到 24位 进行设置
			return Set( img.Convert( COLOR::_3 ) );
		}


	/*
	 @	设置 Buffer
	 */
	FC_INLINE BOOL Win_Clipboard::Set( const Buffer& buf ) noexcept
		{
			return Set( &buf, buf.Size(),
						EncodeBuffer,
						CF_BUFFER );
		}


	/*
	 @	设置 HBITMAP
	 */
	BOOL Win_Clipboard::Set( const HBITMAP hBitmap ) noexcept
		{
			FC_ASSERT( hBitmap );

			BOOL res{ FALSE };

			// 打开 剪贴板
			if( OpenClipboard( s_Win_Window ) )
				{
					// 清空 剪贴板
					EmptyClipboard();

					// 设置 bitmap 到 剪贴板
					res = SetClipboardData( CF_BITMAP, hBitmap )
						  == hBitmap;

					// 关闭 剪贴板
					res &= CloseClipboard();
				}

			return res;
		}
}



/**
 !	获取 object 从 剪贴板
 */
namespace fce::inner::win
{
	/*
	 @	获取 String
	 */
	FC_INLINE BOOL Win_Clipboard::Get( String& str ) noexcept
		{
			return Get( &str,
						DecodeString,
						CF_UNICODETEXT );
		}


	/*
	 @	获取 Image
	 */
	FC_INLINE BOOL Win_Clipboard::Get( Image& img ) noexcept
		{
			// 优先用 CF_DIBV5 => 失败则备用 CF_BITMAP
			return Get( &img,
						DecodeImage,
						CF_DIBV5 );
		}


	/*
	 @	获取 data
	 */
	FC_INLINE BOOL Win_Clipboard::Get( Buffer& buf ) noexcept
		{
			return Get( &buf,
						DecodeBuffer,
						CF_BUFFER );
		}


	/*
	 @	获取 HBITMAP
	 */
	HBITMAP Win_Clipboard::GetBitmap() noexcept
		{
			HBITMAP hBitmap{ NULL };

			// 打开 剪贴板
			if( OpenClipboard( s_Win_Window ) )
				{
					// 获取 HBITMAP
					hBitmap = HBITMAP( GetClipboardData( CF_BITMAP ) );

					// 关闭 剪贴板
					CloseClipboard();
				}

			return hBitmap;
		}
}



/**
 !	编码 object 到 内存块	（ callbacks ）
 */
namespace fce::inner::win
{
	/*
	 @	编码 String
	 */
	VOID Win_Clipboard::EncodeString( const_Variant obj,
											  VOID* data ) noexcept
		{
			// 强制转换 obj => wstr
			const stl::wstring* wstr
				{
					reinterpret_cast< const stl::wstring* >( obj )
				};

			// 内存块 存储的字符是 WCHAR
			lstrcpyW( LPWSTR( data ),
					  wstr->c_str() );
		}


	/*
	 @	编码 Image to DIBV5
	 */
	VOID Win_Clipboard::EncodeImage( const_Variant obj,
											 VOID* data ) noexcept
		{
			// 强制转换 obj => img
			const Image* img
				{
					reinterpret_cast< const Image* >( obj )
				};

			// 强制转换 data
			BITMAPV5HEADER* bm5Header
				{
					reinterpret_cast< BITMAPV5HEADER* >( data )
				};

			// DIBV5 图像阵列 紧跟 BITMAPV5HEADER 后面
			Bitmap::Image_to_DIBV5( *img, *bm5Header,
										   bm5Header + 1 );
		}


	/*
	 @	编码 Buffer
	 */
	VOID Win_Clipboard::EncodeBuffer( const_Variant obj,
											  VOID* data ) noexcept
		{
			// 强制转换 obj => buf
			const Buffer* buf
				{
					reinterpret_cast< const Buffer* >( obj )
				};

			// 拷贝 Buffer 到 剪贴板
			std::memcpy( data, buf->Data(),
							   buf->Size() );
		}
}



/**
 !	解码 object 从 内存块	（ callbacks ）
 */
namespace fce::inner::win
{
	/*
	 @	解码 String
	 */
	VOID Win_Clipboard::DecodeString( Variant obj, const VOID* data,
												   const SIZE_T size ) noexcept
		{
			// 强制转换 obj => str
			String* str
				{
					reinterpret_cast< String* >( obj )
				};

			// wstring 的长度 ( 去除\'0' )
			const UInt len
				{
					( size >> 1 ) - 1
				};

			// 内存块 存储的字符是 WCHAR
			str->Assign( LPCWCH( data ), len );
		}


	/*
	 @	解码 Image

			先尝试 CF_DIBV5 解码，如果失败则转用 CF_BITMAP

			CF_BITMAP	可以将任意格式 都转换为 Image 支持的格式
						能保障所有的解码都成功

			CF_DIBV5	速度更快 但格式繁杂 没必要全部支持
						反正 CF_BITMAP 可以替代 所有的转换
						目前只解析常用的 BI_RGB 24 32 格式
	 */
	VOID Win_Clipboard::DecodeImage( Variant obj, const VOID* data,
												  const SIZE_T size ) noexcept
		{
			// 强制转换 obj => img
			Image* img
				{
					reinterpret_cast< Image* >( obj )
				};

			// 强制转换 data
			const BITMAPV5HEADER* bm5Header
				{
					reinterpret_cast< const BITMAPV5HEADER* >( data )
				};

			// 首选 CF_DIBV5 解码，如果失败转用 CF_BITMAP 确保成功
			if( ! Bitmap::DIBV5_to_Image( *img, *bm5Header,
												 bm5Header + 1 ) )
				{
					// 从剪贴板另外获取 HBITMAP
					const HBITMAP hBitmap
						{
							HBITMAP( GetClipboardData( CF_BITMAP ) )
						};

					// hBitmap 转 img
					if( hBitmap )
						{
							Bitmap::Convert( hBitmap, *img );
						}
				}
		}


	/*
	 @	解码 Buffer
	 */
	VOID Win_Clipboard::DecodeBuffer( Variant obj, const VOID* data,
												   const SIZE_T size ) noexcept
		{
			// 强制转换 obj => buf
			Buffer* buf
				{
					reinterpret_cast< Buffer* >( obj )
				};

			// 拷贝赋值 内存块 给 Buffer
			buf->Set( data, size );
		}
}



/**
 !	处理 事件消息
 */
namespace fce::inner::win
{
	/*
	 @	处理 剪贴板消息
	 */
	VOID Win_Clipboard::on_WndProc( const UINT uMsg,
									const WPARAM wParam,
									const LPARAM lParam ) noexcept
		{
			// 转换 win 消息
			EVENT& e{ s_Clipboard.message.e };

			switch( uMsg )
				{
					// 剪贴板 内容变更
					case WM_CLIPBOARDUPDATE		: { e = EVENT::UPDATE;	} break;

					// 剪贴板 内容清空
					case WM_DESTROYCLIPBOARD	: { e = EVENT::CLEAR;	} break;

					// 不应有 未处理的消息，会造成 EVENT 重复 Dispatch
					default: std::unreachable();
				}

			// 分发消息给 引擎
			s_Clipboard.Dispatch();
		}
}