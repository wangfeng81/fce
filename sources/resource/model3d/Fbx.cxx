/*
**
**	Fbx.
**
		要求 mesh texture material 等资源 不能重名
		因为需要唯一的标识访问

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
	 @	析构
	 */
	Fbx::~Fbx() noexcept
		{
			this->Release();
		}

	/*
	 @	构造
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
	 @	释放 fbx
	 */
	void Fbx::Release() noexcept
		{
			if( this->fbxManager )
				{
					this->fbxManager->Destroy();
				}
		}

	/*
	 @	初始化 fbx
	 */
	bool Fbx::Initialize() noexcept
		{
			// 创建 fbx manager
			if( this->fbxManager = FbxManager::Create() )
				{
					// 创建 io settings
					if( this->fbxIoSettings = FbxIOSettings::Create( this->fbxManager,
																	 IOSROOT ) )
						{
							// 设置 io settings
							this->fbxManager->SetIOSettings( this->fbxIoSettings );

							// 加载 plugins
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
	 @	查找 reader id
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
	 @	查找 writer id
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
	 @	生成 name
	 */
	FbxString Fbx::GenName( const FbxNodeAttribute* const fbxNodeAttribute ) noexcept
		{
			// 先尝试 Attribute 名
			FbxString fbxName
				{
					fbxNodeAttribute->GetNameOnly()
				};

			if( fbxName.IsEmpty() )
				{
					// 再试 node 名
					fbxName = fbxNodeAttribute->GetNode()->GetNameOnly();

					// 最后 unique id
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
	 @	转换 Vector
	 */
	template< class Src,
			  class Dst >

	void Fbx::Convert( const Src& src,
							 Dst& dst ) noexcept
		{
			// 分量类型
			using T = std::remove_reference_t< decltype( dst[ 0 ] ) >;

			// 维度
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
	 @	将 多边形 转换为 三角形
	 */
	FbxMesh* Fbx::Triangulate( const FbxNodeAttribute* const fbxNodeAttribute ) noexcept
		{
			// 获取 mesh
			FbxMesh* const fbxMesh
				{
					fbxNodeAttribute->GetNode()->GetMesh()
				};

			// 检测是否 三角化
			if( fbxMesh && ! fbxMesh->IsTriangleMesh() )
				{
					// 进行 三角化，失败返回 null
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
	 @	解析 FbxScene
	 */
	bool Fbx::Parse( const FbxScene* const fbxScene,
						   Scene::Node* const scnNode ) noexcept
		{
			// 解析 root node
			return Parse( fbxScene->GetRootNode(),
						  scnNode );
		}


	/*
	 @	解析 FbxNode
	 */
	bool Fbx::Parse( const FbxNode* const fbxNode,
						   Scene::Node* const scnNode ) noexcept
		{
			if( fbxNode )
				{
					// 解析 组件
					ParseName( fbxNode, scnNode );
					
					ParseTransform( fbxNode, scnNode );

					ParseAttributes( fbxNode, scnNode );

					// 获取 子节点 数量
					const int childCount
						{
							fbxNode->GetChildCount()
						};

					// 递归 子节点
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
	 @	解码 fbx buffer
	 */
	bool Fbx::Decode( const Buffer& buf,
							Scene::Node* const scnNode ) NOEXCEPT
		{
			bool res{ false };

			// 创建 importer
			FbxImporter* const fbxImporter
				{
					FbxImporter::Create( this->fbxManager,
										 nullptr )
				};

			if( fbxImporter )
				{
					// 创建 stream
					FbxStreamBuffer fbxStream;

					// 初始化 importer
					if( fbxImporter->Initialize( &fbxStream,
												 const_cast< Buffer* >( &buf ),
												 -1, this->fbxIoSettings ) )
						{
							// 创建 scene
							FbxScene* const fbxScene
								{
									FbxScene::Create( this->fbxManager,
													  nullptr )
								};

							if( fbxScene )
								{
									// 导入 scene
									if( fbxImporter->Import( fbxScene ) )
										{
											// 解析 scene
											res = this->Parse( fbxScene,
															   scnNode );
										}

									// 销毁 scene
									fbxScene->Destroy( true );
								}
						}

					// 销毁 importer
					fbxImporter->Destroy( true );
				}

			return res;
		}
}



namespace fce
{
	/*
	 @	解析 name
	 */
	void Fbx::ParseName( const FbxNode* const fbxNode,
							   Scene::Node* const scnNode ) noexcept
		{
			// 获取 fbxNode 名字
			const FbxString fbxName
				{
					fbxNode->GetNameOnly()
				};

			if( ! fbxName.IsEmpty() )
				{
					// 添加 scnNode name
					scnNode->AddName( fbxName );
				}
		}


	/*
	 @	解析 transform
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
	 @	解析 mesh
	 */
	bool Fbx::ParseMesh( const FbxMesh* const fbxMesh,
							   Scene::Node* const scnNode ) noexcept
		{
			if( fbxMesh )
				{
					// 调整坐标系
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
	 @	解析 attributes
	 */
	bool Fbx::ParseAttributes( const FbxNode* const fbxNode,
									 Scene::Node* const scnNode ) noexcept
		{
			// 获取 attribute 数量
			const int attrCount
				{
					fbxNode->GetNodeAttributeCount()
				};

			// 遍历 attributes
			for( int i{ 0 }; i < attrCount; ++i )
				{
					// 获取 attribute
					const FbxNodeAttribute* const fbxNodeAttribute
						{
							fbxNode->GetNodeAttributeByIndex( i )
						};

					if( fbxNodeAttribute )
						{
							// 按 type 分流 attribute
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
	 @	获取 三角形列表 顶点集
	 */
	bool Fbx::GetMesh( const FbxMesh* const fbxMesh,
							 VertexSet& vertexSet ) noexcept
		{
			if( fbxMesh )
				{
					// 三角形 数量
					const int triangleCount
						{
							// 多边形 已全部 三角化
							fbxMesh->GetPolygonCount()
						};

					// 控点 数组
					const FbxVector4* const controlPoints
						{
							fbxMesh->GetControlPoints()
						};

					// 目前只取 第一层layer
					//const int uvCount
					//	{
					//		fbxMesh->GetElementUVCount()
					//	};

					// UV 
					const FbxGeometryElementUV* const fbxUV
						{
							fbxMesh->GetElementUV( 0 )
						};

					// 法线
					const FbxGeometryElementNormal* fbxNormal
						{
							fbxMesh->GetElementNormal( 0 )
						};

					//// 切线
					//const FbxGeometryElementTangent* fbxTangent
					//	{
					//		fbxMesh->GetElementTangent( 0 )
					//	};

					//// 副法线
					//const FbxGeometryElementBinormal* fbxBinormal
					//	{
					//		fbxMesh->GetElementBinormal( 0 )
					//	};

					// 颜色
					//const FbxGeometryElementVertexColor* fbxColor
					//	{
					//		fbxMesh->GetElementVertexColor( 0 )
					//	};

					// 检测 有效性
					if( fbxUV
					 && fbxNormal
					 //&& fbxTangent
					 //&& fbxBinormal
					 && controlPoints
					 && triangleCount > 0 )
						{
							// 三角形 vertex & control id
							int controlId;

							int vertexId{ 0 };

							// 分配 vertexSet 尺寸
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

							// 创建 FbxMesh 各顶点元素对象
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

							// 遍历 triangle 列表
							for( int i{ 0 }; i < triangleCount; ++i )
								{
									// 只处理 triangle
									if( fbxMesh->GetPolygonSize( i ) != 3 )
										{
											return false;
										}

									// 遍历 三个顶点
									for( int j{ 0 }; j < 3; ++j )
										{
											// 获取 control id
											controlId = fbxMesh->GetPolygonVertex( i, j );

											if( controlId < 0 )
												{
													return false;
												}

											// 转换 position
											Convert( controlPoints[ controlId ],
													 vertices->pos );

											// 转换其它 各顶点元素
											elementUV.Convert( vertices->uv );
											elementNormal.Convert( vertices->normal );
											//elementTangent.Convert( vertices->tangent );
											//elementBinormal.Convert( vertices->binormal );

											// 移下一个 vertex
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
	 @	获取 三角形列表 的索引集
	 */
	bool Fbx::GetIndices( const FbxMesh* const fbxMesh,
								IndexSet& indexSet ) noexcept
		{
			if( fbxMesh )
				{
					// 考虑 优化 16位 的索引

					// 三角形 数量
					const int triangleCount
						{
							// 多边形 已全部 三角化
							fbxMesh->GetPolygonCount()
						};

					if( triangleCount > 0 )
						{
							// 顶点 数量
							const int vertexCount
								{
									triangleCount * 3
								};

							// 分配 indexSet 尺寸
							indexSet.Resize( vertexCount,
											 TYPE::UINT32 );

							UInt32* indices
								{
									indexSet.Data< UInt32 >()
								};

							// 遍历 三角形
							for( int i{ 0 }; i < triangleCount; ++i )
								{
									// 只处理 三角形
									if( fbxMesh->GetPolygonSize( i ) != 3 )
										{
											return false;
										}

									// 遍历 三个顶点
									for( int j{ 0 }; j < 3; ++j )
										{
											// 获取 index
											const int index
												{
													fbxMesh->GetPolygonVertex( i, j )
												};

											// 检测 有效性
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
	 @	获取 三角形列表 的控点集
	 */
	bool Fbx::GetVertices( const FbxMesh* const fbxMesh,
								 VertexSet& vertexSet ) noexcept
		{
			if( fbxMesh )
				{
					// 控点 数量
					int controlCount
						{
							fbxMesh->GetControlPointsCount()
						};

					if( controlCount > 0 )
						{
							// 控点 数组
							FbxVector4* controlPoints
								{
									fbxMesh->GetControlPoints()
								};

							if( controlPoints )
								{
									// 分配 vertexSet 尺寸
									vertexSet.Resize( controlCount,
													  sizeof( Vector3 ) );

									Vector3* vertices
										{
											vertexSet.Data< Vector3 >()
										};

									// 遍历 vertex
									while( controlCount-- )
										{
											// 转换 position
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
	 @	获取 Material
	 */
	Material* Fbx::GetMaterial( const FbxMesh* const fbxMesh ) noexcept
		{
			// 获取 第一层 材质
			const FbxSurfaceMaterial* const fbxSurfaceMaterial
				{
					fbxMesh->GetNode()->GetMaterial( 0 )
				};

			if( fbxSurfaceMaterial )
				{
					// 获取 fbxProperty
					const FbxProperty fbxProperty
						{
							fbxSurfaceMaterial->FindProperty
								(
									FbxLayerElement::sTextureChannelNames[ 0 ]
								)
						};

					// 获取 FbxTexture
					const FbxTexture* const fbxTexture
						{
							FbxCast< FbxTexture >( fbxProperty.GetSrcObject( 0 ) )
						};

					Path pathTexture( "line.png" );

					if( fbxTexture )
						{
							// 获取文件名
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
	//		// 获取 FbxAnimStack 数量，进行遍历
	//		int countOfAnimStacks
	//			{
	//				this->fbxScene->GetSrcObjectCount< FbxAnimStack >()
	//			};

	//		for( int i{ 0 }; i < countOfAnimStacks; ++i )
	//			{
	//				// 获取单个 FbxAnimStack
	//				FbxAnimStack* fbxAnimStack
	//					{
	//						this->fbxScene->GetSrcObject< FbxAnimStack >( i )
	//					};

	//				// 获取 FbxAnimLayer
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
	//		// 获取 FbxAnimLayer 数量，进行遍历
	//		int countOfAnimLayers
	//			{
	//				fbxAnimStack->GetMemberCount< FbxAnimLayer >()
	//			};

	//		for( int i{ 0 }; i < countOfAnimLayers; ++i )
	//			{
	//				// 获取单个 FbxAnimStack
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