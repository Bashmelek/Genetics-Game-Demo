#pragma once

#include "resource.h"
#include "stdafx.h"
#include "GameViewState.h"

#ifndef GAMEWINDEF
#define GAMEWINDEF


template <class GAME_WINDOW>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		GAME_WINDOW *pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (GAME_WINDOW*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (GAME_WINDOW*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	BaseWindow() : m_hwnd(NULL), _viewState(std::make_unique<GameViewState>()) { }

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	)
	{
		WNDCLASSEXW wcex = { 0 };

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = GAME_WINDOW::WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
		wcex.lpszClassName = ClassName();
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);

		////_gameState = new GameState();
		/*
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = GAME_WINDOW::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);*/

		m_hwnd = CreateClassWindow(hWndParent, hMenu, wcex.hInstance);
			//CreateWindowEx(0, ClassName(), L"Welcome",
			//(/*WS_OVERLAPPED | \*/
			// /*WS_CAPTION | \*/
			//	WS_SYSMENU | \
			//	WS_MINIMIZEBOX | WS_CLIPCHILDREN
			//	/*WS_THICKFRAME*/),
			//100, 200, WINDOWLENGTH, WINDOWHEIGHT, hWndParent, hMenu, wcex.hInstance, this);
			/*CreateWindowEx(
																						   dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
																						   nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
																						   );*/
		m_hInst = wcex.hInstance;

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND Window() const { return m_hwnd; }

protected:

	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	virtual HWND CreateClassWindow(HWND hWndParent, HMENU hMenu, HINSTANCE hInstance) = 0;

	HWND m_hwnd;
	HINSTANCE m_hInst;
	std::unique_ptr<GameViewState> _viewState;// GameState* _gameState;
};


class MainWindow : public BaseWindow<MainWindow>
{
public:
	PCWSTR  ClassName() const;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	////INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	HWND CreateClassWindow(HWND hWndParent, HMENU hMenu, HINSTANCE hInstance)
	{
		return CreateWindowEx(0, ClassName(), L"Welcome",
			(/*WS_OVERLAPPED | \*/
			 /*WS_CAPTION | \*/
				WS_SYSMENU | \
				WS_MINIMIZEBOX | WS_CLIPCHILDREN
				/*WS_THICKFRAME*/),
			100, 20, WINDOWLENGTH, WINDOWHEIGHT, hWndParent, hMenu, hInstance, this);
	}
};

//#include "MainWindow.cpp"

#endif GAMEWINDEF

