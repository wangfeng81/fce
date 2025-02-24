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

			// 创建一个 shader
			this->id = glCreateShader( type );

			// 检测 id 为0 则创建错误
			FC_ASSERT( this->id );

			// 编译 code
			glShaderSource( this->id, 1,
							&code, nullptr );

			glCompileShader( this->id );

			// 检测 结果
			#ifdef FC_DEBUG
			{
				// 获取状态 是否正常
				GLint status;
				glGetShaderiv( this->id,
							   GL_COMPILE_STATUS,
							   &status );

				if( GL_TRUE != status )
					{
						// 获取日志信息的长度
						GLint infoLen = 0;
						glGetShaderiv( this->id,
									   GL_INFO_LOG_LENGTH,
									   &infoLen );

						if( infoLen )
							{
								// 获取日志信息
								Buffer buf( infoLen );
								glGetShaderInfoLog( this->id, infoLen, 0,
													buf.Ptr< GLchar >() );
							}

						// 根据不同 shader类型 发出警告
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
			// 绑定slot的名字 在shader内的变量 需要在 link 之前
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

			// 创建一个 program
			this->id = glCreateProgram();

			// 检测 id 为0 则创建错误
			FC_ASSERT( this->id );

			// 关联 vs ps
			vertexShader->Attach( this->id );
			pixelShader->Attach( this->id );

			// 绑定slot的名字 在shader内的变量 需要在 link 之前
			vertexShader->BindSlots( this->id );

			// 链接
			glLinkProgram( this->id );

			// 检测 结果
			#ifdef FC_DEBUG
			{
				// 获取 链接状态
				GLint status;
				glGetProgramiv( this->id,
								GL_LINK_STATUS,
								&status );

				if( GL_TRUE != status )
					{
						// 获取日志信息长度
						GLint infoLen = 0;
						glGetProgramiv( this->id,
										GL_INFO_LOG_LENGTH,
										&infoLen );

						if( infoLen )
							{
								// 获取日志信息
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