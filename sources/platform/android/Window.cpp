/*
**
**	CWindow.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#include "Window.h"

#include "../../graphics/opengl/ShaderCache.h"
#include "../../graphics/opengl/Graphics.h"
#include "../../graphics/Texture2DCache.h"
#include "../../graphics/FontCache.h"

#include "../../gui/Gui.h"


namespace fce
{
	/*
	 !	Window.
	 */
	IWindow& Window = SHARED( android::CWindow );


	namespace android
	{
		/*
		!	Constructor.
		*/
		CWindow::CWindow()
		{
			this->display = EGL_NO_DISPLAY;
		}
	
		/*
		!	Destructor.
		*/
		CWindow::~CWindow()
		{
		}


		/*
		 !	���� OPENGL ����
		 @ anw	����
		*/
		EGLBoolean CWindow::Create( ANativeWindow* anw )
		{
			// ��ȡ EGLDisplay
			if( EGL_NO_DISPLAY ==
					( this->display =
						eglGetDisplay( EGL_DEFAULT_DISPLAY ) ) )
				return EGL_FALSE;

			// ��ʼ�� EGLDisplay
			if( ! eglInitialize( this->display, nullptr, nullptr ) )
				return EGL_FALSE;

			// ���� EGL Config ���ԣ���Ҫ����ƥ��
			const EGLint attribs[] = {
					// ���� es2
					EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
					//EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
					//EGL_RED_SIZE, 8,
					//EGL_GREEN_SIZE, 8,
					//EGL_BLUE_SIZE, 8,

					EGL_NONE
			};

			// ѡ�� EGL ����
			EGLint numConfigs;
			EGLConfig config;

			if( ! eglChooseConfig( this->display,
								attribs, &config, 1, &numConfigs ) )
				return EGL_FALSE;

			// ��ȡ EGL ��������
			EGLint format;
			if( ! eglGetConfigAttrib( this->display,
									config, EGL_NATIVE_VISUAL_ID, &format ) )
				return EGL_FALSE;

			// ���� buff ��ʽ
			ANativeWindow_setBuffersGeometry( anw, 0, 0, format );

			// ���� EGLSurface
			if( EGL_NO_SURFACE ==
					( this->surface =
						eglCreateWindowSurface( this->display,
												config, anw, nullptr ) ) )
				return EGL_FALSE;

			// ���� EGLContext
			const EGLint contextList[] = {
 				EGL_CONTEXT_CLIENT_VERSION, 2,
 				EGL_NONE
 			};

			if( EGL_NO_CONTEXT ==
					( this->context =
						eglCreateContext( this->display, config,
								EGL_NO_CONTEXT, contextList ) ) )
				return EGL_FALSE;

			// ѡ��ǰ EGLDisplay �� EGLSurface �� EGLContext
			if( ! eglMakeCurrent( this->display,
								this->surface, this->surface, this->context ) )
				return EGL_FALSE;

			// ��ȡ ���
			EGLint width, height;
			if( ! eglQuerySurface( this->display, this->surface,
								EGL_WIDTH, &width ) )
				return EGL_FALSE;

			if( ! eglQuerySurface( this->display, this->surface,
								EGL_HEIGHT, &height ) )
				return EGL_FALSE;

			this->size.width = width;
			this->size.height = height;

			// ��ȡһ����������
			this->window = anw;
			//ANativeWindow_acquire( anw );

			// ���� ���ڳߴ�
			this->onResize( this->size );

			// �ر���Ȳ���
			glDisable( GL_DEPTH_TEST );

			return EGL_TRUE;
		}


		/*
		 !	Release
		*/
		EGLBoolean CWindow::Destroy()
		{
			EGLBoolean res = EGL_TRUE;

			// ���� ����
			SHARED( CTexture2DCache ).Clear();
			SHARED( CFontCache ).Clear();

			// ���� shader
			SHARED( gl::CShaderCache ).Clear();

			if( this->display != EGL_NO_DISPLAY )
			{
				// ȡ����ǰ�� eglѡ��
				res &= eglMakeCurrent( this->display,
							EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );

				// ���� egl Context
				if( this->context != EGL_NO_CONTEXT )
				{
					res &= eglDestroyContext(
								this->display, this->context );
					this->context = EGL_NO_CONTEXT;
				}

				// ���� egl Surface
				if (this->surface != EGL_NO_SURFACE )
				{
					res &= eglDestroySurface(
								this->display, this->surface );
					this->surface = EGL_NO_SURFACE;
				}

				// ��ֹ egl Display
				res &= eglTerminate( this->display );
				this->display = EGL_NO_DISPLAY;

				// �ͷ� һ����������
				//ANativeWindow_release( this->window );
			}

			return res;
		}


		/*
		 !	Present
		*/
		void CWindow::Present()
		{
			//ǿ��OpenGL����������ʱ��������
			glFlush();

			// ���� ����ҳ��
			eglSwapBuffers( this->display, this->surface );

			// ��� buff
			glClear( GL_COLOR_BUFFER_BIT );
		}


		/*
		 !	OnResize
		*/
		void CWindow::onResize( const Size& size )
		{
			this->size = size;

			// ���� CGraphics
			SHARED( gl::CGraphics ).OnResize( size.width, size.height );

			// ���� GUI
			SHARED( gui::CGui ).OnResize( size );
		}


		/*
		 !	GetPos
		*/
		IWindow::Point CWindow::GetPos() const
		{
			return Point( 0, 0 );
		}


		/*
		 !	GetSize
		*/
		IWindow::Size CWindow::GetSize() const
		{
			return this->size;
		}


		/*
		 !	GetRect
		*/
		IWindow::Rect CWindow::GetRect() const
		{
			return Rect( Point( 0, 0 ),
						this->size );
		}


		/*
		 !	SetPos
		*/
		Bool CWindow::SetPos( const Point& pt )
		{
			return False;
		}


		/*
		 !	SetSize
		*/
		Bool CWindow::SetSize( const Size& sz )
		{
			return False;
		}


		/*
		 !	SetRect
		*/
		Bool CWindow::SetRect( const Rect& rc )
		{
			return False;
		}
	}
}