#pragma once

////#include <list>
////#include <memory>
#include "resource.h"
#include "windowsx.h"
#include "commctrl.h"
#include "Windows.h"

#ifndef SUBWINDOWSDEF
#define SUBWINDOWSDEF



#define WINDOWLENGTH 1400
#define WINDOWHEIGHT 1000
#define SIDEBARWIDTH 280
#define TOPBARHEIGHT 40
#define DETAILSECTIONHEIGHT 290
#define DETAILTABSSECTIONHEIGHT 35

#define BUTTON_STARTNEWGAME 100
#define BUTTON_EXITGAME 101
#define BUTTON_NEXTTURN 102
#define BUTTON_MESSAGETAB 103
//104 is idm_about
//105 is idm_exit
#define BUTTON_ALLELESTAB 106



template <class SUB_WINDOW>
class SubWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SUB_WINDOW *pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (SUB_WINDOW*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (SUB_WINDOW*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
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

	SubWindow() : m_hwnd(NULL) { }

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
		wcex.lpfnWndProc = SUB_WINDOW::WindowProc;
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
};



class GameInfoWindow : public SubWindow<GameInfoWindow>
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
				WS_VISIBLE | WS_CHILD //| WS_VSCROLL| WS_CLIPCHILDREN ////| SS_OWNERDRAW
				/*WS_THICKFRAME*/),
			SIDEBARWIDTH, WINDOWHEIGHT - DETAILSECTIONHEIGHT, WINDOWLENGTH - 2 * SIDEBARWIDTH, DETAILSECTIONHEIGHT, hWndParent, hMenu, hInstance, this);
			//250, WINDOWHEIGHT - 290, WINDOWLENGTH - 500, 200, hWndParent, hMenu, hInstance, this);
			//WINDOWHEIGHT - 200, WINDOWLENGTH - 250, WINDOWLENGTH - 500, 200, hWndParent, hMenu, hInstance, this);
	}

	static WNDPROC OriginalScrollbarProc;

	static LRESULT CALLBACK OwnerDrawScrollBarProc(
		_In_ HWND   hwnd,
		_In_ UINT   uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		switch (uMsg)
		{
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			// TODO: Add any drawing code here...
			EndPaint(hwnd, &ps);
			break;
		case WM_NCPAINT:
			hdc = BeginPaint(hwnd, &ps);
			// TODO: Add any drawing code here...
			EndPaint(hwnd, &ps);
			break;
		case WM_DRAWITEM:
			break;/*
		case WM_CAPTURECHANGED:
			break;
			case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			break;
		case WM_CANCELMODE:
			break;
		case WM_MOUSEACTIVATE:
			break;*/
		//case WM_LBUTTONDBLCLK: break;
		////draw cases = 31, 33, 513, 533...
		////uMsg != 32 && uMsg != 512 && uMsg != 132 && uMsg != 522
		default:
			LRESULT res = CallWindowProc(
				GameInfoWindow::OriginalScrollbarProc,
				hwnd, uMsg, wParam, lParam
			);
			HDC otherDC = GetDC(hwnd);
			RECT buddyrect = { 0, 0, 40, 200 };
			HBRUSH brush2 = CreateSolidBrush(RGB(100, 100, 250));
			FillRect(otherDC, &buddyrect, brush2);
			DeleteObject(brush2);
			ReleaseDC(hwnd, otherDC);

			return res;
			////DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	}
};


class GameInfoTabsWindow : public SubWindow<GameInfoTabsWindow>
{
public:
	PCWSTR  ClassName() const;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND CreateClassWindow(HWND hWndParent, HMENU hMenu, HINSTANCE hInstance)
	{
		return CreateWindowEx(0, ClassName(), L"Welcome",
			(WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN),
			SIDEBARWIDTH, WINDOWHEIGHT - DETAILSECTIONHEIGHT - DETAILTABSSECTIONHEIGHT, WINDOWLENGTH - 2 * SIDEBARWIDTH, DETAILTABSSECTIONHEIGHT, hWndParent, hMenu, hInstance, this);
	}

	void InitInternals();

protected:
	HWND messagesButtonWindow;
	HWND allelesButtonWindow;
};


#endif SUBWINDOWSDEF
