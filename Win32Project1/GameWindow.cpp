#include "stdafx.h"
#include "resource.h"
////#include "GameState.h"
#include "Win32Project1.h"
////#include "SubWindows.h"



//class MainWindow : public BaseWindow<MainWindow>
//{
//public:
//	PCWSTR  ClassName() const { return L"Sample Window Class"; }
//	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
//};

PCWSTR  MainWindow::ClassName() const { return L"Sample Window Class"; }



INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LRESULT MainWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		(*_viewState).InitIntroScreen(m_hwnd);
		(*_viewState).mainWindow = Window();
		SetTimer(m_hwnd, 1, 0, NULL);//120 good for demo
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case BUTTON_STARTNEWGAME:
		case BUTTON_EXITGAME:
		case BUTTON_NEXTTURN:
		case BUTTON_MESSAGETAB:
		case BUTTON_ALLELESTAB:
			(*_viewState).HandleButtonInput(wmId);
			break;
		case IDM_ABOUT:
			DialogBox(m_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), m_hwnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(m_hwnd);
			break;
		default:
			return DefWindowProc(m_hwnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DRAWITEM:
	{
		////(*_viewState).DrawNextFrame(m_hwnd);
		break;
	}
	case WM_ERASEBKGND:
	{
		break;
	}
	case WM_TIMER:
	{
		//int xStart = (*GameViewState::TheGameView()).mapViewX;
		(*_viewState).DrawNextFrame(m_hwnd);
		//here is the rough autorun logic, spoofing commands...must clean up
		if ((*_viewState).IsOnAutoTime)
		{
			(*_viewState).autoTimeProgress += 1;
			if ((*_viewState).autoTimeProgress % 7 == 0)
			{
				(*_viewState).autoTimeProgress = 0;
				(*_viewState).HandleButtonInput(BUTTON_NEXTTURN);
			}
		}
		break;
	}
	case WM_PAINT:
	{
		(*_viewState).PaintScene(m_hwnd);
	}
	break;
	case WM_CLOSE:
		if (MessageBox(m_hwnd, L"In love with Shan?", L"Important notice", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(m_hwnd);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		(*_viewState).mouseJustClicked = true;
		DefWindowProc(m_hwnd, message, wParam, lParam);
		break;
	default:
		return DefWindowProc(m_hwnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



///template class BaseWindow<MainWindow>;
void TemporaryFunction()
{
	MainWindow TempObj;
	TempObj.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW);
	ShowWindow(TempObj.Window(), 0);
}


