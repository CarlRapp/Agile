#pragma once
#ifndef GLWINDOW_H
#define GLWINDOW_H
#include <SDL/SDL_video.h>

class GLWindow
{
private:

	//HWND					g_hWndMain = NULL;


	bool				InitWnd();
	bool				InitWndApp();
	int				Run();
	int				Render(float deltaTime);
	int				Update(float deltaTime);
	char*				FeatureLevelToString();
        SDL_Surface*                    m_surface;

	int m_width, m_height;

public:

	GLWindow(void);
	~GLWindow(void);

	bool InitWindow(int width, int height);
        bool InitGL();
        SDL_Surface* GetSurface();
	
};



#endif