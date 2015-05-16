#ifndef GLWINDOW_H_INCLUDED
#define GLWINDOW_H_INCLUDED

#include <SDL/SDL.h>
#include "utils/settings.h"
#include "common/be_event_system.h"
#include "kernel/be_log.h"
#include "gui/canvas.h"

class GLWindow {

	public:
		GLWindow(BeEventSystem* eventsystem);
		virtual ~GLWindow();

		void create(const std::string& title);
		void createSafe(const std::string& title);

		//	Destroy window and OpenGL Context, close the Display
// 		void destroy();
		void process();

		void setTitle(const std::string& title);
		void setCanvas(boost::shared_ptr<Canvas> canvas) { m_canvas = canvas; }

        void dumpScreenShot();
		
		int width() { return w_width; }
		int height() { return w_height; }

		bool resized() const { return m_resized; }
		
	private:
		//	Resize Window
		BeEventSystem* m_eventsystem;
		void resize();
		void toggleFs();
		unsigned int w_bpp;		// Bits Per Pixel. With XFree86, highest = 24
		int w_width;
		int w_height;
		int n_width;
		int n_height;
		int fs;
		const SDL_VideoInfo* vidInfo;
		SDL_Surface* m_surface;
		int vidFlags;
		bool hwaccel;
// 		const int* settingsfs;
		const int* m_window_fullscreen_p;
		
		int mousex;
		int mousey;
		BeLogDebug m_logDebug;
		boost::shared_ptr<Canvas> m_canvas;
		bool m_resized;
		bool m_resizable;
};

#endif	// GLWINDOW_H
