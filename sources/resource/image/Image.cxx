/*
**
**	Image
**
		Copyright (c) WangFeng since 2000...
*/

#include "Png.cxx"
#include "Info.cxx"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS

	#include "Bitmap.cxx"

#endif


namespace fce
{
	/*	ת������ �б�

			��ͬת��ֻ�� 4 x 3 �����
			������������� ��ͬ��ʽ��ת��
	*/
	constinit const Image::Converter Image::CONVERTER_LIST[ 4 ][ 4 ]
		{
			{
				Convert< Color1, Color1 >,
				Convert< Color1, Color2 >,
				Convert< Color1, Color3 >,
				Convert< Color1, Color4 >
			},
			{
				Convert< Color2, Color1 >,
				Convert< Color2, Color2 >,
				Convert< Color2, Color3 >,
				Convert< Color2, Color4 >
			},
			{
				Convert< Color3, Color1 >,
				Convert< Color3, Color2 >,
				Convert< Color3, Color3 >,
				Convert< Color3, Color4 >
			},
			{
				Convert< Color4, Color1 >,
				Convert< Color4, Color2 >,
				Convert< Color4, Color3 >,
				Convert< Color4, Color4 >
			}
		};


	/*
	 @	���� move
	 */
	Image::Image( Image&& rv ) noexcept
		:
			info( std::move( rv.info ) ),
			buffer( std::move( rv.buffer ) )
		{
		}

	/*
	 @	���� copy
	 */
	Image::Image( const Image& img ) noexcept
		:
			info( img.info ),
			buffer( img.buffer )
		{
		}

	/*
	 @	���� info �趨��С�͸�ʽ�Ŀհ�ͼ
	 */
	Image::Image( const Info& info ) noexcept
		:
			info( info ),
			buffer( info.Capacity() )
		{
			FC_ASSERT( info );
		}

	/*
	 @	���� �趨��С�͸�ʽ�Ŀհ�ͼ
	 */
	Image::Image( const Point size,
				  const COLOR color ) noexcept
		:
			Image( Info( size, color ) )
		{
		}

	/*
	 @	���� �趨��С�͸�ʽ�Ŀհ�ͼ
	 */
	Image::Image( const Int16 width,
				  const Int16 height,
				  const COLOR color ) noexcept
		:
			Image( { width, height }, color )
		{
		}

	/*
	 @	���� �����ļ��� ʧ���� Image ��Ч ��
	 */
	Image::Image( const Path& path )
		:
			Image( Buffer( path ) )
		{
		}

	/*
	 @	���� �������ݣ� ʧ���� Image ��Ч ��
	 */
	Image::Image( const Buffer& buf )
		{
			if( ! buf || ! s_XImage.Decode( *this, buf ) )
				{
					// ���� Image ��Ч
					this->info.value = 0;
				}
		}


	/*
	 @	��ֵ ����
	 */
	Image& Image::operator = ( const Image& img ) noexcept
		{
			this->info = img.info;

			this->buffer = img.buffer;

			return *this;
		}

	/*
	 @	��ֵ �ƶ�
	 */
	Image& Image::operator = ( Image&& img ) noexcept
		{
			this->info = img.info;

			this->buffer = std::move( img.buffer );

			return *this;
		}


	/*
	 @	���л�
	 */
	Buffer& Image::operator >> ( Buffer& buf ) const noexcept
		{
			this->info >> buf;

			return buf << this->buffer;
		}


	/*
	 @	Check whether the Image is valid
	 */
	Image::operator Bool() const noexcept
		{
			return this->info.operator bool()
				&& this->buffer.Size() >= this->info.Capacity();
		}

	/*
	 @	���� info
	 */
	Void Image::Set( const Info& info ) noexcept
		{
			FC_ASSERT( info );

			this->info = info;

			this->buffer.Resize( info.Capacity() );
		}


	/*
	 @	���� ͼ���ļ�
	 */
	RESULT Image::Save( const Path& name,
						const TYPE type ) const noexcept
		{
			// ���뵽 type ���͵�ͼ��
			Buffer buf;

			if( this->Encode( buf, type ) )
				{
					// �����ļ�
					return buf.Save( name );
				}

			return RESULT::DATA_ERROR;
		}


	/*
	 @	ת�� ͼ������ �� ��ɫ��ʽ
	 */
	template< Color_integer SrcColor,
			  Color_integer DstColor >

	FC_INLINE Void Image::Convert( const Void* src,
										 Void* dst, Int32 count )
		{
			DstColor* dstColor
				{
					( DstColor* )dst
				};

			const SrcColor* srcColor
				{
					( const SrcColor* )src
				};

			while( count )
				{
					--count;

					*dstColor++ = DstColor( *srcColor++ );
				}
		}


	/*
	 @	��ȡ ��Ӧ��ת������
	 */
	FC_INLINE Image::Converter Image::GetConverter( const COLOR src,
													const COLOR dst ) noexcept
		{
			// ���ڴ��� COLOR::_0 ��Ҫ��һ
			return CONVERTER_LIST[ UInt8( src ) - 1 ]
								 [ UInt8( dst ) - 1 ];
		}


	/*
	 @	ת�� ͼ������ �� ��ɫ��ʽ ��Ŀ�� Image
	 */
	Image Image::Convert( const COLOR color ) const noexcept
		{
			FC_ASSERT( *this );

			/* �����ɫ��ͬ Ҳ��ֱ�Ӹ���

				if( this->info.color == color )
					{
						return *this;
					}
			*/

			// ���� Ŀ�� img
			Image img( this->Size(), color );

			// ��ȡ ת������
			const Converter converter
				{
					GetConverter( this->Color(), color )
				};

			// ת�� ͼ������ �� ��ɫ��ʽ ��Ŀ�� img
			converter( this->Pixels(), img.Pixels(), this->Area());

			return img;
		}
}