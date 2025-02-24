/*
**
**	Fbx vertex element.
**
		Copyright (c) WangFeng since 2016...
*/

namespace fce
{
	/*
	 !	FbxVertexElement
	 */
	template< class Src >

	class FbxVertexElement final
		{
			/*	element id
			*/
			const int& id;

			/*	Direct array
			*/
			const FbxLayerElementArrayTemplate< Src >& directArray;

			/*	Index array
			*/
			const FbxLayerElementArrayTemplate< int >* const indexArray;

		public:
			/*
			 @	Constructor
			 */
			FbxVertexElement( const FbxLayerElementTemplate< Src >* const fbxLayerElement,
							  const int& controlId,
							  const int& vertexId ) NOEXCEPT
				:
					id( FbxGeometryElement::eByControlPoint
						== fbxLayerElement->GetMappingMode()
						? controlId : vertexId ),

					directArray( fbxLayerElement->GetDirectArray() ),

					indexArray( FbxGeometryElement::eDirect
								!= fbxLayerElement->GetReferenceMode()
								? &fbxLayerElement->GetIndexArray()
								: nullptr )
				{
				}

			/*
			 @	Convert vertex element
			 */
			template< class Dst >

			FC_INLINE void Convert( Dst& dstVertexElement ) const noexcept
				{
					Fbx::Convert( this->GetElement(),
								  dstVertexElement );
				}

		private:
			/*
			 @	Get index
			 */
			FC_INLINE int GetIndex() const noexcept
				{
					return this->indexArray
						 ? this->indexArray->GetAt( this->id )
						 : this->id;
				}

			/*
			 @	Get element
			 */
			FC_INLINE Src GetElement() const noexcept
				{
					return this->directArray.GetAt
								(
									this->GetIndex()
								);
				}
		};
}