/*
**
**	Image::Info
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../../math/Vector.h"


namespace fce::inner::image
{
	/*	Types
	 */
	enum struct TYPE
		:
			Byte
		{
			PNG,
			JPG,
			TGA,
			BMP,
			RAW,
			FCI,

			MAXIMUM
		};


	/*
	 !	Info
	 */
	class FC_API Info final
		{
		public:
			/*	Model
			 */
			union
				{
					QWord value;

					struct
						{
							/*	Size
							*/
							union
								{
									Size size;

									struct
										{
											Int16 width,
												  height;
										};
								};

							/*	COLOR
							*/
							COLOR color;

							/*	Reserves
							*/
							Byte reserves[ 3 ];
						};
				};

		public:
			/*	Constructor
			*/
			Info() noexcept;

			Info( const Info& ) noexcept;

			Info( const Size size,
				  const COLOR ) noexcept;

			Info( const Int16 width,
				  const Int16 height,
				  const COLOR ) noexcept;

			explicit Info( const Buffer& );

			/*	Operater =
			*/
			constexpr Info& operator = ( const Info& ) noexcept;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator Bool() const noexcept;

			/*	Common
			*/
			[[nodiscard]] UInt8 Depth() const noexcept;

			[[nodiscard]] Int32 Pitch() const noexcept;

			[[nodiscard]] Int32 Area() const noexcept;

			[[nodiscard]] UInt32 Capacity() const noexcept;

			[[nodiscard]] Int32 Pitch( const UInt8 ) const noexcept;

			[[nodiscard]] UInt32 Capacity( const UInt8 ) const noexcept;

			/*	Serialize & Deserialize
			*/
			Buffer& operator >> ( Buffer& ) const NOEXCEPT;

			const Buffer& operator << ( const Buffer& );
		};
}


/**
 !	Inlines
 */
#include "Info.inl"