/*
**
**	Buffer.
**
		�������⣺
		
			����µ�union�洢��point rect image::info��
			��Ҫ ��� auto& SeekBeg( this auto&, �޷�����ָ�������

		Copyright (c) WangFeng since 1999...
*/


namespace fce
{
	/*
	 @	����
	 */
	Buffer::~Buffer() noexcept
		{
			// �ͷ� ����
			this->Free();
		}

	/*
	 @	���� Ĭ�Ͽ�
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
	 @	���� move
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
	 @	���� copy
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
	 @	���� ���ڴ�ط�������
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
	 @	���� ָ���ߴ�

			��� size = 0 ����� 1k ����
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
	 @	���� ��ָ�����ݿ�
	 */
	Buffer::Buffer( const Void* const data,
					const UInt size ) NOEXCEPT
		:
			Buffer( size )
		{
			FC_ASSERT( size && data );

			// д�� ����
			std::memcpy( this->beg, data, size );
		}

	/*
	 @	���� �����ļ�
	 */
	Buffer::Buffer( RESULT& res,
					const Path& path ) noexcept
		:
			Buffer()
		{
			// �����ļ� �� ����
			res = this->Load( path );
		}

	/*
	 @	���� �����ļ��� ʧ���� buffer �ڴ�� ��
	 */
	Buffer::Buffer( const Path& path ) noexcept
		:
			Buffer()
		{
			// �����ļ� �� ����
			if( this->Load( path ) != RESULT::SUCCESS )
				{
					// ���ʧ�� ��ջ���
					this->Clear();
				}
		}


	/*
	 @	��ֵ ����
	 */
	Buffer& Buffer::operator = ( const Buffer& buf ) noexcept
		{
			// ���� Ŀ��ߴ�
			const UInt size{ buf.Size() };

			this->Resize( size, false );

			// ���� Ŀ������
			std::memcpy( this->beg, buf.beg, size );

			// ���� ��ǰָ��
			this->cur = this->beg + buf.Pos();

			return *this;
		}

	/*
	 @	��ֵ �ƶ�
	 */
	Buffer& Buffer::operator = ( Buffer&& buf ) noexcept
		{
			//FC_ASSERT( this != &buf );

			// ���� �ڴ�
			std::swap( this->pool, buf.pool );
			std::swap( this->beg, buf.beg );

			// ���� ָ��
			this->end = buf.end;
			this->cur = buf.cur;

			return *this;
		}
}



/**
 !	�������
 */
namespace fce
{
	/*
	 @	��� ����
	 */
	Void Buffer::Clear() noexcept
		{
			// �ͷ� ����
			this->Free();

			// �ÿ� ָ��
			this->pool = nullptr;
			this->beg = nullptr;
			this->end = nullptr;
			this->cur = nullptr;
		}


	/*
	 @	�ͷ� ����
	 */
	Void Buffer::Free() noexcept
		{
			// ��� �Ƿ�Ϊ��
			if( this->Data() )
				{
					// �ͷ� �ڴ�
					s_MemMgmt.Free( this->Data(),
									this->pool );
				}
		}


	/*
	 @	���� size �������� delta
	 */
	Void Buffer::Adjust( const UInt delta ) noexcept
		{
			// ���� delta ���Ŀ���ַ
			const Byte* const dst
				{
					this->cur + delta
				};

			// ���Խ�� �����·���
			if( dst > this->end )
				{
					this->Resize( dst - this->beg,
								  true );
				}
		}


	/*
	 @	���µ��� size

			���� ֻ������
	 */
	Void Buffer::Resize( const UInt size,
						 const Bool retained ) noexcept
		{
			// ������� �Ƿ��㹻
			const UInt capacity{ this->Capacity() };

			if( capacity >= size )
				{
					// ���� end �� size ����
					this->end = this->beg + size;
				}
			else
				{
					// �ȼ�¼�� old pos������ ���ݱ�����
					const UInt oldPos{ this->Pos() };

					// ��ȡ new ��
					MemoryPool* const newPool
						{
							s_MemMgmt.GetPool( size )
						};

					// ���� new ��
					auto newBlock
						{
							static_cast< Byte* >( newPool->Malloc() )
						};

					// ��� �Ƿ���� old ��
					if( this->Data() )
						{
							// ���� old ���� �� new ��
							if( retained )
								{
									std::memcpy( newBlock,
												 this->Data(),
												 this->Size() );
								}

							// �ͷ� old ��
							s_MemMgmt.Free( this->Data(),
											this->pool );
						}

					// ���� ָ��
					this->pool = newPool;
					this->beg = newBlock;
					this->end = newBlock + size;
					this->cur = newBlock + oldPos;
				}
		}


	/*
	 @	����������ݿ� ���� buffer
	 */
	Void Buffer::Set( const Void* const data,
					  const UInt size ) NOEXCEPT
		{
			FC_ASSERT( ! size || ( size && data )  );

			// ���� size
			this->Resize( size );

			// д�� ����
			std::memcpy( this->beg, data, size );

			// ��ǰ�α� ����Ϊ��ʼ
			this->cur = this->beg;
		}


	/*
	 @	д�����ݵ� buffer
	 */
	Buffer& Buffer::Write( const Void* const src,
						   const UInt size ) NOEXCEPT
		{
			FC_ASSERT( ! size || ( size && src )  )

			// ���� size
			this->Adjust( size );

			// д�� ����
			std::memcpy( this->cur, src, size );

			this->cur += size;

			return *this;
		}


	Buffer& Buffer::Write( std::istream& is,
						   const UInt size ) NOEXCEPT
		{
			// ���� size
			this->Adjust( size );

			// д�� ����
			is.read( ( Char* )this->cur, size );

			this->cur += size;

			return *this;
		}


	/*
	 @	���л�	Buffer
	 */
	Buffer& Buffer::operator << ( const Buffer& buf ) noexcept
		{
			// ��������Ҫ����32λ
			FC_ASSERT( buf.Size() < UInt32_MAX );

			// ���� size
			const UInt32 size
				{
					static_cast< UInt32 >( buf.Size() )
				};

			*this << size;

			// д�� ����
			return this->Write( buf.Data(), size );
		}


	const Buffer& Buffer::operator >> ( Buffer& buf ) const noexcept
		{
			// ��ȡ size
			UInt32 size;

			*this >> size;

			// ���� Ŀ�� size
			buf.Resize( size );

			// ��ȡ���ݵ� Ŀ�� buf
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
	 @	�����ļ��� ʧ�����쳣 RESULT ��
	 */
	Void Buffer::Download( const Path& path ) NOEXCEPT
		{
		}
}



/**
 !	��д TYPE
 */
namespace fce
{
	/*
	 @	�� TYPE д������
	 */
	Buffer& Buffer::Write( const TYPE type,
						   const Buffer& buf ) noexcept
		{
			// ���� TYPE
			switch( type )
				{
					// ���� ����
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

					// �ַ� ����
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

					// ���� ����
					case TYPE::BOOL:
						{
							*this << *buf.Ptr< Bool >();
						}
						break;

					// 2 ά���� ����
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

					// 3 ά���� ����
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

					// 4 ά���� ����
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

					// ���� ����
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

					// ��ƥ��� ��������
					default: std::unreachable();
				}

			// �ƶ� Ŀ��ָ��
			buf.Seek( sizeOf[ type ] );

			return *this;
		}


	/*
	 @	�� TYPE ��ȡ����
	 */
	const Buffer& Buffer::Read( const TYPE type,
								Buffer& buf ) const
		{
			// ���� TYPE
			switch( type )
				{
					// ���� ����
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

					// �ַ� ����
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

					// ���� ����
					case TYPE::BOOL:
						{
							*this >> *buf.Ptr< Bool >();
						}
						break;

					// 2 ά���� ����
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

					// 3 ά���� ����
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

					// 4 ά���� ����
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

					// ���� ����
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

					// ��ƥ��� ��������
					default: std::unreachable();
				}

			// �ƶ� Ŀ��ָ��
			buf.Seek( sizeOf[ type ] );

			return *this;
		}
}