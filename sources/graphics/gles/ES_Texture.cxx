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
			// ת�������ʽ
			GLenum glfmt = Texture2::ConvertColor( info.color );

			// ��������
			glGenTextures( 1, &this->id );

			// �� ����
			glBindTexture( GL_TEXTURE_2D, this->id );

			// ��������������
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

			// �����ֽ�
			glPixelStorei( GL_UNPACK_ALIGNMENT,
						   info.color == COLOR::_4
							? 4 : 1 );

			// ��������
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
			// ת�������ʽ
			GLenum glfmt = Texture2::ConvertColor( img.Color() );

			// �� ����
			glBindTexture( GL_TEXTURE_2D,
						   this->id );

			// �����ֽ�
			glPixelStorei( GL_UNPACK_ALIGNMENT,
						   img.Color() == COLOR::_4
							? 4 : 1 );

			// ��������
			glTexSubImage2D( GL_TEXTURE_2D, 0,
							 x, y,
							 img.Width(),
							 img.Height(),
							 glfmt, GL_UNSIGNED_BYTE,
							*img );

			// ������
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
			// ���õ�ǰ������ ��������Ԫ 0��32
			glActiveTexture( GL_TEXTURE0 + c );

			// ������ID�� ��ǰ�� ��������Ԫ
			this->vecTexture2[ c ]->Use();

			// ��ȡ��ǰprogram���� ������ ���  u_sampler0ΪGLSL�� ����������
			// ���� ������ һ�±���  �ڶ���������Ӧ  ����Ԫ 0��32
			//glUniform1i( glGetUniformLocation( currProgram, "u_sampler0" ), c );
		}
	}
}