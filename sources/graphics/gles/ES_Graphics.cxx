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

				// ��ȡ���� DC
				if( ! ( this->hdc = GetDC( s_Win_Window ) ) )
					return false;

				// �������ظ�ʽ
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

				// ���� gl������
				if( ! ( this->hrc = wglCreateContext( this->hdc ) ) )
					return false;

				// ѡ��ǰ DC ������
				if( ! wglMakeCurrent( this->hdc, this->hrc ) )
					return false;

				// ��ʼ�� gl
				if( GLEW_OK != glewInit() )
					return false;

			#endif

			// ��������������
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
					 
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

			// �޳�����
			glEnable( GL_CULL_FACE );
			glCullFace( GL_BACK );

			// ��Ȳ���
			glEnable( GL_DEPTH_TEST );
			//glEnable( GL_STENCIL_TEST );
			//�����
			//glEnable( GL_MULTISAMPLE );
			//glDepthFunc( GL_LEQUAL );

			glEnable( GL_BLEND );

			// �� ֡����
			glEnable( GL_FRAMEBUFFER );
			glEnable( GL_RENDERBUFFER );

			// ��Ҫ֧�� glsl �Լ� gl 2.0
			return GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader
					&& glewIsSupported("GL_VERSION_3_2");
		}


	/*
	 @	Destroy
	 */
	Void ES_Graphics::Destroy()
		{
			#ifdef FC_WINDOWS

				// ���� wgl
				if( this->hdc )
					{
						wglMakeCurrent( this->hdc, NULL );

						ReleaseDC( s_Win_Window,
								   this->hdc );
					}

				// ���� gl������
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