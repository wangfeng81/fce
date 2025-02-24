/*
**
**	Engine
**
		待解问题：考虑将所有 inline 和 cpp 文件 include 到一个 cpp 文件，便于 私有内联函数

		Copyright (c) WangFeng since 1999...
*/

#include "../include/fce.h"


namespace fce
{
	/*
	 !	Engine
	 */
	class Engine final
		{
		public:
			/*	Instance
			*/
			template< typename T >

			static T instance;

		public:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Engine );

			~Engine() noexcept;

			Engine() noexcept;

			/*	On terminate
			*/
			static void on_Terminate() noexcept;
		}

	/*
	 @	Engine singleton
	 */
	static s_Engine;

	/*
	 @	Instances
	 */
	template< typename T >

	T Engine::instance;


	/*	Common			shortcut
	 */
	XImage&				s_XImage		= Engine::instance< XImage >;
	XModel3D&			s_XModel3D		= Engine::instance< XModel3D >;
	Director&			s_Director		= Engine::instance< Director >;

	/*	Platform		shortcut
	 */
	Application&		s_App			= Engine::instance< Application >;
	Clipboard&			s_Clipboard		= Engine::instance< Clipboard >;
	Graphics&			s_Graphics		= Engine::instance< Graphics >;
	Window&				s_Window		= Engine::instance< Window >;

	/*	Input			shortcut
	 */
	Ime&				s_Ime			= Engine::instance< Window >.ime;
	Mouse&				s_Mouse			= Engine::instance< Window >.mouse;
	Keyboard&			s_Keyboard		= Engine::instance< Window >.keyboard;

	/*	Management		shortcut
	 */
	AssetManagement&	s_AssetMgmt		= Engine::instance< AssetManagement >;
	TimerManagement&	s_TimerMgmt		= Engine::instance< TimerManagement >;
	StorageManagement&	s_StorageMgmt	= Engine::instance< StorageManagement >;
	MaterialManagement&	s_MaterialMgmt	= Engine::instance< MaterialManagement >;

	/*	SceneGroup		shortcut
	 */
	SceneGroup&			s_SceneGroup	= Engine::instance< Window >.sceneGroup;
}



/**
 !	class Engine
 */
namespace fce
{
	/*
	 @	析构
	 */
	Engine::~Engine() noexcept
		{
		}

	/*
	 @	构造
	 */
	Engine::Engine() noexcept
		{
			// 设置 Terminate
			#ifdef FC_NDEBUG

				std::set_terminate( on_Terminate );

			#endif
	}

	/*
	 @	On terminate

			用于异常堆栈等信息提取
	 */
	Void Engine::on_Terminate() noexcept
		{
			// 未处理的异常
			s_Window.PromptBox( "\n An unhandled exception caused the program to terminate..." );

			std::quick_exit( -1 );
		}
}



/**
 !	class Framework
 */
namespace fce
{
	/*
	 @	Framework constructor
	 */
	Framework::Framework( Application& ) noexcept
		{
		}

	/*
	 @	Framework destructor
	 */
	Framework::~Framework() noexcept
		{
			// 禁止 非法删除 Framework
			#ifdef FC_DEBUG

				if( s_App.framework && s_App.framework == this )
					{
						FC_ASSERT_LOG( false,
									   L"Destroying the framework component will cause the program to exit..." );

						std::quick_exit( -1 );
					}

			#endif
		}

	/*
	 @	设置为 依赖自己，组件一旦添加 则禁止移除
	 */
	Bool Framework::DependentOn( const Component* const cmpt ) const noexcept
		{
			return this == cmpt;
		}
}