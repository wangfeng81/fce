/*
**
**	ES_Graphics.
**
		Gles implementation

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 @	Create
	 */
	Bool ES_Graphics::Create()
		{
			#ifdef FC_WINDOWS

				// 获取窗口 DC
				if( ! ( this->hdc = GetDC( s_Win_Window ) ) )
					return false;

				// 设置像素格式
				PIXELFORMATDESCRIPTOR pfd;
					{   
						ZeroMemory( &pfd, sizeof(pfd) );
						pfd.nSize = sizeof(pfd); 

						pfd.nVersion	= 1;
						pfd.cColorBits	= 32;
						pfd.cDepthBits	= 16;
						pfd.iPixelType	= PFD_TYPE_RGBA;
						pfd.iLayerType	= PFD_MAIN_PLANE;
						pfd.dwFlags		= PFD_SUPPORT_OPENGL
											| PFD_DRAW_TO_WINDOW
											| PFD_DOUBLEBUFFER;
					};

				int pixelFormat = ChoosePixelFormat( this->hdc, &pfd );

				if( ! ( SetPixelFormat( this->hdc, pixelFormat, &pfd ) ) )
					return false;

				// 创建 gl上下文
				if( ! ( this->hrc = wglCreateContext( this->hdc ) ) )
					return false;

				// 选择当前 DC 上下文
				if( ! wglMakeCurrent( this->hdc, this->hrc ) )
					return false;

				// 初始化 gl
				if( GLEW_OK != glewInit() )
					return false;

			#endif

			// 设置纹理环境参数
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
					 
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

			// 剔除背面
			glEnable( GL_CULL_FACE );
			glCullFace( GL_BACK );

			// 深度测试
			glEnable( GL_DEPTH_TEST );
			//glEnable( GL_STENCIL_TEST );
			//抗锯齿
			//glEnable( GL_MULTISAMPLE );
			//glDepthFunc( GL_LEQUAL );

			glEnable( GL_BLEND );

			// 打开 帧缓冲
			glEnable( GL_FRAMEBUFFER );
			glEnable( GL_RENDERBUFFER );

			// 需要支持 glsl 以及 gl 2.0
			return GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader
					&& glewIsSupported("GL_VERSION_3_2");
		}


	/*
	 @	Destroy
	 */
	Void ES_Graphics::Destroy()
		{
			#ifdef FC_WINDOWS

				// 销毁 wgl
				if( this->hdc )
					{
						wglMakeCurrent( this->hdc, NULL );

						ReleaseDC( s_Win_Window,
								   this->hdc );
					}

				// 销毁 gl上下文
				if( this->hrc )
					{
						wglDeleteContext( this->hrc );
					}

			#endif
		}


	/*
	 @	Screenshot
	 */
	Void ES_Graphics::Screenshot( Image& img,
								  Int16 x,
								  Int16 y,
								  Int16 width,
								  Int16 height ) const
		{
			img.Set( width, height, COLOR::_4 );

			glReadPixels( x, y,
						  width,
						  height,
						  GL_RGBA,
						  GL_UNSIGNED_BYTE,
						 *img );
		}
}