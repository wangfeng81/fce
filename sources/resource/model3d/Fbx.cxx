/*
**
**	Fbx.
**
		Ҫ�� mesh texture material ����Դ ��������
		��Ϊ��ҪΨһ�ı�ʶ����

	Copyright (c) WangFeng since 2012...
*/

#include "FbxStreamBuffer.h"
#include "FbxVertexElement.h"

#ifdef FC_WINDOWS
	#pragma comment( lib, "libfbxsdk.lib" )
#endif



namespace fce
{
	/*
	 @	Singleton instantiation
	 */
	Fbx Fbx::instance;

	/*
	 @	FORMAT
	 */
	const char* Fbx::FORMAT
		{
			"FBX (*.fbx)"
		};

	/*
	 @	����
	 */
	Fbx::~Fbx() noexcept
		{
			this->Release();
		}

	/*
	 @	����
	 */
	Fbx::Fbx() noexcept
		:
			fbxManager( nullptr ),
			fbxIoSettings( nullptr )
		{
			if( ! this->Initialize() )
				{
					std::quick_exit( -1 );
				}
		}
}



namespace fce
{
	/*
	 @	�ͷ� fbx
	 */
	void Fbx::Release() noexcept
		{
			if( this->fbxManager )
				{
					this->fbxManager->Destroy();
				}
		}

	/*
	 @	��ʼ�� fbx
	 */
	bool Fbx::Initialize() noexcept
		{
			// ���� fbx manager
			if( this->fbxManager = FbxManager::Create() )
				{
					// ���� io settings
					if( this->fbxIoSettings = FbxIOSettings::Create( this->fbxManager,
																	 IOSROOT ) )
						{
							// ���� io settings
							this->fbxManager->SetIOSettings( this->fbxIoSettings );

							// ���� plugins
							const FbxString appPath
								{
									FbxGetApplicationDirectory()
								};

							return this->fbxManager->LoadPluginsDirectory( appPath.Buffer() );
						}
				}

			return false;
		}
}



namespace fce
{
	/*
	 @	���� reader id
	 */
	int Fbx::FindReaderID() const noexcept
		{
			const FbxIOPluginRegistry* const fbxIOPluginRegistry
				{
					this->fbxManager->GetIOPluginRegistry()
				};

			if( fbxIOPluginRegistry )
				{
					return fbxIOPluginRegistry->FindReaderIDByDescription( FORMAT );
				}

			return -1;
		}

	/*
	 @	���� writer id
	 */
	int Fbx::FindWriterID() const noexcept
		{
			const FbxIOPluginRegistry* const fbxIOPluginRegistry
				{
					this->fbxManager->GetIOPluginRegistry()
				};

			if( fbxIOPluginRegistry )
				{
					return fbxIOPluginRegistry->FindWriterIDByDescription( FORMAT );
				}

			return -1;
		}


	/*
	 @	���� name
	 */
	FbxString Fbx::GenName( const FbxNodeAttribute* const fbxNodeAttribute ) noexcept
		{
			// �ȳ��� Attribute ��
			FbxString fbxName
				{
					fbxNodeAttribute->GetNameOnly()
				};

			if( fbxName.IsEmpty() )
				{
					// ���� node ��
					fbxName = fbxNodeAttribute->GetNode()->GetNameOnly();

					// ��� unique id
					if( fbxName.IsEmpty() )
						{
							const std::string uniqueID
								 {
									 std::format( "{:0X}",
												  fbxNodeAttribute->GetUniqueID() )
								 };

							fbxName = uniqueID.c_str();
						}
				}

			return fbxName;
		}


	/*
	 @	ת�� Vector
	 */
	template< class Src,
			  class Dst >

	void Fbx::Convert( const Src& src,
							 Dst& dst ) noexcept
		{
			// ��������
			using T = std::remove_reference_t< decltype( dst[ 0 ] ) >;

			// ά��
			constexpr uint8_t N
				{
					uint8_t( sizeof( Dst ) / sizeof( T ) )
				};

			for( uint8_t i{ 0 }; i < N; ++i )
				{
					dst[ i ] = static_cast< T >( src[ i ] );
				}
		}


	/*
	 @	�� ����� ת��Ϊ ������
	 */
	FbxMesh* Fbx::Triangulate( const FbxNodeAttribute* const fbxNodeAttribute ) noexcept
		{
			// ��ȡ mesh
			FbxMesh* const fbxMesh
				{
					fbxNodeAttribute->GetNode()->GetMesh()
				};

			// ����Ƿ� ���ǻ�
			if( fbxMesh && ! fbxMesh->IsTriangleMesh() )
				{
					// ���� ���ǻ���ʧ�ܷ��� null
					FbxGeometryConverter fbxGeometryConverter( this->fbxManager );

					if( ! fbxGeometryConverter.Triangulate( fbxMesh->GetScene(),
															true, false ) )
						{
							return nullptr;
						}
				}

			return fbxMesh;
		}
}



namespace fce
{
	/*
	 @	���� FbxScene
	 */
	bool Fbx::Parse( const FbxScene* const fbxScene,
						   Scene::Node* const scnNode ) noexcept
		{
			// ���� root node
			return Parse( fbxScene->GetRootNode(),
						  scnNode );
		}


	/*
	 @	���� FbxNode
	 */
	bool Fbx::Parse( const FbxNode* const fbxNode,
						   Scene::Node* const scnNode ) noexcept
		{
			if( fbxNode )
				{
					// ���� ���
					ParseName( fbxNode, scnNode );
					
					ParseTransform( fbxNode, scnNode );

					ParseAttributes( fbxNode, scnNode );

					// ��ȡ �ӽڵ� ����
					const int childCount
						{
							fbxNode->GetChildCount()
						};

					// �ݹ� �ӽڵ�
					for( int i{ 0 }; i < childCount; ++i )
						{
							Parse( fbxNode->GetChild( i ),
								   scnNode->NewChild() );
						}

					return true;
				}

			return false;
		}


	/*
	 @	���� fbx buffer
	 */
	bool Fbx::Decode( const Buffer& buf,
							Scene::Node* const scnNode ) NOEXCEPT
		{
			bool res{ false };

			// ���� importer
			FbxImporter* const fbxImporter
				{
					FbxImporter::Create( this->fbxManager,
										 nullptr )
				};

			if( fbxImporter )
				{
					// ���� stream
					FbxStreamBuffer fbxStream;

					// ��ʼ�� importer
					if( fbxImporter->Initialize( &fbxStream,
												 const_cast< Buffer* >( &buf ),
												 -1, this->fbxIoSettings ) )
						{
							// ���� scene
							FbxScene* const fbxScene
								{
									FbxScene::Create( this->fbxManager,
													  nullptr )
								};

							if( fbxScene )
								{
									// ���� scene
									if( fbxImporter->Import( fbxScene ) )
										{
											// ���� scene
											res = this->Parse( fbxScene,
															   scnNode );
										}

									// ���� scene
									fbxScene->Destroy( true );
								}
						}

					// ���� importer
					fbxImporter->Destroy( true );
				}

			return res;
		}
}



namespace fce
{
	/*
	 @	���� name
	 */
	void Fbx::ParseName( const FbxNode* const fbxNode,
							   Scene::Node* const scnNode ) noexcept
		{
			// ��ȡ fbxNode ����
			const FbxString fbxName
				{
					fbxNode->GetNameOnly()
				};

			if( ! fbxName.IsEmpty() )
				{
					// ��� scnNode name
					scnNode->AddName( fbxName );
				}
		}


	/*
	 @	���� transform
	 */
	void Fbx::ParseTransform( const FbxNode* const fbxNode,
									Scene::Node* const scnNode ) noexcept
		{
			Transform& transform{ scnNode->transform };

			// scale
			Convert( fbxNode->LclScaling.Get(),
					 transform.scale );

			// position
			Convert( fbxNode->LclTranslation.Get(),
					 transform.position );

			// orientation
			Vector3 angles;

			Convert( fbxNode->LclRotation.Get(),
					 angles );

			transform.orientation = angles;
		}


	/*
	 @	���� mesh
	 */
	bool Fbx::ParseMesh( const FbxMesh* const fbxMesh,
							   Scene::Node* const scnNode ) noexcept
		{
			if( fbxMesh )
				{
					// ��������ϵ
					scnNode->RotateX( math::Radian( -90.0f ) );

					if( 0 )
						{
							// IndexSet
							IndexSet indexSet;

							// VertexSet
							VertexSet vertexSet;

							//
							if( GetIndices( fbxMesh, indexSet )
							 && GetVertices( fbxMesh, vertexSet ) )
								{
									// Material
									Material* material
										{
											Material::Generate< Material >( "renderPipeline/WireFrame.rpl" )
										};

									// indexMeshRenderer
									renderer::IndexMesh& indexMeshRenderer
										{
											scnNode->AddComponent< renderer::IndexMesh >
												(
													material,
													IndexBuffer::Generate( indexSet ),
													VertexBuffer::Generate( vertexSet )
												)
										};

									return true;
								}
						}
					else
						{
							// VertexSet
							VertexSet vertexSet;

							// Material
							Material* const material
								{
									GetMaterial( fbxMesh )
								};

							//
							if( material && GetMesh( fbxMesh, vertexSet ) )
								{
									// MeshRenderer
									renderer::Mesh& meshRenderer
										{
											scnNode->AddComponent< renderer::Mesh >
												(
													material,
													VertexBuffer::Generate( vertexSet )
												)
										};

									return true;
								}
						}
				}

			return false;
		}


	/*
	 @	���� attributes
	 */
	bool Fbx::ParseAttributes( const FbxNode* const fbxNode,
									 Scene::Node* const scnNode ) noexcept
		{
			// ��ȡ attribute ����
			const int attrCount
				{
					fbxNode->GetNodeAttributeCount()
				};

			// ���� attributes
			for( int i{ 0 }; i < attrCount; ++i )
				{
					// ��ȡ attribute
					const FbxNodeAttribute* const fbxNodeAttribute
						{
							fbxNode->GetNodeAttributeByIndex( i )
						};

					if( fbxNodeAttribute )
						{
							// �� type ���� attribute
							switch( fbxNodeAttribute->GetAttributeType() )
								{
									case FbxNodeAttribute::eUnknown:				break;
									case FbxNodeAttribute::eNull:					break;
									case FbxNodeAttribute::eMarker:					break;
									case FbxNodeAttribute::eSkeleton:				break;

									case FbxNodeAttribute::eMesh:
										{
											return ParseMesh( instance.Triangulate( fbxNodeAttribute ),
															  scnNode );
										}

									case FbxNodeAttribute::eNurbs:					break;
									case FbxNodeAttribute::ePatch:					break;
									case FbxNodeAttribute::eCamera:					break;
									case FbxNodeAttribute::eCameraStereo:			break;
									case FbxNodeAttribute::eCameraSwitcher:			break;
									case FbxNodeAttribute::eLight:					break;
									case FbxNodeAttribute::eOpticalReference:		break;
									case FbxNodeAttribute::eOpticalMarker:			break;
									case FbxNodeAttribute::eNurbsCurve:				break;
									case FbxNodeAttribute::eTrimNurbsSurface:		break;
									case FbxNodeAttribute::eBoundary:				break;
									case FbxNodeAttribute::eNurbsSurface:			break;
									case FbxNodeAttribute::eShape:					break;
									case FbxNodeAttribute::eLODGroup:				break;
									case FbxNodeAttribute::eSubDiv:					break;
									case FbxNodeAttribute::eCachedEffect:			break;
									case FbxNodeAttribute::eLine:					break;
								}
							}
				}

			return true;
		}
}



namespace fce
{
	/*
	 @	��ȡ �������б� ���㼯
	 */
	bool Fbx::GetMesh( const FbxMesh* const fbxMesh,
							 VertexSet& vertexSet ) noexcept
		{
			if( fbxMesh )
				{
					// ������ ����
					const int triangleCount
						{
							// ����� ��ȫ�� ���ǻ�
							fbxMesh->GetPolygonCount()
						};

					// �ص� ����
					const FbxVector4* const controlPoints
						{
							fbxMesh->GetControlPoints()
						};

					// Ŀǰֻȡ ��һ��layer
					//const int uvCount
					//	{
					//		fbxMesh->GetElementUVCount()
					//	};

					// UV 
					const FbxGeometryElementUV* const fbxUV
						{
							fbxMesh->GetElementUV( 0 )
						};

					// ����
					const FbxGeometryElementNormal* fbxNormal
						{
							fbxMesh->GetElementNormal( 0 )
						};

					//// ����
					//const FbxGeometryElementTangent* fbxTangent
					//	{
					//		fbxMesh->GetElementTangent( 0 )
					//	};

					//// ������
					//const FbxGeometryElementBinormal* fbxBinormal
					//	{
					//		fbxMesh->GetElementBinormal( 0 )
					//	};

					// ��ɫ
					//const FbxGeometryElementVertexColor* fbxColor
					//	{
					//		fbxMesh->GetElementVertexColor( 0 )
					//	};

					// ��� ��Ч��
					if( fbxUV
					 && fbxNormal
					 //&& fbxTangent
					 //&& fbxBinormal
					 && controlPoints
					 && triangleCount > 0 )
						{
							// ������ vertex & control id
							int controlId;

							int vertexId{ 0 };

							// ���� vertexSet �ߴ�
							const int vertexCount
								{
									triangleCount * 3
								};

							vertexSet.Resize( vertexCount,
											  sizeof( Vertex ) );

							Vertex* vertices
								{
									vertexSet.Data< Vertex >()
								};

							// ���� FbxMesh ������Ԫ�ض���
							const FbxVertexElement elementUV( fbxUV,
															  controlId,
															  vertexId );

							const FbxVertexElement elementNormal( fbxNormal,
																  controlId,
																  vertexId );

							//const FbxVertexElement elementTangent( fbxTangent,
							//									   controlId,
							//									   vertexId );

							//const FbxVertexElement elementBinormal( fbxBinormal,
							//										controlId,
							//										vertexId );

							// ���� triangle �б�
							for( int i{ 0 }; i < triangleCount; ++i )
								{
									// ֻ���� triangle
									if( fbxMesh->GetPolygonSize( i ) != 3 )
										{
											return false;
										}

									// ���� ��������
									for( int j{ 0 }; j < 3; ++j )
										{
											// ��ȡ control id
											controlId = fbxMesh->GetPolygonVertex( i, j );

											if( controlId < 0 )
												{
													return false;
												}

											// ת�� position
											Convert( controlPoints[ controlId ],
													 vertices->pos );

											// ת������ ������Ԫ��
											elementUV.Convert( vertices->uv );
											elementNormal.Convert( vertices->normal );
											//elementTangent.Convert( vertices->tangent );
											//elementBinormal.Convert( vertices->binormal );

											// ����һ�� vertex
											++vertexId;
											++vertices;
										}
								}

							return true;
						}
				}

			return false;
		}


	/*
	 @	��ȡ �������б� ��������
	 */
	bool Fbx::GetIndices( const FbxMesh* const fbxMesh,
								IndexSet& indexSet ) noexcept
		{
			if( fbxMesh )
				{
					// ���� �Ż� 16λ ������

					// ������ ����
					const int triangleCount
						{
							// ����� ��ȫ�� ���ǻ�
							fbxMesh->GetPolygonCount()
						};

					if( triangleCount > 0 )
						{
							// ���� ����
							const int vertexCount
								{
									triangleCount * 3
								};

							// ���� indexSet �ߴ�
							indexSet.Resize( vertexCount,
											 TYPE::UINT32 );

							UInt32* indices
								{
									indexSet.Data< UInt32 >()
								};

							// ���� ������
							for( int i{ 0 }; i < triangleCount; ++i )
								{
									// ֻ���� ������
									if( fbxMesh->GetPolygonSize( i ) != 3 )
										{
											return false;
										}

									// ���� ��������
									for( int j{ 0 }; j < 3; ++j )
										{
											// ��ȡ index
											const int index
												{
													fbxMesh->GetPolygonVertex( i, j )
												};

											// ��� ��Ч��
											if( index < 0 )
												{
													return false;
												}

											*indices++ = index;
										}
								}

							return true;
						}
				}

			return false;
		}


	/*
	 @	��ȡ �������б� �Ŀص㼯
	 */
	bool Fbx::GetVertices( const FbxMesh* const fbxMesh,
								 VertexSet& vertexSet ) noexcept
		{
			if( fbxMesh )
				{
					// �ص� ����
					int controlCount
						{
							fbxMesh->GetControlPointsCount()
						};

					if( controlCount > 0 )
						{
							// �ص� ����
							FbxVector4* controlPoints
								{
									fbxMesh->GetControlPoints()
								};

							if( controlPoints )
								{
									// ���� vertexSet �ߴ�
									vertexSet.Resize( controlCount,
													  sizeof( Vector3 ) );

									Vector3* vertices
										{
											vertexSet.Data< Vector3 >()
										};

									// ���� vertex
									while( controlCount-- )
										{
											// ת�� position
											Convert( *controlPoints++,
													 *vertices++ );
										}

									return true;
								}
						}
				}

			return false;
		}


	/*
	 @	��ȡ Material
	 */
	Material* Fbx::GetMaterial( const FbxMesh* const fbxMesh ) noexcept
		{
			// ��ȡ ��һ�� ����
			const FbxSurfaceMaterial* const fbxSurfaceMaterial
				{
					fbxMesh->GetNode()->GetMaterial( 0 )
				};

			if( fbxSurfaceMaterial )
				{
					// ��ȡ fbxProperty
					const FbxProperty fbxProperty
						{
							fbxSurfaceMaterial->FindProperty
								(
									FbxLayerElement::sTextureChannelNames[ 0 ]
								)
						};

					// ��ȡ FbxTexture
					const FbxTexture* const fbxTexture
						{
							FbxCast< FbxTexture >( fbxProperty.GetSrcObject( 0 ) )
						};

					Path pathTexture( "line.png" );

					if( fbxTexture )
						{
							// ��ȡ�ļ���
							if( const char* name{ fbxTexture->GetName() } )
								{
									pathTexture = stl::string( name ) + ".png";

									if( ! s_StorageMgmt.Exists( pathTexture ) )
										{
											pathTexture = "line.png";
										}
								}
						}

					return Material::Generate< material::Mesh >( "renderPipeline/Mesh.rpl",
																 pathTexture );
				}

			return nullptr;
		}
}



namespace fce
{
	/*
	 @	Load fbx file
	 */
	//bool Fbx::LoadAmi()
	//	{
	//		// ��ȡ FbxAnimStack ���������б���
	//		int countOfAnimStacks
	//			{
	//				this->fbxScene->GetSrcObjectCount< FbxAnimStack >()
	//			};

	//		for( int i{ 0 }; i < countOfAnimStacks; ++i )
	//			{
	//				// ��ȡ���� FbxAnimStack
	//				FbxAnimStack* fbxAnimStack
	//					{
	//						this->fbxScene->GetSrcObject< FbxAnimStack >( i )
	//					};

	//				// ��ȡ FbxAnimLayer
	//				this->LoadAmiLayer( fbxAnimStack );

	//				FbxString lOutputString( fbxAnimStack->GetName() );
	//			}

	//		return false;
	//	}


	///*
	// @	Load fbx file
	// */
	//bool Fbx::LoadAmiLayer( FbxAnimStack* fbxAnimStack )
	//	{
	//		// ��ȡ FbxAnimLayer ���������б���
	//		int countOfAnimLayers
	//			{
	//				fbxAnimStack->GetMemberCount< FbxAnimLayer >()
	//			};

	//		for( int i{ 0 }; i < countOfAnimLayers; ++i )
	//			{
	//				// ��ȡ���� FbxAnimStack
	//				FbxAnimLayer* fbxAnimLayer
	//					{
	//						fbxAnimStack->GetMember< FbxAnimLayer >( i )
	//					};

	//				FbxString lOutputString( fbxAnimLayer->GetName() );
	//			}

	//		return false;
	//	}


	///*
	// @	Travel
	// */
	//void Fbx::Travel( FbxNode* fbxNode,
	//				  Scene::Node* scnNode )
	//	{
	//		FC_ASSERT( fbxNode );

	//		//FbxAnimCurveNode* fbxAnimCurveNode = fbxNode->LclRotation.GetCurveNode( this->fbxScene->GetSrcObject< FbxAnimStack >( 0 ) );
	//		//if( fbxAnimCurveNode )
	//		//	{
	//		//		int cc = fbxAnimCurveNode->GetCurveCount(0);
	//		//		auto n = fbxAnimCurveNode->GetName();

	//		//		FbxAnimCurve* ac = fbxAnimCurveNode->GetCurve( 0 );

	//		//		FbxTime fbxTime = ac->KeyGetTime( 2 );
	//		//		FbxVector4 rot = fbxNode->EvaluateLocalRotation( fbxTime );
	//		//		int k = ac->KeyGetCount();
	//		//		fbxAnimCurveNode->GetCurve( 2 );
	//		//	}

	//		//FbxAnimCurve* lAnimCurve =
	//		//	fbxNode->LclRotation.GetCurve( this->fbxScene->GetSrcObject< FbxAnimStack >( 0 )->GetMember< FbxAnimLayer >( 0 ), FBXSDK_CURVENODE_COMPONENT_X );
	//		//if( lAnimCurve )
	//		//	{
	//		//	auto n= fbxNode->GetName();
	//		//	int k =lAnimCurve->KeyGetCount();
	//		//	//KFCurve* pCurve = lAnimCurve->GetKFCurve();
	//		//	FbxTime fbxTime = lAnimCurve->KeyGetTime( 2 );
	//		//	FbxVector4 rot = fbxNode->EvaluateLocalRotation( fbxTime );
	//		//	lAnimCurve = nullptr;
	//		//	}
	//	}
}