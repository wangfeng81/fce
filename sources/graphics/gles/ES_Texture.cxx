/*
**
**	ES_Texture.
**
		Gles implementation

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 @	Set.
	 */
	Void ES_Texture2::Set( const Info& info,
						   const Void* pixels )
		{
			// 转换纹理格式
			GLenum glfmt = Texture2::ConvertColor( info.color );

			// 创建纹理
			glGenTextures( 1, &this->id );

			// 绑定 纹理
			glBindTexture( GL_TEXTURE_2D, this->id );

			// 设置纹理环境参数
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

			// 对齐字节
			glPixelStorei( GL_UNPACK_ALIGNMENT,
						   info.color == COLOR::_4
							? 4 : 1 );

			// 加载纹理
			glTexImage2D( GL_TEXTURE_2D, 0, glfmt,
						  info.width, info.height, 0,
						  glfmt, GL_UNSIGNED_BYTE, pixels );
		}


	/*
	 @	Modify
	 */
	Void ES_Texture2::Modify( Int16 x, Int16 y,
							  const Image& img )
		{
			// 转换纹理格式
			GLenum glfmt = Texture2::ConvertColor( img.Color() );

			// 绑定 纹理
			glBindTexture( GL_TEXTURE_2D,
						   this->id );

			// 对齐字节
			glPixelStorei( GL_UNPACK_ALIGNMENT,
						   img.Color() == COLOR::_4
							? 4 : 1 );

			// 加载纹理
			glTexSubImage2D( GL_TEXTURE_2D, 0,
							 x, y,
							 img.Width(),
							 img.Height(),
							 glfmt, GL_UNSIGNED_BYTE,
							*img );

			// 错误检测
			if( GL_NO_ERROR != glGetError() )
				{
				}
		}
}



namespace fce
{
	/*
	 @	ES_MultiTexture::Use
	 */
	Void ES_MultiTexture::Use() const
	{
		//GLuint currProgram = ShaderCache::instance.currProgram;

		for( Byte c = 0; c < vecTexture2.size(); ++c )
		{
			// 设置当前操作的 多重纹理单元 0到32
			glActiveTexture( GL_TEXTURE0 + c );

			// 绑定纹理ID到 当前的 多重纹理单元
			this->vecTexture2[ c ]->Use();

			// 获取当前program里面 采样器 编号  u_sampler0为GLSL内 采样器名字
			// 设置 采样器 一致变量  第二个参数对应  纹理单元 0到32
			//glUniform1i( glGetUniformLocation( currProgram, "u_sampler0" ), c );
		}
	}
}