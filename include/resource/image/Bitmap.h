/*
**
**	Bitmap.
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce::inner::image
{
	/*
	 !	Bitmap
	 */
	class Bitmap final
		{
			/*	Friends
			*/
			friend win::Win_Clipboard;

		public:
			/*	Constructor
			*/
			~Bitmap() noexcept;

			Bitmap() = default;

			Bitmap( Bitmap&& ) noexcept;

			Bitmap( const Bitmap& ) NOEXCEPT;

			Bitmap( const HBITMAP ) NOEXCEPT;

			Bitmap( const Image& ) NOEXCEPT;

			Bitmap( const HDC,
					const Image& ) NOEXCEPT;

			/*	Release
			*/
			BOOL Release() noexcept;

			/*	Assignment
			*/
			Bitmap& operator = ( Bitmap&& ) noexcept;

			Bitmap& operator = ( const Image& ) NOEXCEPT;

			Bitmap& operator = ( const HBITMAP ) NOEXCEPT;

			Bitmap& operator = ( const Bitmap& ) NOEXCEPT;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator Bool() const noexcept;

			/*	Get HBITMAP
			*/
			[[nodiscard]] operator const HBITMAP() const noexcept;

			/*	Clone HBITMAP
			*/
			[[nodiscard]] static HBITMAP Clone( const HBITMAP ) NOEXCEPT;

			/*	Create HBITMAP frome Image
			*/
			[[nodiscard]] static HBITMAP Create( const HDC,
												 const Image& ) NOEXCEPT;

			[[nodiscard]] static HBITMAP Create( const Image& ) NOEXCEPT;

			/*	Convert HBITMAP to Image
			*/
			static BOOL Convert( const HDC,
								 const HBITMAP, Image& ) NOEXCEPT;

			static BOOL Convert( const HBITMAP, Image& ) NOEXCEPT;

		private:
			/*	Convert Image <=> DIBV5
			*/
			static BOOL DIBV5_to_Image( Image&, const BITMAPV5HEADER&,
												const VOID* ) NOEXCEPT;

			static BOOL Image_to_DIBV5( const Image&, BITMAPV5HEADER&,
													  VOID* ) NOEXCEPT;

			/*	Convert Image::Info <=> BITMAPV5HEADER
			*/
			static BITMAPV5HEADER Info_to_DIBV5Header( const Info& ) noexcept;

			/*	Buffer size for Image conversion to DIBV5
			*/
			static UInt Buf_size_for_img_to_dibv5( const Info& ) noexcept;

		private:
			/*	HBITMAP
			*/
			HBITMAP hBitmap;
		};
}



/**
 !	class Bitmap
 */
namespace fce::inner::image
{
	/*
	 @	Check whether the bitmap is valid
	 */
	FC_INLINE Bitmap::operator Bool() const noexcept
		{
			return this->hBitmap;
		}

	/*
	 @	Get HBITMAP
	 */
	FC_INLINE Bitmap::operator const HBITMAP() const noexcept
		{
			return this->hBitmap;
		}

	/*
	 @	Move assignment
	 */
	FC_INLINE Bitmap& Bitmap::operator = ( Bitmap&& bmp ) noexcept
		{
			std::swap( this->hBitmap, bmp.hBitmap );

			return *this;
		}

	/*
	 @	Copy assignment
	 */
	FC_INLINE Bitmap& Bitmap::operator = ( const Bitmap& bmp ) NOEXCEPT
		{
			return *this = bmp.hBitmap;
		}
}