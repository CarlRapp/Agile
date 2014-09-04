#pragma once
#ifndef DXWINDOW_H
#define DXWINDOW_H
#include <windows.h>
#include <D3D11.h>
#include <d3dCompiler.h>

class DXWindow
{
private:

	HWND					g_hWndMain = NULL;


	bool				InitWnd(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd);
	bool				InitWndApp(HINSTANCE hInstanceHandle, int show);
	LRESULT CALLBACK	WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int					Run();
	HRESULT				Render(float deltaTime);
	HRESULT				Update(float deltaTime);
	HRESULT				InitDirect3D();
	char*				FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel);


	int m_Width, m_Height;

public:

	DXWindow(void);
	~DXWindow(void);

	bool  InitWindow(int width, int height);

	
};



#endif