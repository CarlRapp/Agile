#include "DXWindow.h"
#include <ctime> 
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
DXWindow::DXWindow(void)
{
	m_X = 0;
	m_Y = 0;
	m_Width = 0;
	m_Height = 0;
}

DXWindow::~DXWindow(void)
{

}


bool DXWindow::InitWindow(int _X, int _Y, int _Width, int _Height)
{
	m_X = _X;
	m_Y = _Y;
	m_Width = _Width;
	m_Height = _Height;
	return InitWnd(GetModuleHandle(0), NULL, GetCommandLine(), SW_SHOW);
}



bool DXWindow::InitWnd(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _pCmdLine, int _nShowCmd)
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

	if (!InitWndApp(_hInstance, _nShowCmd))
	{
		::MessageBox(0, "Initalaization Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

bool DXWindow::InitWndApp(HINSTANCE _hInstanceHandle, int _show)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstanceHandle;
	wc.hIcon = ::LoadIconA(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = "Window";

	if (!RegisterClass(&wc))
	{
		::MessageBox(0, "Failed to register WNDCLASS", 0, MB_OK);
		return false;
	}

	DWORD	wStyle = WS_POPUP;

	RECT rc = { 0, 0, m_Width, m_Height };
	AdjustWindowRect(&rc, wStyle, FALSE);

	m_hWndMain = ::CreateWindow("Window",
		"Breakout",
		wStyle,
		m_X,
		m_Y,
		rc.right - rc.left,
		rc.bottom - rc.top,
		0,
		0,
		_hInstanceHandle,
		0);

	if (m_hWndMain == 0)
	{
		::MessageBox(0, "Failed to create WNDCLASS", 0, MB_OK);
		return false;
	}


	::ShowWindow(m_hWndMain, _show);
	::UpdateWindow(m_hWndMain);

	return true;
}


