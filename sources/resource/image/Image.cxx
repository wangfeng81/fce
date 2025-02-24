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
	/*	转换函数 列表

			不同转换只有 4 x 3 种组合
			这里包含了四种 相同格式的转换
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
	 @	构造 move
	 */
	Image::Image( Image&& rv ) noexcept
		:
			info( std::move( rv.info ) ),
			buffer( std::move( rv.buffer ) )
		{
		}

	/*
	 @	构造 copy
	 */
	Image::Image( const Image& img ) noexcept
		:
			info( img.info ),
			buffer( img.buffer )
		{
		}

	/*
	 @	构造 info 设定大小和格式的空白图
	 */
	Image::Image( const Info& info ) noexcept
		:
			info( info ),
			buffer( info.Capacity() )
		{
			FC_ASSERT( info );
		}

	/*
	 @	构造 设定大小和格式的空白图
	 */
	Image::Image( const Point size,
				  const COLOR color ) noexcept
		:
			Image( Info( size, color ) )
		{
		}

	/*
	 @	构造 设定大小和格式的空白图
	 */
	Image::Image( const Int16 width,
				  const Int16 height,
				  const COLOR color ) noexcept
		:
			Image( { width, height }, color )
		{
		}

	/*
	 @	构造 加载文件（ 失败则 Image 无效 ）
	 */
	Image::Image( const Path& path )
		:
			Image( Buffer( path ) )
		{
		}

	/*
	 @	构造 解码数据（ 失败则 Image 无效 ）
	 */
	Image::Image( const Buffer& buf )
		{
			if( ! buf || ! s_XImage.Decode( *this, buf ) )
				{
					// 设置 Image 无效
					this->info.value = 0;
				}
		}


	/*
	 @	赋值 拷贝
	 */
	Image& Image::operator = ( const Image& img ) noexcept
		{
			this->info = img.info;

			this->buffer = img.buffer;

			return *this;
		}

	/*
	 @	赋值 移动
	 */
	Image& Image::operator = ( Image&& img ) noexcept
		{
			this->info = img.info;

			this->buffer = std::move( img.buffer );

			return *this;
		}


	/*
	 @	序列化
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
	 @	设置 info
	 */
	Void Image::Set( const Info& info ) noexcept
		{
			FC_ASSERT( info );

			this->info = info;

			this->buffer.Resize( info.Capacity() );
		}


	/*
	 @	保存 图像文件
	 */
	RESULT Image::Save( const Path& name,
						const TYPE type ) const noexcept
		{
			// 编码到 type 类型的图像
			Buffer buf;

			if( this->Encode( buf, type ) )
				{
					// 保存文件
					return buf.Save( name );
				}

			return RESULT::DATA_ERROR;
		}


	/*
	 @	转换 图像阵列 的 颜色格式
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
	 @	获取 相应的转换函数
	 */
	FC_INLINE Image::Converter Image::GetConverter( const COLOR src,
													const COLOR dst ) noexcept
		{
			// 由于存在 COLOR::_0 需要减一
			return CONVERTER_LIST[ UInt8( src ) - 1 ]
								 [ UInt8( dst ) - 1 ];
		}


	/*
	 @	转换 图像阵列 的 颜色格式 到目标 Image
	 */
	Image Image::Convert( const COLOR color ) const noexcept
		{
			FC_ASSERT( *this );

			/* 如果颜色相同 也可直接复制

				if( this->info.color == color )
					{
						return *this;
					}
			*/

			// 创建 目标 img
			Image img( this->Size(), color );

			// 获取 转换函数
			const Converter converter
				{
					GetConverter( this->Color(), color )
				};

			// 转换 图像阵列 的 颜色格式 到目标 img
			converter( this->Pixels(), img.Pixels(), this->Area());

			return img;
		}
}