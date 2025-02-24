/*
**
**	Utility.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/common/Utility.h"


/**
 !	PrettyFunction
 */
namespace fce
{
	/*
	 @	Format name
	 */
	stl::string PrettyFunction::FormatName( const std::string_view& rawName ) noexcept
		{
			const stl::string str( rawName );

			const std::regex regex( "class |struct |union | " );

			return std::regex_replace( str, regex, "" );
		}
}









//namespace fce
//{
//	/*
//	 !	CRC32C Castagnoli
//	 */
//	class Crc32c
//		{
//		public:
//			/*	Check sum
//			*/
//			UInt32 CheckSum( const Byte* data, UInt32 size ) noexcept;
//
//			static Crc32c singleton;
//		protected:
//			/*	Constructor
//			*/
//			Crc32c() noexcept;
//
//			/*	Init table
//			*/
//			Void InitTable() noexcept;
//
//			/*	CRC_32C table
//			*/
//			UInt32 arrTable[ 256 ];
//
//			/*	CRC-32C Castagnoli
//			*/
//			static constexpr UInt32	POLY{ 0x82F63B78 };
//
//		};
//
//	Crc32c Crc32c::singleton;
//
//	/*
//	 @	Constructor
//	 */
//	Crc32c::Crc32c() noexcept
//		{
//			this->InitTable();
//		}
//
//	/*
//	 @	初始化 CRC_32C 表.
//	 */
//	Void Crc32c::InitTable() noexcept
//		{
//			for( UInt16 i{ 0 }; i < 256; i++ )
//				{
//					UInt32 crc{ i };
//
//					for( UInt8 j{ 0 }; j < 8; j++ )
//						{
//							if( crc & 1 )
//								{
//									crc = ( crc >> 1 ) ^ 0x04C11DB7;
//								}
//							else
//								{
//									crc >>= 1;
//								}
//						}
//
//					this->arrTable[ i ] = crc;
//				}
//		}
//
//
//	/*
//	 @	32位CRC	0x1EDC6F41 or 0x82F63B78 (0x05EC76F1)
//			data		数据
//			size		大小
//			RETURN	Crc32c校验码
//	 */
//	UInt32 Crc32c::CheckSum( const Byte* data, UInt32 size ) noexcept
//		{
//			UInt32 checksum{ ~0u };
//
//			for( DWord c{ 0 }; c < size; c++ )
//				{
//					checksum = ( checksum >> 8 ) ^
//						this->arrTable[ ( checksum ^ data[ c ] ) & 0xFF ];
//				}
//
//			return ~checksum;
//		}
//}
