/*
**
**	Debug.
**
		Copyright (c) WangFeng since 2000...
*/

#include "../../include/platform/Application.h"


/**
 !	Debug kit
 */
namespace fce
{
	/*
	 @	����
	 */
	Assertion::Assertion( const Char* const expression,
						  const std::wstring& tips,
						  const std::source_location& sl )
		:
			line( sl.line() ),
			column( sl.column() ),

			file( sl.file_name() ),
			func( sl.function_name() ),

			expression( expression ),
			tips( tips )
		{
		}


	/*
	 @	�ƶ�����
	 */
	Assertion::Assertion( Assertion&& a ) noexcept
		:
			line( a.line ),
			column( a.column ),

			file( std::move( a.file ) ),
			func( std::move( a.func ) ),

			tips( std::move( a.tips ) ),
			expression( std::move( a.expression ) )
		{
		}


	/*
	 @	On exception

			���� run ��ҵʱ�� �쳣

			���� create & destroy ���쳣����ֱ�� �˳�
	 */
	Void Assertion::on_Exception() noexcept
		{
			s_App.on_Exception();
		}


	/*
	 @	std::exception::what
	 */
	const char* Assertion::what() const noexcept
		{
			return "fce assertion";
		}


	/*
	 @	��ʽ�� ��Ϣ
	 */
	std::wstring Assertion::Format() const
		{
			// ��ʽ�� �ַ���
			const WChar* const fmt
				{
					L"%s\n\n__FILE__:   %s\n__LINE__:   %d\n__FUNC__:   %s\n__ASSERT__:   %s"
				};

			return fce::Format( fmt, tips,
								Path( this->file ).filename().native(),
								this->line, this->func, this->expression );
		}
}



/**
 !	Debug
 */
namespace fce
{
	#ifdef FC_DEBUG

		/*
		 @	Console
		 */
		Void Debug::Console( const TYPE type,
							 const std::wstring& msg ) const
			{
				std::wstring str;

				switch( type )
					{
						case TYPE::MSG:
							{
								str = msg;
							}
							break;

						case TYPE::ERR:
							{
								str = L"ERROR: " + msg;
							}
							break;

						case TYPE::WARN:
							{
								str = L"WARNING: " + msg;
							}
							break;

						// ��Ӧ�� δ���������
						default: std::unreachable();
					}

				this->impl.Console( str );
			}


		/*
		 @	����
		 */
		Bool Debug::Assert( const Assertion& a ) const
			{
				// ��ʽ�� ��Ϣ
				std::wstring msg( a.Format() );

				// ��������̨
				this->Console( fce::Debug::TYPE::ERR, msg );

				// ���� �����
				if( s_Window.ErrorBox( msg ) )
					{
						// �����ok���������ڵ���ģʽ
						if( this->isDebugging() )
							{
								return true;
							}
					}
				else
					{
						// �㡰no�� ֱ���˳�
						std::quick_exit( -1 );
					}

				return false;
			}

	#endif
}