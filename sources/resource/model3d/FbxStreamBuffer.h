/*
**
**	Fbx stream buffer.
**
		Copyright (c) WangFeng since 2016...
*/

namespace fce
{
	/*
	 !	FbxStreamBuffer
	 */
	class FC_API FbxStreamBuffer final
		:
		public FbxStream
		{
			/*	ID
			*/
			int idReader;

			/*	Buffer
			*/
			Buffer* buffer;

		public:
			/*
			 @	Constructor & Destructor
			 */
			FbxStreamBuffer() noexcept
				:
					buffer( nullptr ),
					idReader( Fbx::instance.FindReaderID() )
				{
				}

			/*
			 @	Query the current state of the stream
			 */
			EState GetState() noexcept override
				{
					return this->buffer
						 ? this->buffer->Empty() ? EState::eEmpty
												 : EState::eOpen
						 : EState::eClosed;
				}

			/*
			 @	Open the stream
			 */
			bool Open( void* const pStreamData ) noexcept override
				{
					if( this->buffer == nullptr )
						{
							this->buffer = static_cast< Buffer* >( pStreamData );
						}

					if( this->buffer )
						{
							this->buffer->SeekBeg();

							return true;
						}

					return false;
				}

			/*
			 @	Close the stream
			 */
			bool Close() noexcept override
				{
					this->buffer = nullptr;

					return true;
				}

			/*
			 @	Empties the internal data of the stream
			 */
			bool Flush() noexcept override
				{
					return true;
				}

			/*
			 @	Writes a memory block
			 */
			std::size_t Write( const void* /*pData*/, FbxUInt64 /*pSize*/ ) noexcept override
				{
					return 0;
				}

			/*
			 @	Read bytes from the stream and store them in the memory block
			 */
			std::size_t Read( void* const pData,
							  const FbxUInt64 pSize ) const NOEXCEPT override
				{
					FC_ASSERT( this->buffer );

					const UInt actualSize
						{
							std::min( static_cast< UInt >( pSize ),
									  this->buffer->CurSize() )
						};

					this->buffer->Read( pData, actualSize );

					return actualSize;
				}

			/*
			 @	Get reader id
			 */
			int GetReaderID() const noexcept override
				{
					return this->idReader;
				}

			/*
			 @	Get writer id
			 */
			int GetWriterID() const noexcept override
				{
					return -1;
				}

			/*
			 @	Adjust the current stream position
			 */
			void Seek( const FbxInt64& pOffset,
					   const FbxFile::ESeekPos& pSeekPos ) NOEXCEPT override
				{
					FC_ASSERT( this->buffer );

					switch( pSeekPos )
						{
							case FbxFile::eBegin:
								// pOffset can not be negatived
								this->buffer->SeekBeg( UInt( pOffset ) );
								break;

							case FbxFile::eCurrent:
								this->buffer->Seek( pOffset );
								break;

							case FbxFile::eEnd:
								// pOffset can not be negatived
								FC_ASSERT( pOffset >= 0 );

								this->buffer->SeekEnd( UInt( pOffset ) );
								break;
						}
				}

			/*
			 @	Get the current stream position
			 */
			FbxInt64 GetPosition() const NOEXCEPT override
				{
					FC_ASSERT( this->buffer );

					return this->buffer->Pos();
				}

			/*
			 @	Set the current stream position
			 */
			void SetPosition( const FbxInt64 pPosition ) NOEXCEPT override
				{
					FC_ASSERT( this->buffer );

					this->Seek( pPosition, FbxFile::eBegin );
				}

			/*
			 @	Get error
			 */
			int GetError() const noexcept override
				{
					return 0;
				}

			/*
			 @	Clear current error condition by setting the current error value to 0
			 */
			void ClearError() noexcept override
				{
				}
		};
}