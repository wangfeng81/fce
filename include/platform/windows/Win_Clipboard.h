/*
**
**	Win_Clipboard.
**
		Windows platform implementation

	Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::win
{
	/*
	 !	Win_Clipboard
	 */
	class Win_Clipboard final
		{
			/*	Friends
			*/
			friend Clipboard;

			friend Win_Window;

			/*	Using inner kit
			*/
			using EVENT = clipboard::EVENT;

			/*	Object variants
			*/
			using Variant = LPVOID;

			using const_Variant = LPCVOID;

			/*	Encode & Decode callbacks
			*/
			using Encoder = VOID( * )( const_Variant, VOID* ) noexcept;

			using Decoder = VOID( * )( Variant, const VOID*,
												const SIZE_T ) noexcept;

		public:
			/*	Bitmap
			*/
			using Bitmap = image::Bitmap;

			/*	Get & Set HBITMAP
			*/
			static HBITMAP GetBitmap() noexcept;

			static BOOL Set( const HBITMAP ) noexcept;

		protected:
			/*	Clear
			*/
			static BOOL Clear() noexcept;

			/*	Get object
			*/
			static BOOL Get( String& ) noexcept;

			static BOOL Get( Image& ) noexcept;

			static BOOL Get( Buffer& ) noexcept;

			/*	Set object
			*/
			static BOOL Set( const stl::wstring& ) noexcept;

			static BOOL Set( const Image& ) noexcept;

			static BOOL Set( const Buffer& ) noexcept;

			/*	Has object
			*/
			static BOOL Has( const UINT format ) noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Win_Clipboard );

			~Win_Clipboard() = default;

			Win_Clipboard() = default;

			/*	Get format
			*/
			static constexpr UINT GetFormat( const UINT ) noexcept;

			/*	Unified getting and setting objects
			*/
			static BOOL Get( Variant, const Decoder,
									  const UINT format ) noexcept;

			static BOOL Set( const_Variant, const SIZE_T,
											const Encoder,
											const UINT format ) noexcept;

			/*	Encoding function for callback
			*/
			static VOID EncodeString( const_Variant, VOID* ) noexcept;

			static VOID EncodeImage( const_Variant, VOID* ) noexcept;

			static VOID EncodeBuffer( const_Variant, VOID* ) noexcept;

			/*	Decoding function for callback
			*/
			static VOID DecodeString( Variant, const VOID*,
											   const SIZE_T ) noexcept;

			static VOID DecodeImage( Variant, const VOID*,
											  const SIZE_T ) noexcept;

			static VOID DecodeBuffer( Variant, const VOID*,
											   const SIZE_T ) noexcept;

			/*	Handle messages
			*/
			static VOID on_WndProc( const UINT,
									const WPARAM,
									const LPARAM ) noexcept;

		private:
			/*	Buffer format
			*/
			static const UINT CF_BUFFER;

			/*	Format list
			*/
			static constexpr UINT TYPE_LIST[]
				{
					CF_UNICODETEXT,
					CF_DIBV5,

					CF_MAX
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::clipboard
{
	/*
	 @	Clipboard win impl
	 */
	using Impl = win::Win_Clipboard;

	/*
	 @	Win_Clipboard singleton shortcut.
	 */
	#define s_Win_Clipboard s_Clipboard.impl
}



/**
 !	class Win_Clipboard
 */
namespace fce::inner::win
{
	/*
	 @	Has object
	 */
	FC_INLINE BOOL Win_Clipboard::Has( const UINT format ) noexcept
		{
			return IsClipboardFormatAvailable( GetFormat( format ) );
		}

	/*
	 @	Get format
	 */
	FC_INLINE constexpr UINT Win_Clipboard::GetFormat( const UINT format ) noexcept
		{
			return CF_MAX == format
				 ? CF_BUFFER
				 : format;
		}
}