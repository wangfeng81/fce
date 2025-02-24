/*
**
**	Png.
**
		Copyright (c) WangFeng since 2006...
*/

#include "Png.h"

#ifdef FC_WINDOWS
	#pragma comment( lib, "libpng16.lib" )
#endif


namespace fce::inner::image
{
	/*
	 @	Singleton instantiation
	 */
	Png Png::instance;


	/*
	 @	CheckType
	 */
	Bool Png::CheckType( const Buffer& buf ) const
		{
			// 检测PNG标志大小
			const png_size_t PNGSIGSIZE{ 8 };

			return buf.Size() > PNGSIGSIZE
					&& png_check_sig( *buf, PNGSIGSIZE );
		}


	/*
	 @	GetInfo
	 */
	Bool Png::GetInfo( const Buffer& buf, Info& info ) const
		{
			// 创建 PNG read 结构
			if( png_structp png_ptr =
					png_create_read_struct( PNG_LIBPNG_VER_STRING,
											nullptr, nullptr, nullptr ) )
				{
					// 创建PNG信息
					if( png_infop png_info_ptr =
							png_create_info_struct( png_ptr ) )
						{
							// 设置 读取回调函数
							png_set_read_fn( png_ptr,
											 const_cast<Buffer*>(&buf),
											 ReadCallback );

							// 读取 PNG 信息
							png_read_info( png_ptr, png_info_ptr );

							// 获取 Info
							info.width = png_get_image_width( png_ptr, png_info_ptr ),
							info.height = png_get_image_height( png_ptr, png_info_ptr );

							info.color = this->Convert(
												png_get_color_type(
													png_ptr, png_info_ptr ) );

							// 释放png资源
							png_destroy_info_struct( png_ptr, &png_info_ptr );
							png_destroy_read_struct( &png_ptr, &png_info_ptr, 0 );

							return true;
						}
				}

			return false;
		}


	/*
	 @	Decode
	 */
	Bool Png::Decode( const Buffer& buf, Image& img )
		{
			// 创建 PNG read 结构
			if( png_structp png_ptr =
					png_create_read_struct( PNG_LIBPNG_VER_STRING,
											nullptr, nullptr, nullptr ) )
				{
					// 创建 PNG信息
					if( png_infop png_info_ptr =
							png_create_info_struct( png_ptr ) )
						{
							// 设置 读取回调函数 将buffer传入
							png_set_read_fn( png_ptr,
											 const_cast< Buffer* >( &buf ),
											 ReadCallback );

							// 读取 PNG 信息
							png_read_info( png_ptr, png_info_ptr );

							// 获取 Info
							png_uint_32 width, height;
							png_int_32 depth, color;

							png_get_IHDR( png_ptr, png_info_ptr,
										  &width, &height, &depth, &color,
										  nullptr, nullptr, nullptr );

							// 扩展这些格式 位深低于8位 调色板格式
							if( depth < 8						||
								color == PNG_COLOR_TYPE_PALETTE	||
								png_get_valid( png_ptr, png_info_ptr,
											   PNG_INFO_tRNS ) )
								{
									// 设置扩展
									png_set_expand( png_ptr );

									// 更新 info
									png_read_update_info( png_ptr, png_info_ptr );

									// 重新获取 info
									png_get_IHDR( png_ptr, png_info_ptr,
												  &width, &height, &depth, &color,
												  nullptr, nullptr, nullptr );
								}

							// 根据最新参数 设置 Image
							img.Set( static_cast< Int16 >( width ),
									 static_cast< Int16 >( height ),
									 this->Convert( color ) );

							// 将Image的所有行的指针 装进rowPointers数组传入 减少中间拷贝
							stl::vector< png_bytep > rowPointers( height );

							for( png_uint_16 c{ 0 }; c < height; ++c )
								{
									rowPointers[ c ] = img.Row( c );
								}

							// 解压读取 image 位图
							png_read_image( png_ptr, &rowPointers[ 0 ] );

							// 释放png资源
							png_destroy_info_struct( png_ptr, &png_info_ptr );
							png_destroy_read_struct( &png_ptr, &png_info_ptr, nullptr );

							return true;
						}
				}

			return false;
		}


	/*
	 @	Encode

			目前 16位 没有直接支持，转 24位 处理
	 */
	Bool Png::Encode( Buffer& buf, const Image& img )
		{
			// 16位 转换到 24位 存储
			if( img->color == COLOR::_2 )
				{
					return Encode( buf, img.Convert( COLOR::_3 ) );
				}

			// 创建 PNG write 结构
			if( png_structp png_ptr =
					png_create_write_struct( PNG_LIBPNG_VER_STRING,
											 nullptr, nullptr, nullptr ) )
				{
					// 创建 PNG信息
					if( png_infop png_info_ptr =
							png_create_info_struct( png_ptr ) )
						{
							// 设置 写入回调函数 将buffer传入
							png_set_write_fn( png_ptr, &buf,
											  WriteCallback, nullptr );

							// 设置 Info
							const Int16 width{ img->width };
							const Int16 height{ img->height };

							png_set_IHDR( png_ptr, png_info_ptr,
										  width, height, 8,
										  this->Convert( img->color ),
										  PNG_INTERLACE_NONE,
										  PNG_COMPRESSION_TYPE_BASE,
										  PNG_FILTER_TYPE_BASE );

							// 写入 PNG 信息
							png_write_info( png_ptr, png_info_ptr );

							// 将Image的所有行的指针 装进rowPointers数组传入 减少中间拷贝
							stl::vector< png_bytep > rowPointers( height );

							for( png_uint_16 c{ 0 }; c < height; ++c )
								{
									rowPointers[ height - c - 1 ] =
										const_cast< png_bytep >( img.Row( c ) );
								}

							// 压缩写入 image 位图
							png_write_image( png_ptr, &rowPointers[ 0 ] );

							// 释放png资源
							png_destroy_info_struct( png_ptr, &png_info_ptr );
							png_destroy_write_struct( &png_ptr, &png_info_ptr );

							return true;
						}
				}

			return false;
		}


	/*
	 @	ReadCallback
	 */
	void Png::ReadCallback( png_structp png_ptr,
							png_bytep data,
							png_size_t size )
		{
			// 取出 buffer
			const Buffer* buf
				{
					static_cast< const Buffer* >
						(
							png_get_io_ptr( png_ptr )
						)
				};

			// 从传递的 buffer 里面读取
			buf->Read( data, size );
		}


	/*
	 @	WriteCallback
	 */
	void Png::WriteCallback( png_structp png_ptr,
							 png_bytep data,
							 png_size_t size )
		{
			// 取出 buffer
			Buffer* buf
				{
					static_cast< Buffer* >
						(
							png_get_io_ptr( png_ptr )
						)
				};

			// 将传入的数据写入 buffer
			buf->Write( data, size );
		}


	/*
	 @	Convert color type
	 */
	COLOR Png::Convert( png_int_32 color_type ) noexcept
		{
			switch( color_type )
				{
					case PNG_COLOR_TYPE_GRAY: return COLOR::_1;
					case PNG_COLOR_TYPE_RGB	: return COLOR::_3;
					case PNG_COLOR_TYPE_RGBA: return COLOR::_4;
				}

			return COLOR::UNKNOWN;
		}
}