/*
**
**	VK_TempBuffer.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_TempBuffer
	 */
	class FC_API VK_TempBuffer final
		{
			/*	Friends
			*/
			friend Image;

			friend IndexSet;

			friend VertexSet;

		public:
			/*	Size
			*/
			[[nodiscard]] UInt32 Size() const noexcept;

			/*	Get base pointer
			*/
			template< typename T = Byte >

			[[nodiscard]] auto* Data( this auto& ) noexcept;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator bool() const noexcept;

			/*	Get VkBuffer
			*/
			[[nodiscard]] auto* operator * ( this auto& ) noexcept;

			[[nodiscard]] operator const VkBuffer() const noexcept;

			[[nodiscard]] const VkBuffer& GetHandle() const noexcept;

			/*	Zero
			*/
			Void Zero() noexcept;

			/*	Resize
			*/
			void Resize( const VkDeviceSize ) NOEXCEPT;

			/*	Serialize
			*/
			Buffer& operator >> ( Buffer& ) const NOEXCEPT;

			/*	Assignment
			*/
			VK_TempBuffer& operator = ( VK_TempBuffer&& ) noexcept;

			VK_TempBuffer& operator = ( const VK_TempBuffer& ) NOEXCEPT;

			/*	Copy buffer
			*/
			VkResult Copy( VkBuffer dstBuffer,
						   const VkDeviceSize size,
						   const VkDeviceSize srcOffset,
						   const VkDeviceSize dstOffset ) const NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			~VK_TempBuffer() noexcept;

			VK_TempBuffer( VK_TempBuffer&& ) noexcept;

			VK_TempBuffer( const VK_TempBuffer& ) NOEXCEPT;

			explicit VK_TempBuffer( const VkDeviceSize = 0 ) NOEXCEPT;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const VkDeviceSize ) noexcept;

		private:
			/*	Mapped data
			*/
			void* mappedData;

			/*	VkDeviceSize
			*/
			VkDeviceSize size;

			/*	VkBuffer
			*/
			VkBuffer vkBuffer;

			/*	VmaAllocation
			*/
			VmaAllocation vmaAlloc;
		};
}



/**
 !	class VK_TempBuffer
 */
namespace fce::inner::vulkan
{
	/*
	 @	Zero
	 */
	FC_INLINE Void VK_TempBuffer::Zero() noexcept
		{
			memset( this->mappedData, 0,
					this->size );
		}

	/*
	 @	Size
	 */
	FC_INLINE UInt32 VK_TempBuffer::Size() const noexcept
		{
			return static_cast< UInt32 >( this->size );
		}

	/*
	 @	Check whether the buffer is valid
	 */
	FC_INLINE VK_TempBuffer::operator bool() const noexcept
		{
			return this->mappedData;
		}

	/*
	 @	Get base pointer
	 */
	template< typename T >

	FC_INLINE auto* VK_TempBuffer::Data( this auto& self ) noexcept
		{
			return &std::forward_like< decltype( self ) >
					(
						*static_cast< T* >( self.mappedData )
					);
		}

	/*
	 @	Get vkBuffer
	 */
	FC_INLINE VK_TempBuffer::operator const VkBuffer() const noexcept
		{
			return this->vkBuffer;
		}

	FC_INLINE const VkBuffer& VK_TempBuffer::GetHandle() const noexcept
		{
			return this->vkBuffer;
		}

	FC_INLINE auto* VK_TempBuffer::operator * ( this auto& self ) noexcept
		{
			return self.Data();
		}
}