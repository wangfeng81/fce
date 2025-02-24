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
	 @	构造
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
	 @	移动构造
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

			处理 run 作业时的 异常

			对于 create & destroy 的异常另外直接 退出
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
	 @	格式化 信息
	 */
	std::wstring Assertion::Format() const
		{
			// 格式化 字符串
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

						// 不应有 未处理的类型
						default: std::unreachable();
					}

				this->impl.Console( str );
			}


		/*
		 @	断言
		 */
		Bool Debug::Assert( const Assertion& a ) const
			{
				// 格式化 消息
				std::wstring msg( a.Format() );

				// 发往控制台
				this->Console( fce::Debug::TYPE::ERR, msg );

				// 弹出 错误框
				if( s_Window.ErrorBox( msg ) )
					{
						// 点击“ok”，并处于调试模式
						if( this->isDebugging() )
							{
								return true;
							}
					}
				else
					{
						// 点“no” 直接退出
						std::quick_exit( -1 );
					}

				return false;
			}

	#endif
}