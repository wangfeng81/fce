/*
**
**	Engine
**
		�������⣺���ǽ����� inline �� cpp �ļ� include ��һ�� cpp �ļ������� ˽����������

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
	 @	����
	 */
	Engine::~Engine() noexcept
		{
		}

	/*
	 @	����
	 */
	Engine::Engine() noexcept
		{
			// ���� Terminate
			#ifdef FC_NDEBUG

				std::set_terminate( on_Terminate );

			#endif
	}

	/*
	 @	On terminate

			�����쳣��ջ����Ϣ��ȡ
	 */
	Void Engine::on_Terminate() noexcept
		{
			// δ������쳣
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
			// ��ֹ �Ƿ�ɾ�� Framework
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
	 @	����Ϊ �����Լ������һ����� ���ֹ�Ƴ�
	 */
	Bool Framework::DependentOn( const Component* const cmpt ) const noexcept
		{
			return this == cmpt;
		}
}