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
	int					m_x, m_y, m_width, m_height;


	bool				InitWnd(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _pCmdLine, int _nShowCmd, DisplayMode _displayMode);
	bool				InitWndApp(HINSTANCE _hInstanceHandle, int show, DisplayMode _displayMode);
	int					Run();
	HRESULT				Render(float _deltaTime);
	HRESULT				Update(float _deltaTime);
	HRESULT				InitDirect3D();
	char*				FeatureLevelToString(D3D_FEATURE_LEVEL _featureLevel);


public:

	DXWindow(void);
	~DXWindow(void);

	bool  InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode);

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }


	HWND GetHandle(){ return m_hWndMain; }
};



#endif