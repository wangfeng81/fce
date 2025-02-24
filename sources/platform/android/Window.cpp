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
		 !	创建 OPENGL 窗口
		 @ anw	窗口
		*/
		EGLBoolean CWindow::Create( ANativeWindow* anw )
		{
			// 获取 EGLDisplay
			if( EGL_NO_DISPLAY ==
					( this->display =
						eglGetDisplay( EGL_DEFAULT_DISPLAY ) ) )
				return EGL_FALSE;

			// 初始化 EGLDisplay
			if( ! eglInitialize( this->display, nullptr, nullptr ) )
				return EGL_FALSE;

			// 设置 EGL Config 属性，需要尝试匹配
			const EGLint attribs[] = {
					// 设置 es2
					EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
					//EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
					//EGL_RED_SIZE, 8,
					//EGL_GREEN_SIZE, 8,
					//EGL_BLUE_SIZE, 8,

					EGL_NONE
			};

			// 选择 EGL 配置
			EGLint numConfigs;
			EGLConfig config;

			if( ! eglChooseConfig( this->display,
								attribs, &config, 1, &numConfigs ) )
				return EGL_FALSE;

			// 获取 EGL 配置属性
			EGLint format;
			if( ! eglGetConfigAttrib( this->display,
									config, EGL_NATIVE_VISUAL_ID, &format ) )
				return EGL_FALSE;

			// 设置 buff 格式
			ANativeWindow_setBuffersGeometry( anw, 0, 0, format );

			// 创建 EGLSurface
			if( EGL_NO_SURFACE ==
					( this->surface =
						eglCreateWindowSurface( this->display,
												config, anw, nullptr ) ) )
				return EGL_FALSE;

			// 创建 EGLContext
			const EGLint contextList[] = {
 				EGL_CONTEXT_CLIENT_VERSION, 2,
 				EGL_NONE
 			};

			if( EGL_NO_CONTEXT ==
					( this->context =
						eglCreateContext( this->display, config,
								EGL_NO_CONTEXT, contextList ) ) )
				return EGL_FALSE;

			// 选择当前 EGLDisplay 的 EGLSurface 和 EGLContext
			if( ! eglMakeCurrent( this->display,
								this->surface, this->surface, this->context ) )
				return EGL_FALSE;

			// 获取 宽高
			EGLint width, height;
			if( ! eglQuerySurface( this->display, this->surface,
								EGL_WIDTH, &width ) )
				return EGL_FALSE;

			if( ! eglQuerySurface( this->display, this->surface,
								EGL_HEIGHT, &height ) )
				return EGL_FALSE;

			this->size.width = width;
			this->size.height = height;

			// 获取一个窗口引用
			this->window = anw;
			//ANativeWindow_acquire( anw );

			// 更新 窗口尺寸
			this->onResize( this->size );

			// 关闭深度测试
			glDisable( GL_DEPTH_TEST );

			return EGL_TRUE;
		}


		/*
		 !	Release
		*/
		EGLBoolean CWindow::Destroy()
		{
			EGLBoolean res = EGL_TRUE;

			// 销毁 纹理
			SHARED( CTexture2DCache ).Clear();
			SHARED( CFontCache ).Clear();

			// 销毁 shader
			SHARED( gl::CShaderCache ).Clear();

			if( this->display != EGL_NO_DISPLAY )
			{
				// 取消当前的 egl选择
				res &= eglMakeCurrent( this->display,
							EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );

				// 销毁 egl Context
				if( this->context != EGL_NO_CONTEXT )
				{
					res &= eglDestroyContext(
								this->display, this->context );
					this->context = EGL_NO_CONTEXT;
				}

				// 销毁 egl Surface
				if (this->surface != EGL_NO_SURFACE )
				{
					res &= eglDestroySurface(
								this->display, this->surface );
					this->surface = EGL_NO_SURFACE;
				}

				// 终止 egl Display
				res &= eglTerminate( this->display );
				this->display = EGL_NO_DISPLAY;

				// 释放 一个窗口引用
				//ANativeWindow_release( this->window );
			}

			return res;
		}


		/*
		 !	Present
		*/
		void CWindow::Present()
		{
			//强制OpenGL函数在有限时间内运行
			glFlush();

			// 交换 缓冲页面
			eglSwapBuffers( this->display, this->surface );

			// 清空 buff
			glClear( GL_COLOR_BUFFER_BIT );
		}


		/*
		 !	OnResize
		*/
		void CWindow::onResize( const Size& size )
		{
			this->size = size;

			// 更新 CGraphics
			SHARED( gl::CGraphics ).OnResize( size.width, size.height );

			// 更新 GUI
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