/*
**
**	Win_Cursor.
**
		Windows platform implementation

	Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::win
{
	/*
	 !	Win_Cursor
	 */
	class Win_Cursor final
		{
			/*	Friends
			*/
			friend Cursor;

		public:
			/*	Bitmap
			*/
			using Bitmap = image::Bitmap;

			/*	Get & Set HBITMAP
			*/
			static HBITMAP GetBitmap();

			static BOOL Set( const HBITMAP );

		protected:
			/*	Set
			*/
			Bool Set( TYPE );

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Win_Cursor );

			~Win_Cursor() = default;

			Win_Cursor() = default;

		private:
			/*	HCURSOR
			*/
			HCURSOR hCursor;

			/*	TYPE list
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
namespace fce::inner::Cursor
{
	/*
	 @	Cursor win impl
	 */
	using Impl = win::Win_Cursor;

	/*
	 @	Win_Cursor singleton shortcut.
	 */
	#define s_Win_Cursor s_Cursor.impl
}



/**
 !	class Win_Cursor
 */
namespace fce::inner::win
{
	/*
	 @	Has object
	 */
	FC_INLINE BOOL Win_Cursor::Has( const UINT format )
		{
			return IsCursorFormatAvailable( GetFormat( format ) );
		}
}