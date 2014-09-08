
#include "GLWindow.h"
#include <ctime> 
#include <iostream>
#include <stdio.h>
//#include <io.h>
#include <fcntl.h>

GLWindow::GLWindow(void)
{
	m_Width = 0;
	m_Height = 0;
}

GLWindow::~GLWindow(void)
{

}


bool GLWindow::InitWindow(int width, int height)
{
	m_Width = width;
	m_Height = height;
	return InitWnd();
}



bool GLWindow::InitWnd()
{
//	AllocConsole();
//
//	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
//	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
//	FILE* hf_out = _fdopen(hCrt, "w");
//	setvbuf(hf_out, NULL, _IONBF, 1);
//	*stdout = *hf_out;
//
//	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
//	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
//	FILE* hf_in = _fdopen(hCrt, "r");
//	setvbuf(hf_in, NULL, _IONBF, 128);
//	*stdin = *hf_in;
//
//	if (!InitWndApp(hInstance, nShowCmd))
//	{
//		::MessageBox(0, "Initalaization Failed", "Error", MB_OK);
//		return 0;
//	}

	return 1;
}

int WndProc()
{
//	switch (msg)
//	{
//	case WM_CLOSE:
//		DestroyWindow(hwnd);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hwnd, msg, wParam, lParam);
//	}
	return 0;
}

bool GLWindow::InitWndApp()
{
//	WNDCLASS wc;
//	wc.style = CS_HREDRAW | CS_VREDRAW;
//	//wc.lpfnWndProc = WndProc;
//	wc.lpfnWndProc = WndProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = 0;
//	wc.hInstance = hInstanceHandle;
//	wc.hIcon = ::LoadIconA(0, IDI_APPLICATION);
//	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
//	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
//	wc.lpszMenuName = 0;
//	wc.lpszClassName = "Window";
//
//	if (!RegisterClass(&wc))
//	{
//		::MessageBox(0, "Failed to register WNDCLASS", 0, MB_OK);
//		return 0;
//	}
//
//	DWORD	wStyle = WS_POPUP;
//
//	RECT rc = { 0, 0, m_Width, m_Height };
//	AdjustWindowRect(&rc, wStyle, FALSE);
//
//	g_hWndMain = ::CreateWindow("Window",
//		"Breakout",
//		wStyle,
//		CW_USEDEFAULT,
//		CW_USEDEFAULT,
//		rc.right - rc.left,
//		rc.bottom - rc.top,
//		0,
//		0,
//		hInstanceHandle,
//		0);
//
//	if (g_hWndMain == 0)
//	{
//		::MessageBox(0, "Failed to create WNDCLASS", 0, MB_OK);
//		return 0;
//	}
//
//
//	::ShowWindow(g_hWndMain, show);
//	::UpdateWindow(g_hWndMain);

	return true;
}


