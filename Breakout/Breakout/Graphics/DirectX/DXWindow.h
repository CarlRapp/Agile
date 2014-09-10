#pragma once
#ifndef DXWINDOW_H
#define DXWINDOW_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>
#include "../IGraphics.h"

class DXWindow
{
private:

	HWND				m_hWndMain = NULL;
	int					m_X, m_Y, m_Width, m_Height;


	bool				InitWnd(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd, DisplayMode _displayMode);
	bool				InitWndApp(HINSTANCE hInstanceHandle, int show, DisplayMode _displayMode);
	int					Run();
	HRESULT				Render(float deltaTime);
	HRESULT				Update(float deltaTime);
	HRESULT				InitDirect3D();
	char*				FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel);


public:

	DXWindow(void);
	~DXWindow(void);

	bool  InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode);

	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }


	HWND GetHandle(){ return m_hWndMain; }
};



#endif