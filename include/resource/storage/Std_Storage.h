/*
**
**	Std_Storage.
**
		Cpp std implementation

	Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::storage
{
	/*
	 !	Std_Storage
	 */
	class Std_Storage final
		:
		public Storage
		{
			/*	Friends
			*/
			friend StorageManagement;

		public:
			/*	Load & Save
			*/
			RESULT Load( const Path&,
						 Buffer& ) const;

			RESULT Save( const Path&,
						 const Buffer& );

			RESULT Save( const Path&,
						 const Void*,
						 const UInt size );

			/*	Copy
			*/
			RESULT Copy( const Path& src,
						 const Path& dst );

			/*	Rename
			*/
			RESULT Rename( const Path& src,
						   const Path& dst );

			/*	Remove
			*/
			RESULT Remove( const Path& );

			/*	Make folder
			*/
			RESULT MakeFolder( const Path& );

			/*	Exists
			*/
			[[nodiscard]] Bool Exists( const Path& ) const noexcept;

			/*	Get available space
			*/
			[[nodiscard]] UInt64 GetAvailableSpace() const noexcept;

			/*	Get file size
			*/
			[[nodiscard]] UInt64 GetSize( const Path& ) const noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Std_Storage );

			~Std_Storage() override = default;

			Std_Storage();

			/*	Load
			*/
			RESULT Load( const Path&,
							   Void*,
						 const UInt size ) const;

			/*	On error
			*/
			static RESULT on_Error( const std::error_code& ) noexcept;
		};


	/*
	 @	Local storage implementation
	 */
	using Local = Std_Storage;
}



/**
 !	class Std_Storage
 */
namespace fce::inner::storage
{
	/*
	 @	Save
	 */
	FC_INLINE RESULT Std_Storage::Save( const Path& name,
										const Buffer& buf )
		{
			return this->Save( name, buf.Data(), buf.Size() );
		}
}