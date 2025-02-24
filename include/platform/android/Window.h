/*
**
**	CWindow.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include <EGL/egl.h>

#include "../../../interface/platform/Window.h"
#include "../../../interface/common/Singleton.h"


namespace fce
{
	namespace android
	{
		/*
		!	CWindow
		*/
		class CWindow : public IWindow
		{
			ANativeWindow*		window;

			EGLDisplay			display;
			EGLSurface			surface;
			EGLContext			context;

			Size		size;

		public:
			EGLBoolean			Create( ANativeWindow* anw );
			EGLBoolean			Destroy();
			void				Present();
			void				OnResize( const Size& size );

			// IWindow
			Point		GetPos() const;
			Size		GetSize() const;
			Rect		GetRect() const;

			Bool		SetPos( const Point& pt );
			Bool		SetSize( const Size& sz );
			Bool		SetRect( const Rect& rc );

		private:
			CWindow();
			virtual ~CWindow();
			SINGLETON( CWindow );
		};
	}
}