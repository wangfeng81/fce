/*
**
**	ES_Shader.
**
		Copyright (c) WangFeng since 2007...
*/

namespace fce
{
	/*
	 @	Constructor
	 */
	ES_Shader::ES_Shader( GLenum type, const GLchar* code )
		{
			FC_ASSERT( code );

			// ����һ�� shader
			this->id = glCreateShader( type );

			// ��� id Ϊ0 �򴴽�����
			FC_ASSERT( this->id );

			// ���� code
			glShaderSource( this->id, 1,
							&code, nullptr );

			glCompileShader( this->id );

			// ��� ���
			#ifdef FC_DEBUG
			{
				// ��ȡ״̬ �Ƿ�����
				GLint status;
				glGetShaderiv( this->id,
							   GL_COMPILE_STATUS,
							   &status );

				if( GL_TRUE != status )
					{
						// ��ȡ��־��Ϣ�ĳ���
						GLint infoLen = 0;
						glGetShaderiv( this->id,
									   GL_INFO_LOG_LENGTH,
									   &infoLen );

						if( infoLen )
							{
								// ��ȡ��־��Ϣ
								Buffer buf( infoLen );
								glGetShaderInfoLog( this->id, infoLen, 0,
													buf.Ptr< GLchar >() );
							}

						// ���ݲ�ͬ shader���� ��������
						type == GL_VERTEX_SHADER
						? FC_ASSERT( !"Vertex shader compile error!" )
						: FC_ASSERT( !"Pixel shader compile error!" );
					}
			}
			#endif
		}
}



namespace fce
{
	/*
	 @	BindSlots
	 */
	GLvoid ES_VertexShader::BindSlots( GLuint program )
		{
			// ��slot������ ��shader�ڵı��� ��Ҫ�� link ֮ǰ
			for( GLuint s = 0; s < MAXIMUM_SLOT; ++s )
				if( this->HasSlot( s ) )
					glBindAttribLocation( program, s,
										  VertexShader::slotNames[ s ] );
		}
}



namespace fce
{
	GLuint ES_ShaderProgram::currProgram = 0;

	/*
	 @	Constructor.
	 */
	ES_ShaderProgram::ES_ShaderProgram( ES_VertexShader* vertexShader,
										ES_PixelShader* pixelShader )
		: vertexShader( vertexShader )
		{
			FC_ASSERT( vertexShader );
			FC_ASSERT( pixelShader );

			// ����һ�� program
			this->id = glCreateProgram();

			// ��� id Ϊ0 �򴴽�����
			FC_ASSERT( this->id );

			// ���� vs ps
			vertexShader->Attach( this->id );
			pixelShader->Attach( this->id );

			// ��slot������ ��shader�ڵı��� ��Ҫ�� link ֮ǰ
			vertexShader->BindSlots( this->id );

			// ����
			glLinkProgram( this->id );

			// ��� ���
			#ifdef FC_DEBUG
			{
				// ��ȡ ����״̬
				GLint status;
				glGetProgramiv( this->id,
								GL_LINK_STATUS,
								&status );

				if( GL_TRUE != status )
					{
						// ��ȡ��־��Ϣ����
						GLint infoLen = 0;
						glGetProgramiv( this->id,
										GL_INFO_LOG_LENGTH,
										&infoLen );

						if( infoLen )
							{
								// ��ȡ��־��Ϣ
								Buffer buf( infoLen );
								glGetShaderInfoLog( this->id, infoLen, 0,
													buf.Ptr< GLchar >() );

								FC_ASSERT_LOG( false, "Shader program link error:\n" );
							}
					}
			}
			#endif
		}
}