#include "DXWindow.h"
#include <ctime> 
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
DXWindow::DXWindow(void)
{
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;
}

DXWindow::~DXWindow(void)
{

}


bool DXWindow::InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode)
{
	m_x = _x;
	m_y = _y;
	m_width = _width;
	m_height = _height;
	return InitWnd(GetModuleHandle(0), NULL, GetCommandLine(), SW_SHOW, _displayMode);
}



bool DXWindow::InitWnd(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _pCmdLine, int _nShowCmd, DisplayMode _displayMode)
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

	if (!InitWndApp(_hInstance, _nShowCmd, _displayMode))
	{
		::MessageBox(0, "Initalaization Failed", "Error", MB_OK);
		return false;
	}

	return true;
}

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_CLOSE:
		DestroyWindow(_hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	case WM_MOVE:
		break;
	default:
		return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
	}
	return 0;
}

bool DXWindow::InitWndApp(HINSTANCE _hInstanceHandle, int _show, DisplayMode _displayMode)
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

	DWORD	wStyle;
	if (_displayMode == DisplayMode::Windowed)
		wStyle = WS_BORDER;
	else
	{
		wStyle = WS_POPUP;
	}

	RECT rc = { 0, 0, m_width, m_height };
	AdjustWindowRect(&rc, wStyle, FALSE);

	m_hWndMain = ::CreateWindow("Window",
		"Breakout",
		wStyle,
		m_x,
		m_y,
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


void DXWindow::Update()
{
	// TEMP
	RECT rc;
	rc.left = 100;
	rc.right = rc.left + 1000;
	rc.top = 350;
	rc.bottom = rc.top + 600;

	ClipCursor(&rc);
	ShowCursor(true);

	MSG msg;
	if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

