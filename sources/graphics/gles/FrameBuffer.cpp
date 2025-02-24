/*
**
**	FrameBuffer.
**
	���� ��� ģ�� ������ȾĿ��
	�� alpha ���ǰһ֡ ��������
		Copyright (c) WangFeng since 2006...
*/

#include "../../../include/graphics/FrameBuffer.h"


namespace fce
{
	Renderbuffer::Renderbuffer( TYPE type, Int16 width, Int16 height )
		{
			glGenRenderbuffers( 1, &this->id );

			glBindRenderbuffer( GL_RENDERBUFFER, this->id );

			glRenderbufferStorage( GL_RENDERBUFFER,
								   type, width, height );

			// ������ ������
			glBindRenderbuffer( GL_RENDERBUFFER, 0 );
		}

	
	
	
	//void Editor::Framebuf(  )
	//{
	//	// ����
	//	this->frameBuffer = s_FrameBufferCache.Add("");
	//	this->tex = s_Texture2Mgmt.Add( "frame", Image::Info{ 512,512,COLOR_4 } );
	//	Renderbuffer* depthBuffer = s_RenderbufferCache.Add( "", Renderbuffer::DEPTH, 512, 512 );

	//	// ��
	//	this->frameBuffer->Begin();
	//	this->frameBuffer->Attach( this->tex );
	//	this->frameBuffer->Attach( depthBuffer );
	//	this->frameBuffer->End();

	//	// ���� ��ά��Ƭ
	//	Vertices::Describe vertexDescribe2( 2 );
	//	vertexDescribe2[0].Set( SLOT_POSITION, TYPE_FLOAT32, 3 );
	//	vertexDescribe2[1].Set( SLOT_TEXCOORD, TYPE_FLOAT32, 2 );
	//	vertexDescribe2.Update();

	//	Vertices vertices2( vertexDescribe2, 4, PRIMITIVE::TRIANGLE_FAN );

	//	struct Vertex2{ Vector3 position; Vector2 v2; };
	//	static const Vertex2 vertex2[] = {
	//		Vector3( 0.0f, 0.0f, 500.0f ), Vector2( 0.0f, 0.0f ),
	//		Vector3( 0.0f, 500.0f, 500.0f ),  Vector2( 0.0f, 1.0f ),
	//		Vector3( 500.0f, 500.0f, 500.0f ), Vector2( 1.0f, 1.0f ),
	//		Vector3( 500.0f, 0.0f, 500.0f ),  Vector2( 1.0f, 0.0f ),
	//		};

	//	memcpy( *vertices2, vertex2, vertices2.GetSize() );

	//	VertexBuffer *vb3 = s_VertexBufferCache.Add( "ff", vertexDescribe2, vertices2 );

	//	this->meshFrame = s_MeshCache.Add( "fra",
	//										s_Application.window.graphics.shaderProgramCache.Request( ShaderProgram::ID( VS_TEXTURE2, PS_TEXTURE2 ) ),
	//										vb3, nullptr, this->tex);

	//}




	//void Editor::RunFramebuf()
	//{
	//	// ��ʼ
	//	this->frameBuffer->Begin();

	//	glViewport( 0, 0, 512, 512 );

	//	// ��Ⱦ������
	//	s_Graphics.SetClearColor( Colors::BLUE );
	//	s_Graphics.Clear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );


	//	// �ָ�
	//	glViewport( 0, 0, 1024, 768 );
	//	s_Graphics.SetClearColor( Colors::GRAY );


	//	///////////
	//	glDisable( GL_DEPTH_TEST );
	//	// ���Ҫ�� ��Ⱦ������ ���ڶ��� ��ر� ��Ȼ���
	//	glEnable( GL_DEPTH_TEST );
	//}
}