/*
**
**	Buffer.
**
		待解问题：
		
			大端下的union存储，point rect image::info等
			需要 诊断 auto& SeekBeg( this auto&, 无法调用指针的问题

		Copyright (c) WangFeng since 1999...
*/


namespace fce
{
	/*
	 @	析构
	 */
	Buffer::~Buffer() noexcept
		{
			// 释放 缓存
			this->Free();
		}

	/*
	 @	构造 默认空
	 */
	Buffer::Buffer() noexcept
		:
			pool( nullptr ),
			beg( nullptr ),
			end( nullptr ),
			cur( nullptr )
		{
		}

	/*
	 @	构造 move
	 */
	Buffer::Buffer( Buffer&& rv ) noexcept
		:
			pool( rv.pool ),
			beg( rv.beg ),
			end( rv.end ),
			cur( rv.cur )
		{
			rv.beg = nullptr;
		}

	/*
	 @	构造 copy
	 */
	Buffer::Buffer( const Buffer& buf ) noexcept
		:
			pool( buf.pool ),
			beg( static_cast< Byte* >( pool ? pool->Malloc() : nullptr ) ),
			end( beg + buf.Size() ),
			cur( beg + buf.Pos() )
		{
			std::memcpy( this->beg, buf.beg, buf.Size() );
		}

	/*
	 @	构造 按内存池分配容量
	 */
	Buffer::Buffer( MemoryPool& pool ) noexcept
		:
			pool( &pool ),
			beg( static_cast< Byte* >( pool.Malloc() ) ),
			end( beg ),
			cur( beg )
		{
		}

	/*
	 @	构造 指定尺寸

			如果 size = 0 则分配 1k 容量
	 */
	Buffer::Buffer( const UInt size ) noexcept
		:
			pool( s_MemMgmt.GetPool( size ? size : 1024 ) ),
			beg( static_cast< Byte* >( pool->Malloc() ) ),
			end( beg + size ),
			cur( beg )
		{
		}

	/*
	 @	构造 按指定数据块
	 */
	Buffer::Buffer( const Void* const data,
					const UInt size ) NOEXCEPT
		:
			Buffer( size )
		{
			FC_ASSERT( size && data );

			// 写入 数据
			std::memcpy( this->beg, data, size );
		}

	/*
	 @	构造 加载文件
	 */
	Buffer::Buffer( RESULT& res,
					const Path& path ) noexcept
		:
			Buffer()
		{
			// 加载文件 到 缓冲
			res = this->Load( path );
		}

	/*
	 @	构造 加载文件（ 失败则 buffer 内存空 ）
	 */
	Buffer::Buffer( const Path& path ) noexcept
		:
			Buffer()
		{
			// 加载文件 到 缓冲
			if( this->Load( path ) != RESULT::SUCCESS )
				{
					// 如果失败 清空缓存
					this->Clear();
				}
		}


	/*
	 @	赋值 拷贝
	 */
	Buffer& Buffer::operator = ( const Buffer& buf ) noexcept
		{
			// 调整 目标尺寸
			const UInt size{ buf.Size() };

			this->Resize( size, false );

			// 复制 目标数据
			std::memcpy( this->beg, buf.beg, size );

			// 更新 当前指针
			this->cur = this->beg + buf.Pos();

			return *this;
		}

	/*
	 @	赋值 移动
	 */
	Buffer& Buffer::operator = ( Buffer&& buf ) noexcept
		{
			//FC_ASSERT( this != &buf );

			// 交换 内存
			std::swap( this->pool, buf.pool );
			std::swap( this->beg, buf.beg );

			// 更新 指针
			this->end = buf.end;
			this->cur = buf.cur;

			return *this;
		}
}



/**
 !	常规操作
 */
namespace fce
{
	/*
	 @	清空 缓存
	 */
	Void Buffer::Clear() noexcept
		{
			// 释放 缓存
			this->Free();

			// 置空 指针
			this->pool = nullptr;
			this->beg = nullptr;
			this->end = nullptr;
			this->cur = nullptr;
		}


	/*
	 @	释放 缓存
	 */
	Void Buffer::Free() noexcept
		{
			// 检测 是否为空
			if( this->Data() )
				{
					// 释放 内存
					s_MemMgmt.Free( this->Data(),
									this->pool );
				}
		}


	/*
	 @	调整 size 到能容下 delta
	 */
	Void Buffer::Adjust( const UInt delta ) noexcept
		{
			// 加上 delta 后的目标地址
			const Byte* const dst
				{
					this->cur + delta
				};

			// 如果越界 则重新分配
			if( dst > this->end )
				{
					this->Resize( dst - this->beg,
								  true );
				}
		}


	/*
	 @	重新调整 size

			容量 只增不减
	 */
	Void Buffer::Resize( const UInt size,
						 const Bool retained ) noexcept
		{
			// 检测容量 是否足够
			const UInt capacity{ this->Capacity() };

			if( capacity >= size )
				{
					// 调整 end 到 size 即可
					this->end = this->beg + size;
				}
			else
				{
					// 先记录下 old pos，避免 数据被覆盖
					const UInt oldPos{ this->Pos() };

					// 获取 new 池
					MemoryPool* const newPool
						{
							s_MemMgmt.GetPool( size )
						};

					// 分配 new 块
					auto newBlock
						{
							static_cast< Byte* >( newPool->Malloc() )
						};

					// 检查 是否存在 old 块
					if( this->Data() )
						{
							// 复制 old 数据 到 new 块
							if( retained )
								{
									std::memcpy( newBlock,
												 this->Data(),
												 this->Size() );
								}

							// 释放 old 块
							s_MemMgmt.Free( this->Data(),
											this->pool );
						}

					// 更新 指针
					this->pool = newPool;
					this->beg = newBlock;
					this->end = newBlock + size;
					this->cur = newBlock + oldPos;
				}
		}


	/*
	 @	按传入的数据块 设置 buffer
	 */
	Void Buffer::Set( const Void* const data,
					  const UInt size ) NOEXCEPT
		{
			FC_ASSERT( ! size || ( size && data )  );

			// 重置 size
			this->Resize( size );

			// 写入 数据
			std::memcpy( this->beg, data, size );

			// 当前游标 设置为起始
			this->cur = this->beg;
		}


	/*
	 @	写入数据到 buffer
	 */
	Buffer& Buffer::Write( const Void* const src,
						   const UInt size ) NOEXCEPT
		{
			FC_ASSERT( ! size || ( size && src )  )

			// 适配 size
			this->Adjust( size );

			// 写入 数据
			std::memcpy( this->cur, src, size );

			this->cur += size;

			return *this;
		}


	Buffer& Buffer::Write( std::istream& is,
						   const UInt size ) NOEXCEPT
		{
			// 适配 size
			this->Adjust( size );

			// 写入 数据
			is.read( ( Char* )this->cur, size );

			this->cur += size;

			return *this;
		}


	/*
	 @	序列化	Buffer
	 */
	Buffer& Buffer::operator << ( const Buffer& buf ) noexcept
		{
			// 几乎不需要超过32位
			FC_ASSERT( buf.Size() < UInt32_MAX );

			// 存入 size
			const UInt32 size
				{
					static_cast< UInt32 >( buf.Size() )
				};

			*this << size;

			// 写入 数据
			return this->Write( buf.Data(), size );
		}


	const Buffer& Buffer::operator >> ( Buffer& buf ) const noexcept
		{
			// 获取 size
			UInt32 size;

			*this >> size;

			// 调整 目标 size
			buf.Resize( size );

			// 读取数据到 目标 buf
			return this->Read( buf.Data(), size );
		}

	/*
	 @	Crc32
	 */
	Crc32& Buffer::operator >> ( Crc32& crc32 ) const noexcept
		{
			return crc32( this->beg, this->end );
		}


	/*
	 @	下载文件（ 失败抛异常 RESULT ）
	 */
	Void Buffer::Download( const Path& path ) NOEXCEPT
		{
		}
}



/**
 !	读写 TYPE
 */
namespace fce
{
	/*
	 @	按 TYPE 写入数据
	 */
	Buffer& Buffer::Write( const TYPE type,
						   const Buffer& buf ) noexcept
		{
			// 分流 TYPE
			switch( type )
				{
					// 数据 类型
					case TYPE::INT8:
						{
							*this << *buf.Ptr< Int8 >();
						}
						break;

					case TYPE::UINT8:
						{
							*this << *buf.Ptr< UInt8 >();
						}
						break;

					case TYPE::INT16:
						{
							*this << *buf.Ptr< Int16 >();
						}
						break;

					case TYPE::UINT16:
						{
							*this << *buf.Ptr< UInt16 >();
						}
						break;

					case TYPE::INT32:
						{
							*this << *buf.Ptr< Int32 >();
						}
						break;

					case TYPE::UINT32:
						{
							*this << *buf.Ptr< UInt32 >();
						}
						break;

					case TYPE::INT64:
						{
							*this << *buf.Ptr< Int64 >();
						}
						break;

					case TYPE::UINT64:
						{
							*this << *buf.Ptr< UInt64 >();
						}
						break;

					case TYPE::FLOAT32:
						{
							*this << *buf.Ptr< Float32 >();
						}
						break;

					case TYPE::FLOAT64:
						{
							*this << *buf.Ptr< Float64 >();
						}
						break;

					// 字符 类型
					case TYPE::CHAR:
						{
							*this << *buf.Ptr< Char >();
						}
						break;

					case TYPE::WCHAR:
						{
							*this << *buf.Ptr< WChar >();
						}
						break;

					case TYPE::CHAR8:
						{
							*this << *buf.Ptr< Char8 >();
						}
						break;

					case TYPE::CHAR16:
						{
							*this << *buf.Ptr< Char16 >();
						}
						break;

					case TYPE::CHAR32:
						{
							*this << *buf.Ptr< Char32 >();
						}
						break;

					// 布尔 类型
					case TYPE::BOOL:
						{
							*this << *buf.Ptr< Bool >();
						}
						break;

					// 2 维向量 类型
					case TYPE::VECTOR2_I8:
						{
							*this << buf.Ptr< Int8 >()[ 0 ];
									 buf.Ptr< Int8 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_U8:
						{
							*this << buf.Ptr< UInt8 >()[ 0 ];
									 buf.Ptr< UInt8 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_I16:
						{
							*this << buf.Ptr< Int16 >()[ 0 ];
									 buf.Ptr< Int16 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_U16:
						{
							*this << buf.Ptr< UInt16 >()[ 0 ];
									 buf.Ptr< UInt16 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_I32:
						{
							*this << buf.Ptr< Int32 >()[ 0 ];
									 buf.Ptr< Int32 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_U32:
						{
							*this << buf.Ptr< UInt32 >()[ 0 ];
									 buf.Ptr< UInt32 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_I64:
						{
							*this << buf.Ptr< Int64 >()[ 0 ];
									 buf.Ptr< Int64 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_U64:
						{
							*this << buf.Ptr< UInt64 >()[ 0 ];
									 buf.Ptr< UInt64 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_F32:
						{
							*this << buf.Ptr< Float32 >()[ 0 ];
									 buf.Ptr< Float32 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_F64:
						{
							*this << buf.Ptr< Float64 >()[ 0 ];
									 buf.Ptr< Float64 >()[ 1 ];
						}
						break;

					// 3 维向量 类型
					case TYPE::VECTOR3_I8:
						{
							*this << buf.Ptr< Int8 >()[ 0 ];
									 buf.Ptr< Int8 >()[ 1 ];
									 buf.Ptr< Int8 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_U8:
						{
							*this << buf.Ptr< UInt8 >()[ 0 ];
									 buf.Ptr< UInt8 >()[ 1 ];
									 buf.Ptr< UInt8 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_I16:
						{
							*this << buf.Ptr< Int16 >()[ 0 ];
									 buf.Ptr< Int16 >()[ 1 ];
									 buf.Ptr< Int16 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_U16:
						{
							*this << buf.Ptr< UInt16 >()[ 0 ];
									 buf.Ptr< UInt16 >()[ 1 ];
									 buf.Ptr< UInt16 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_I32:
						{
							*this << buf.Ptr< Int32 >()[ 0 ];
									 buf.Ptr< Int32 >()[ 1 ];
									 buf.Ptr< Int32 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_U32:
						{
							*this << buf.Ptr< UInt32 >()[ 0 ];
									 buf.Ptr< UInt32 >()[ 1 ];
									 buf.Ptr< UInt32 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_I64:
						{
							*this << buf.Ptr< Int64 >()[ 0 ];
									 buf.Ptr< Int64 >()[ 1 ];
									 buf.Ptr< Int64 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_U64:
						{
							*this << buf.Ptr< UInt64 >()[ 0 ];
									 buf.Ptr< UInt64 >()[ 1 ];
									 buf.Ptr< UInt64 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_F32:
						{
							*this << buf.Ptr< Float32 >()[ 0 ];
									 buf.Ptr< Float32 >()[ 1 ];
									 buf.Ptr< Float32 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_F64:
						{
							*this << buf.Ptr< Float64 >()[ 0 ];
									 buf.Ptr< Float64 >()[ 1 ];
									 buf.Ptr< Float64 >()[ 2 ];
						}
						break;

					// 4 维向量 类型
					case TYPE::VECTOR4_I8:
						{
							*this << buf.Ptr< Int8 >()[ 0 ];
									 buf.Ptr< Int8 >()[ 1 ];
									 buf.Ptr< Int8 >()[ 2 ];
									 buf.Ptr< Int8 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_U8:
						{
							*this << buf.Ptr< UInt8 >()[ 0 ];
									 buf.Ptr< UInt8 >()[ 1 ];
									 buf.Ptr< UInt8 >()[ 2 ];
									 buf.Ptr< UInt8 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_I16:
						{
							*this << buf.Ptr< Int16 >()[ 0 ];
									 buf.Ptr< Int16 >()[ 1 ];
									 buf.Ptr< Int16 >()[ 2 ];
									 buf.Ptr< Int16 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_U16:
						{
							*this << buf.Ptr< UInt16 >()[ 0 ];
									 buf.Ptr< UInt16 >()[ 1 ];
									 buf.Ptr< UInt16 >()[ 2 ];
									 buf.Ptr< UInt16 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_I32:
						{
							*this << buf.Ptr< Int32 >()[ 0 ];
									 buf.Ptr< Int32 >()[ 1 ];
									 buf.Ptr< Int32 >()[ 2 ];
									 buf.Ptr< Int32 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_U32:
						{
							*this << buf.Ptr< UInt32 >()[ 0 ];
									 buf.Ptr< UInt32 >()[ 1 ];
									 buf.Ptr< UInt32 >()[ 2 ];
									 buf.Ptr< UInt32 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_I64:
						{
							*this << buf.Ptr< Int64 >()[ 0 ];
									 buf.Ptr< Int64 >()[ 1 ];
									 buf.Ptr< Int64 >()[ 2 ];
									 buf.Ptr< Int64 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_U64:
						{
							*this << buf.Ptr< UInt64 >()[ 0 ];
									 buf.Ptr< UInt64 >()[ 1 ];
									 buf.Ptr< UInt64 >()[ 2 ];
									 buf.Ptr< UInt64 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_F32:
						{
							*this << buf.Ptr< Float32 >()[ 0 ];
									 buf.Ptr< Float32 >()[ 1 ];
									 buf.Ptr< Float32 >()[ 2 ];
									 buf.Ptr< Float32 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_F64:
						{
							*this << buf.Ptr< Float64 >()[ 0 ];
									 buf.Ptr< Float64 >()[ 1 ];
									 buf.Ptr< Float64 >()[ 2 ];
									 buf.Ptr< Float64 >()[ 3 ];
						}
						break;

					// 布尔 向量
					case TYPE::VECTOR2_B:
						{
							*this << buf.Ptr< Bool >()[ 0 ];
									 buf.Ptr< Bool >()[ 1 ];
						}
						break;

					case TYPE::VECTOR3_B:
						{
							*this << buf.Ptr< Bool >()[ 0 ];
									 buf.Ptr< Bool >()[ 1 ];
									 buf.Ptr< Bool >()[ 2 ];
						}
						break;

					case TYPE::VECTOR4_B:
						{
							*this << buf.Ptr< Bool >()[ 0 ];
									 buf.Ptr< Bool >()[ 1 ];
									 buf.Ptr< Bool >()[ 2 ];
									 buf.Ptr< Bool >()[ 3 ];
						}
						break;

					// 不匹配的 顶点类型
					default: std::unreachable();
				}

			// 移动 目标指针
			buf.Seek( sizeOf[ type ] );

			return *this;
		}


	/*
	 @	按 TYPE 读取数据
	 */
	const Buffer& Buffer::Read( const TYPE type,
								Buffer& buf ) const
		{
			// 分流 TYPE
			switch( type )
				{
					// 数据 类型
					case TYPE::INT8:
						{
							*this >> *buf.Ptr< Int8 >();
						}
						break;

					case TYPE::UINT8:
						{
							*this >> *buf.Ptr< UInt8 >();
						}
						break;

					case TYPE::INT16:
						{
							*this >> *buf.Ptr< Int16 >();
						}
						break;

					case TYPE::UINT16:
						{
							*this >> *buf.Ptr< UInt16 >();
						}
						break;

					case TYPE::INT32:
						{
							*this >> *buf.Ptr< Int32 >();
						}
						break;

					case TYPE::UINT32:
						{
							*this >> *buf.Ptr< UInt32 >();
						}
						break;

					case TYPE::INT64:
						{
							*this >> *buf.Ptr< Int64 >();
						}
						break;

					case TYPE::UINT64:
						{
							*this >> *buf.Ptr< UInt64 >();
						}
						break;

					case TYPE::FLOAT32:
						{
							*this >> *buf.Ptr< Float32 >();
						}
						break;

					case TYPE::FLOAT64:
						{
							*this >> *buf.Ptr< Float64 >();
						}
						break;

					// 字符 类型
					case TYPE::CHAR:
						{
							*this >> *buf.Ptr< Char >();
						}
						break;

					case TYPE::WCHAR:
						{
							*this >> *buf.Ptr< WChar >();
						}
						break;

					case TYPE::CHAR8:
						{
							*this >> *buf.Ptr< Char8 >();
						}
						break;

					case TYPE::CHAR16:
						{
							*this >> *buf.Ptr< Char16 >();
						}
						break;

					case TYPE::CHAR32:
						{
							*this >> *buf.Ptr< Char32 >();
						}
						break;

					// 布尔 类型
					case TYPE::BOOL:
						{
							*this >> *buf.Ptr< Bool >();
						}
						break;

					// 2 维向量 类型
					case TYPE::VECTOR2_I8:
						{
							*this >> buf.Ptr< Int8 >()[ 0 ];
									 buf.Ptr< Int8 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_U8:
						{
							*this >> buf.Ptr< UInt8 >()[ 0 ];
									 buf.Ptr< UInt8 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_I16:
						{
							*this >> buf.Ptr< Int16 >()[ 0 ];
									 buf.Ptr< Int16 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_U16:
						{
							*this >> buf.Ptr< UInt16 >()[ 0 ];
									 buf.Ptr< UInt16 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_I32:
						{
							*this >> buf.Ptr< Int32 >()[ 0 ];
									 buf.Ptr< Int32 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_U32:
						{
							*this >> buf.Ptr< UInt32 >()[ 0 ];
									 buf.Ptr< UInt32 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_I64:
						{
							*this >> buf.Ptr< Int64 >()[ 0 ];
									 buf.Ptr< Int64 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_U64:
						{
							*this >> buf.Ptr< UInt64 >()[ 0 ];
									 buf.Ptr< UInt64 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_F32:
						{
							*this >> buf.Ptr< Float32 >()[ 0 ];
									 buf.Ptr< Float32 >()[ 1 ];
						}
						break;

					case TYPE::VECTOR2_F64:
						{
							*this >> buf.Ptr< Float64 >()[ 0 ];
									 buf.Ptr< Float64 >()[ 1 ];
						}
						break;

					// 3 维向量 类型
					case TYPE::VECTOR3_I8:
						{
							*this >> buf.Ptr< Int8 >()[ 0 ];
									 buf.Ptr< Int8 >()[ 1 ];
									 buf.Ptr< Int8 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_U8:
						{
							*this >> buf.Ptr< UInt8 >()[ 0 ];
									 buf.Ptr< UInt8 >()[ 1 ];
									 buf.Ptr< UInt8 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_I16:
						{
							*this >> buf.Ptr< Int16 >()[ 0 ];
									 buf.Ptr< Int16 >()[ 1 ];
									 buf.Ptr< Int16 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_U16:
						{
							*this >> buf.Ptr< UInt16 >()[ 0 ];
									 buf.Ptr< UInt16 >()[ 1 ];
									 buf.Ptr< UInt16 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_I32:
						{
							*this >> buf.Ptr< Int32 >()[ 0 ];
									 buf.Ptr< Int32 >()[ 1 ];
									 buf.Ptr< Int32 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_U32:
						{
							*this >> buf.Ptr< UInt32 >()[ 0 ];
									 buf.Ptr< UInt32 >()[ 1 ];
									 buf.Ptr< UInt32 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_I64:
						{
							*this >> buf.Ptr< Int64 >()[ 0 ];
									 buf.Ptr< Int64 >()[ 1 ];
									 buf.Ptr< Int64 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_U64:
						{
							*this >> buf.Ptr< UInt64 >()[ 0 ];
									 buf.Ptr< UInt64 >()[ 1 ];
									 buf.Ptr< UInt64 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_F32:
						{
							*this >> buf.Ptr< Float32 >()[ 0 ];
									 buf.Ptr< Float32 >()[ 1 ];
									 buf.Ptr< Float32 >()[ 2 ];
						}
						break;

					case TYPE::VECTOR3_F64:
						{
							*this >> buf.Ptr< Float64 >()[ 0 ];
									 buf.Ptr< Float64 >()[ 1 ];
									 buf.Ptr< Float64 >()[ 2 ];
						}
						break;

					// 4 维向量 类型
					case TYPE::VECTOR4_I8:
						{
							*this >> buf.Ptr< Int8 >()[ 0 ];
									 buf.Ptr< Int8 >()[ 1 ];
									 buf.Ptr< Int8 >()[ 2 ];
									 buf.Ptr< Int8 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_U8:
						{
							*this >> buf.Ptr< UInt8 >()[ 0 ];
									 buf.Ptr< UInt8 >()[ 1 ];
									 buf.Ptr< UInt8 >()[ 2 ];
									 buf.Ptr< UInt8 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_I16:
						{
							*this >> buf.Ptr< Int16 >()[ 0 ];
									 buf.Ptr< Int16 >()[ 1 ];
									 buf.Ptr< Int16 >()[ 2 ];
									 buf.Ptr< Int16 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_U16:
						{
							*this >> buf.Ptr< UInt16 >()[ 0 ];
									 buf.Ptr< UInt16 >()[ 1 ];
									 buf.Ptr< UInt16 >()[ 2 ];
									 buf.Ptr< UInt16 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_I32:
						{
							*this >> buf.Ptr< Int32 >()[ 0 ];
									 buf.Ptr< Int32 >()[ 1 ];
									 buf.Ptr< Int32 >()[ 2 ];
									 buf.Ptr< Int32 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_U32:
						{
							*this >> buf.Ptr< UInt32 >()[ 0 ];
									 buf.Ptr< UInt32 >()[ 1 ];
									 buf.Ptr< UInt32 >()[ 2 ];
									 buf.Ptr< UInt32 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_I64:
						{
							*this >> buf.Ptr< Int64 >()[ 0 ];
									 buf.Ptr< Int64 >()[ 1 ];
									 buf.Ptr< Int64 >()[ 2 ];
									 buf.Ptr< Int64 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_U64:
						{
							*this >> buf.Ptr< UInt64 >()[ 0 ];
									 buf.Ptr< UInt64 >()[ 1 ];
									 buf.Ptr< UInt64 >()[ 2 ];
									 buf.Ptr< UInt64 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_F32:
						{
							*this >> buf.Ptr< Float32 >()[ 0 ];
									 buf.Ptr< Float32 >()[ 1 ];
									 buf.Ptr< Float32 >()[ 2 ];
									 buf.Ptr< Float32 >()[ 3 ];
						}
						break;

					case TYPE::VECTOR4_F64:
						{
							*this >> buf.Ptr< Float64 >()[ 0 ];
									 buf.Ptr< Float64 >()[ 1 ];
									 buf.Ptr< Float64 >()[ 2 ];
									 buf.Ptr< Float64 >()[ 3 ];
						}
						break;

					// 布尔 向量
					case TYPE::VECTOR2_B:
						{
							*this >> buf.Ptr< Bool >()[ 0 ];
									 buf.Ptr< Bool >()[ 1 ];
						}
						break;

					case TYPE::VECTOR3_B:
						{
							*this >> buf.Ptr< Bool >()[ 0 ];
									 buf.Ptr< Bool >()[ 1 ];
									 buf.Ptr< Bool >()[ 2 ];
						}
						break;

					case TYPE::VECTOR4_B:
						{
							*this >> buf.Ptr< Bool >()[ 0 ];
									 buf.Ptr< Bool >()[ 1 ];
									 buf.Ptr< Bool >()[ 2 ];
									 buf.Ptr< Bool >()[ 3 ];
						}
						break;

					// 不匹配的 顶点类型
					default: std::unreachable();
				}

			// 移动 目标指针
			buf.Seek( sizeOf[ type ] );

			return *this;
		}
}