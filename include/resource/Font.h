/*
**
**	FontCache.
**
	按照 unicode 编码大小，越小的通常越常用，所以当纹理画满之后，可优先替换 值更大的 字符

		Copyright (c) WangFeng. All Rights Reserved.
*/

#pragma once

#include "math/Size.h"
#include "Texture.h"


namespace fce
{
	/*
	!	Font
	*/
	struct Font
	{
		typedef Size2	Size;

		stl::string		name;
		Size			size;
		Float32			italic;
		UInt16			bold;
		UInt16			outline;

		/*
		 !	Constructor
		*/
		Font() {}

		Font( stl::string name,
				Size size,
				Float32 italic = 0,
				UInt16 bold = 0,
				UInt16 outline = 0 )
			: name( name ),
				size( size ),
				italic( italic ),
				bold( bold ),
				outline( outline )
		{}

		/*
		 !	Comparison
		*/
		friend bool operator< ( const Font& font1, const Font& font2 )
		{
			if( font1.name != font2.name )
				return font1.name < font2.name;
			else if( font1.size.width != font2.size.width )
				return font1.size.width < font2.size.width;
			else if( font1.size.height != font2.size.height )
				return font1.size.height < font2.size.height;
			else if( font1.bold != font2.bold )
				return font1.bold < font2.bold;
			else if( font1.italic != font2.italic )
				return font1.italic < font2.italic;
			else if( font1.outline != font2.outline )
				return font1.outline < font2.outline;
			else
				return false;
		}

		/*
		 !	Serialize
		*/
		friend Buffer& operator >> ( Buffer& buf, Font& font )
		{
			return buf	>> font.name
						>> font.size
						>> font.italic
						>> font.bold
						>> font.outline;
		}

		/*
		 !	Deserialize
		*/
		friend Buffer& operator << ( Buffer& buf, const Font& font )
		{
			return buf	<< font.name
						<< font.size
						<< font.italic
						<< font.bold
						<< font.outline;
		}
	};


	/*
	!	Glyph
	*/
	struct Glyph
	{
		/*	STYLE
		*/
		enum class STYLE
		{
			NORMAL		= 0,
			BOLD		= 1 << 0,
			ITALIC		= 1 << 1,
			OUTLINE		= 1 << 2,
			UNDERLINE	= 1 << 3,
			STRIKEOUT	= 1 << 4,
		
			MAXIMUM		= 6
		};

		Texture2*	tex2D;
		Int16		advance;
		Int16		offset;
		Int16		ascent;
		Int16		descent;

		Glyph()	{ tex2D = NULL; }
		~Glyph() { }//if( tex2D ) delete tex2D;
	};


	/*
	!	FontCache
	*/
	class FontCache
	{
	public:
		typedef Vector2	Point;
		typedef Size2	Size;
		typedef Rect2		Rect;

	public:
		virtual const Font*		SetFont( const Font& font ) = 0;
		virtual Void			SetSmooth( Bool smooth ) = 0;
		virtual Void			Clear() = 0;

		virtual const Glyph*	AddGlyph( WChar code ) = 0;
		virtual const Glyph*	GetGlyph( WChar code ) = 0;
		virtual Bool			DelGlyph( WChar code ) = 0;

		virtual Int16			GetBox( const WChar* const str, Size& size ) = 0;

		virtual Void			Render( const WChar* const str,
										const Color4F& color,
										const Point& pos,
										const Rect* clip = NULL,
										const Color4F& underline = Color4( 0 ),
										const Color4F& strikethrough = Color4( 0 ) ) = 0;

	protected:
		virtual ~FontCache(){}
	};
	

	/*
	 !	global FontCache instance by FontCache.
	 */
	extern FC_API FontCache&	g_FontCache;
}