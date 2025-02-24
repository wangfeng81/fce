/*
**
**	Std_Charset.
**
		Std implementation

			std::codecvt 直接支持的转换

				utf32 <=> utf8
				utf16 <=> utf8
				wcs	  <=> mbcs

			自定义的 间接转换

				utf32 <=> utf16		by utf8
				mbcs  <=> unicode	by wcs
				wcs   <=> unicode	by utf16 or 32 ( Mapping_wchar )

	Copyright (c) WangFeng since 2012...
*/

namespace fce::inner::charset
{
	/*
	 @	构造
	 
			locale 传入空串，获取本地 mbcs 字符集

			获取所有 std::codecvt 支持的转换类型
	 */
	FC_INLINE Std_Charset::Std_Charset() noexcept
		:
			locale( "" ),	// os local

			codecvt_u32_u8( std::use_facet< Codecvt_u32_u8 >( locale ) ),

			codecvt_u16_u8( std::use_facet< Codecvt_u16_u8 >( locale ) ),

			codecvt_wcs_mbcs( std::use_facet< Codecvt_wcs_mbcs >( locale ) )
		{
		}


	/*
	 @	释放
	 */
	FC_INLINE Void Std_Charset::Release() noexcept
		{
		}


	/*
	 @	初始化
	 */
	FC_INLINE RESULT Std_Charset::Initialize() noexcept
		{
			return RESULT::SUCCESS;
		}
}