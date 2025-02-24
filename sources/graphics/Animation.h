/*
**
**	CAnimation.
**
		Copyright (c) WangFeng. All Rights Reserved.
*/

#pragma once

#include "../../interface/graphics/Animation.h"


namespace fce
{
	/*
	!	CAnimation
	*/
	class CAnimation : public IAnimation
	{
		Point	pos;
		UInt8	frmCur;

		Bool	looped;
		Bool	paused;
		Bool	mirrored;

		CTimer	timer;
		Float32	interval;

		Float32	alpha;

		stl::vector< stl::string >	frmFiles;

	public:
		ITexture2D*		GetTexture( UInt8 frame ) const;

		const Point&	GetPos() const				{ return this->pos;							}
		Void			SetPos( const Point& pos )	{ this->pos = pos;							}

		UInt8			GetNum() const				{ return this->frmFiles.size();				}
		UInt8			GetCurFrm() const			{ return this->frmCur;						}
		Void			SetCurFrm( UInt8 frame )	{ this->frmCur = frame;						}

		Bool			GetLooped() const			{ return this->looped;						}
		Void			SetLooped( Bool looped )	{ this->looped = looped;					}

		Bool			GetPaused() const			{ return this->paused;						}
		Void			SetPaused( Bool paused )	{ this->paused = paused;					}

		Bool			GetMirrored() const			{ return this->mirrored;					}
		Void			SetMirrored( Bool mirrored ){ this->mirrored = mirrored;				}

		Float32			GetInterval() const			{ return this->interval;					}
		Void			SetInterval( Float32 t )	{ this->interval = t;						}

		Float32			GetAlpha() const			{ return this->alpha;						}
		Void			SetAlpha( Float32 alpha )	{ this->alpha = alpha;						}

		Bool			IsBegin() const				{ return this->frmCur == 0;					}
		Bool			IsEnd() const				{ return this->frmCur +1 == this->GetNum();	}

		Void			Load( const stl::string dir );
		Void			Reset();

		Void			Render();
		Void			Release()					{ delete this;								}
	};
}