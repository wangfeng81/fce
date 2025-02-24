/*
**
**	Png.
**
		解码遇到 坏数据的 异常宏替换

		#ifndef PNG_ABORT
			#define PNG_ABORT() throw RESULT::DATA_ERROR;
		#endif

	Copyright (c) WangFeng since 2006...
*/


#include <png.h>

namespace fce::inner::image
{
	/*
	!	Png
	*/
	class Png final
		:
		public Codec
		{
		public:
			/*	
			*/
			Bool CheckType( const Buffer& ) const override;

			Bool GetInfo( const Buffer&, Info& ) const override;

			Bool Decode( const Buffer&, Image& ) override;

			Bool Encode( Buffer&, const Image& ) override;

			/*
			 @	Singleton instance
			 */
			static Png instance;

		protected:
			/*	Constructor & Destructor
			*/
			~Png() override = default;

			Png() = default;

			/*	Callback
			*/
			static void	ReadCallback( png_structp,
									  png_bytep data,
									  png_size_t size );

			static void	WriteCallback( png_structp,
									   png_bytep data,
									   png_size_t size );

			/*	Convert
			*/
			[[nodiscard]] static COLOR Convert( png_int_32 color_type ) noexcept;

			[[nodiscard]] static constexpr auto Convert( COLOR );

		private:
			/*	Color list
			*/
			static constexpr png_int_32 COLOR_LIST[]
				{
					-1,
					PNG_COLOR_TYPE_GRAY,
					-1,
					PNG_COLOR_TYPE_RGB,
					PNG_COLOR_TYPE_RGBA
				};
		};
}



/**
 !	class Png
 */
namespace fce::inner::image
{
	/*
	 @	ConvertType
	 */
	FC_INLINE constexpr auto Png::Convert( COLOR color )
		{
			FC_ASSERT( color > COLOR::UNKNOWN
					&& color < COLOR::MAXIMUM );

			return COLOR_LIST[ UInt8( color ) ];
		}
}