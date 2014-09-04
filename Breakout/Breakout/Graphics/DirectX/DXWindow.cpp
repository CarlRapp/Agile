#include "DXWindow.h"
#include <ctime> 
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
DXWindow::DXWindow(void)
{
	m_Width = 0;
	m_Height = 0;
}

DXWindow::~DXWindow(void)
{

}


bool DXWindow::InitWindow(int width, int height)
{
	m_Width = width;
	m_Height = height;
	return InitWnd(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW);
}



bool DXWindow::InitWnd(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;

	if (!InitWndApp(hInstance, nShowCmd))
	{
		::MessageBox(0, "Initalaization Failed", "Error", MB_OK);
		return 0;
	}

	return 1;
}

bool DXWindow::InitWndApp(HINSTANCE hInstanceHandle, int show)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.lpfnWndProc = WndProc;
	wc.lpfnWndProc = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstanceHandle;
	wc.hIcon = ::LoadIconA(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = "Window";

	if (!RegisterClass(&wc))
	{
		::MessageBox(0, "Failed to register WNDCLASS", 0, MB_OK);
		return 0;
	}

	DWORD	wStyle = WS_POPUP;

	RECT rc = { 0, 0, m_Width, m_Height };
	AdjustWindowRect(&rc, wStyle, FALSE);

	g_hWndMain = ::CreateWindow("Window",
		"Breakout",
		wStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		0,
		0,
		hInstanceHandle,
		0);

	if (g_hWndMain == 0)
	{
		::MessageBox(0, "Failed to create WNDCLASS", 0, MB_OK);
		return 0;
	}


	::ShowWindow(g_hWndMain, show);
	::UpdateWindow(g_hWndMain);

	return true;
}

LRESULT CALLBACK DXWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
			//case VK_ESCAPE:					//Avslutar programmet om man trycker på escape.
			//CloseApplication();
			//::DestroyWindow(hWnd);
		}
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
