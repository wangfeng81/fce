/*
**
**	Std_Charset.
**
		Std implementation

			std::codecvt ֱ��֧�ֵ�ת��

				utf32 <=> utf8
				utf16 <=> utf8
				wcs	  <=> mbcs

			�Զ���� ���ת��

				utf32 <=> utf16		by utf8
				mbcs  <=> unicode	by wcs
				wcs   <=> unicode	by utf16 or 32 ( Mapping_wchar )

	Copyright (c) WangFeng since 2012...
*/

namespace fce::inner::charset
{
	/*
	 @	����
	 
			locale ����մ�����ȡ���� mbcs �ַ���

			��ȡ���� std::codecvt ֧�ֵ�ת������
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
	 @	�ͷ�
	 */
	FC_INLINE Void Std_Charset::Release() noexcept
		{
		}


	/*
	 @	��ʼ��
	 */
	FC_INLINE RESULT Std_Charset::Initialize() noexcept
		{
			return RESULT::SUCCESS;
		}
}