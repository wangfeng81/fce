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
			// ���PNG��־��С
			const png_size_t PNGSIGSIZE{ 8 };

			return buf.Size() > PNGSIGSIZE
					&& png_check_sig( *buf, PNGSIGSIZE );
		}


	/*
	 @	GetInfo
	 */
	Bool Png::GetInfo( const Buffer& buf, Info& info ) const
		{
			// ���� PNG read �ṹ
			if( png_structp png_ptr =
					png_create_read_struct( PNG_LIBPNG_VER_STRING,
											nullptr, nullptr, nullptr ) )
				{
					// ����PNG��Ϣ
					if( png_infop png_info_ptr =
							png_create_info_struct( png_ptr ) )
						{
							// ���� ��ȡ�ص�����
							png_set_read_fn( png_ptr,
											 const_cast<Buffer*>(&buf),
											 ReadCallback );

							// ��ȡ PNG ��Ϣ
							png_read_info( png_ptr, png_info_ptr );

							// ��ȡ Info
							info.width = png_get_image_width( png_ptr, png_info_ptr ),
							info.height = png_get_image_height( png_ptr, png_info_ptr );

							info.color = this->Convert(
												png_get_color_type(
													png_ptr, png_info_ptr ) );

							// �ͷ�png��Դ
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
			// ���� PNG read �ṹ
			if( png_structp png_ptr =
					png_create_read_struct( PNG_LIBPNG_VER_STRING,
											nullptr, nullptr, nullptr ) )
				{
					// ���� PNG��Ϣ
					if( png_infop png_info_ptr =
							png_create_info_struct( png_ptr ) )
						{
							// ���� ��ȡ�ص����� ��buffer����
							png_set_read_fn( png_ptr,
											 const_cast< Buffer* >( &buf ),
											 ReadCallback );

							// ��ȡ PNG ��Ϣ
							png_read_info( png_ptr, png_info_ptr );

							// ��ȡ Info
							png_uint_32 width, height;
							png_int_32 depth, color;

							png_get_IHDR( png_ptr, png_info_ptr,
										  &width, &height, &depth, &color,
										  nullptr, nullptr, nullptr );

							// ��չ��Щ��ʽ λ�����8λ ��ɫ���ʽ
							if( depth < 8						||
								color == PNG_COLOR_TYPE_PALETTE	||
								png_get_valid( png_ptr, png_info_ptr,
											   PNG_INFO_tRNS ) )
								{
									// ������չ
									png_set_expand( png_ptr );

									// ���� info
									png_read_update_info( png_ptr, png_info_ptr );

									// ���»�ȡ info
									png_get_IHDR( png_ptr, png_info_ptr,
												  &width, &height, &depth, &color,
												  nullptr, nullptr, nullptr );
								}

							// �������²��� ���� Image
							img.Set( static_cast< Int16 >( width ),
									 static_cast< Int16 >( height ),
									 this->Convert( color ) );

							// ��Image�������е�ָ�� װ��rowPointers���鴫�� �����м俽��
							stl::vector< png_bytep > rowPointers( height );

							for( png_uint_16 c{ 0 }; c < height; ++c )
								{
									rowPointers[ c ] = img.Row( c );
								}

							// ��ѹ��ȡ image λͼ
							png_read_image( png_ptr, &rowPointers[ 0 ] );

							// �ͷ�png��Դ
							png_destroy_info_struct( png_ptr, &png_info_ptr );
							png_destroy_read_struct( &png_ptr, &png_info_ptr, nullptr );

							return true;
						}
				}

			return false;
		}


	/*
	 @	Encode

			Ŀǰ 16λ û��ֱ��֧�֣�ת 24λ ����
	 */
	Bool Png::Encode( Buffer& buf, const Image& img )
		{
			// 16λ ת���� 24λ �洢
			if( img->color == COLOR::_2 )
				{
					return Encode( buf, img.Convert( COLOR::_3 ) );
				}

			// ���� PNG write �ṹ
			if( png_structp png_ptr =
					png_create_write_struct( PNG_LIBPNG_VER_STRING,
											 nullptr, nullptr, nullptr ) )
				{
					// ���� PNG��Ϣ
					if( png_infop png_info_ptr =
							png_create_info_struct( png_ptr ) )
						{
							// ���� д��ص����� ��buffer����
							png_set_write_fn( png_ptr, &buf,
											  WriteCallback, nullptr );

							// ���� Info
							const Int16 width{ img->width };
							const Int16 height{ img->height };

							png_set_IHDR( png_ptr, png_info_ptr,
										  width, height, 8,
										  this->Convert( img->color ),
										  PNG_INTERLACE_NONE,
										  PNG_COMPRESSION_TYPE_BASE,
										  PNG_FILTER_TYPE_BASE );

							// д�� PNG ��Ϣ
							png_write_info( png_ptr, png_info_ptr );

							// ��Image�������е�ָ�� װ��rowPointers���鴫�� �����м俽��
							stl::vector< png_bytep > rowPointers( height );

							for( png_uint_16 c{ 0 }; c < height; ++c )
								{
									rowPointers[ height - c - 1 ] =
										const_cast< png_bytep >( img.Row( c ) );
								}

							// ѹ��д�� image λͼ
							png_write_image( png_ptr, &rowPointers[ 0 ] );

							// �ͷ�png��Դ
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
			// ȡ�� buffer
			const Buffer* buf
				{
					static_cast< const Buffer* >
						(
							png_get_io_ptr( png_ptr )
						)
				};

			// �Ӵ��ݵ� buffer �����ȡ
			buf->Read( data, size );
		}


	/*
	 @	WriteCallback
	 */
	void Png::WriteCallback( png_structp png_ptr,
							 png_bytep data,
							 png_size_t size )
		{
			// ȡ�� buffer
			Buffer* buf
				{
					static_cast< Buffer* >
						(
							png_get_io_ptr( png_ptr )
						)
				};

			// �����������д�� buffer
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