#include "stdafx.h"
#include "SubWindows.h"



PCWSTR  GameInfoTabsWindow::ClassName() const { return L"Game Info Tabs Window Class"; }

LRESULT GameInfoTabsWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case BUTTON_MESSAGETAB:
		case BUTTON_ALLELESTAB:
			SendMessage( GetParent((*this).Window()), WM_COMMAND, wParam, NULL);
			break;
		default:
			return DefWindowProc(m_hwnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DRAWITEM:
	{
		break;
	}
	case WM_VSCROLL:
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(Window(), &ps);
		// TODO: Add any drawing code here...
		EndPaint(Window(), &ps);
		break;
	}
	default:
		return DefWindowProc(m_hwnd, message, wParam, lParam);
	}

	return 0;
}

void GameInfoTabsWindow::InitInternals()
{
	messagesButtonWindow = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("History"),
		WS_VISIBLE | WS_CHILD,// | BS_OWNERDRAW,// | BS_NOTIFY,
		10, 5, 80, 25,
		(*this).Window(), (HMENU)BUTTON_MESSAGETAB, NULL, NULL);

	allelesButtonWindow = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Alleles"),
		WS_VISIBLE | WS_CHILD,// | BS_OWNERDRAW,// | BS_NOTIFY,
		100, 5, 80, 25,
		(*this).Window(), (HMENU)BUTTON_ALLELESTAB, NULL, NULL);
}




PCWSTR  GameInfoWindow::ClassName() const { return L"Game Message Window Class"; }

WNDPROC GameInfoWindow::OriginalScrollbarProc = 0;
//LRESULT CALLBACK CustomScrollbarProc(HWND hWnd, UINT msg,
//	WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass,
//	DWORD_PTR dwRefData)
//{
//	switch (msg)
//		{
//		case WM_PAINT:
//			PAINTSTRUCT ps;
//			HDC hdc;
//			hdc = BeginPaint(hWnd, &ps);
//				// TODO: Add any drawing code here...
//			EndPaint(hWnd, &ps);
//			break;
//		default:
//			return DefSubclassProc(hWnd, msg, wParam, lParam);//// DefWindowProc(hwnd, uMsg, wParam, lParam);
//		}
//		return TRUE;
//}


LRESULT GameInfoWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		//HWND scrollbar = (CreateWindowEx(
		//	0,                      // no extended styles 
		//	L"SCROLLBAR",           // scroll bar control class 
		//	(PTSTR)NULL,           // no window text 
		//	WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_RADIOBUTTON //| BS_AUTOCHECKBOX // window styles  
		//	| SBS_VERT,         // horizontal scroll bar style 
		//	0,              // horizontal position 
		//	0, // vertical position 
		//	40,             // width of the scroll bar 
		//	200,               // height of the scroll bar
		//	Window(),             // handle to main window 
		//	(HMENU)NULL,           // no menu 
		//	m_hInst,                // instance owning this window 
		//	(PVOID)NULL            // pointer not needed 
		//));
		////GameMessagesWindow::OriginalScrollbarProc =
		//OriginalScrollbarProc =
		/*GameMessagesWindow::OriginalScrollbarProc =
		(WNDPROC)SetWindowLongPtr(scrollbar,
			GWLP_WNDPROC, (LONG_PTR)GameMessagesWindow::OwnerDrawScrollBarProc);*/
		//SetWindowSubclass(scrollbar, CustomScrollbarProc, 0, 0);
		break;
	}
	case WM_DRAWITEM:
	{
		////(*_viewState).DrawNextFrame(m_hwnd);
		break;
	}
	case WM_VSCROLL:
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(Window(), &ps);
		// TODO: Add any drawing code here...
		EndPaint(Window(), &ps);
		break;
	}
	default:
		return DefWindowProc(m_hwnd, message, wParam, lParam);
	}

	return 0;
}
