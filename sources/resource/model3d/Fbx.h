/*
**
**	Fbx.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 !	Fbx
	 */
	class FC_API Fbx final
		{
			/*	Friends
			*/
			friend XModel3D;

			/*	Vertex

					当 Float 改 64位 后
					可能模型继续用 32 位

					FbxVectorTemplate3< FbxFloat >
			*/
			struct FC_API Vertex
				{
					Vector3 pos;
					Vector2 uv;
					Vector3 normal;
					//Vector3 tangent;
					//Vector3 binormal;
				};

		public:
			/*	Singleton instance
			*/
			static Fbx instance;

			/*	Find ID
			*/
			int FindReaderID() const noexcept;

			int FindWriterID() const noexcept;

			/*	Decode fbx
			*/
			bool Decode( const Buffer&,
						 Scene::Node* const ) NOEXCEPT;

			/*	Convert vector
			*/
			template< class Src,
					  class Dst >

			static void Convert( const Src&, Dst& ) noexcept;

			/*	Triangulate
			*/
			FbxMesh* Triangulate( const FbxNodeAttribute* const ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Fbx )

			~Fbx() noexcept;

			Fbx() noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			bool Initialize() noexcept;

		protected:
			/*	Parse fbx
			*/
			static bool Parse( const FbxScene* const,
									 Scene::Node* const ) noexcept;

			static bool Parse( const FbxNode* const,
									 Scene::Node* const ) noexcept;

			/*	Parse components
			*/
			static void ParseName( const FbxNode* const,
										 Scene::Node* const ) noexcept;

			static bool ParseMesh( const FbxMesh* const,
										 Scene::Node* const ) noexcept;

			static void ParseTransform( const FbxNode* const,
											  Scene::Node* const ) noexcept;

			static bool ParseAttributes( const FbxNode* const,
											   Scene::Node* const ) noexcept;

		protected:
			/*	Get mesh ( triangle list vertices )
			*/
			static bool GetMesh( const FbxMesh* const,
								 VertexSet& ) noexcept;

			/*	Get control points ( index & vertex position )
			*/
			//static bool GetMesh( const FbxMesh* const,
			//						   IndexSet&,
			//						   VertexSet& ) noexcept;

			static bool GetIndices( const FbxMesh* const,
										  IndexSet& ) noexcept;

			static bool GetVertices( const FbxMesh* const,
										   VertexSet& ) noexcept;

			/*	Get Material
			*/
			static Material* GetMaterial( const FbxMesh* const ) noexcept;

			/*	Gen name
			*/
			static FbxString GenName( const FbxNodeAttribute* const ) noexcept;

		private:
			/*	Fbx objects
			*/
			FbxManager* fbxManager;

			FbxIOSettings* fbxIoSettings;

			/*	FORMAT
			*/
			static const char* FORMAT;
		};
}