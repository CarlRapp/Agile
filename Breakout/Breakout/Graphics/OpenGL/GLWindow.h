#pragma once
#ifndef GLWINDOW_H
#define GLWINDOW_H

class GLWindow
{
private:

	//HWND					g_hWndMain = NULL;


	bool				InitWnd();
	bool				InitWndApp();
	int				Run();
	int				Render(float deltaTime);
	int				Update(float deltaTime);
	int				InitDirect3D();
	char*				FeatureLevelToString();


	int m_Width, m_Height;

public:

	GLWindow(void);
	~GLWindow(void);

	bool  InitWindow(int width, int height);

	
};



#endif