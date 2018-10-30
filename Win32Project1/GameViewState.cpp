#include "stdafx.h"
#include "GameViewState.h"

GameViewState::GameViewState()
{
	_gameState = std::make_unique<GameState>();
	mapViewX = 0;
	mapViewY = 0;
	mapWidth = 2400;
	mapHeight = 1800;

	manaBarImage = (HBITMAP)LoadImage(
		NULL,
		L"manabar.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	manaBarCoverMask = (HBITMAP)LoadImage(
		NULL,
		L"manabar1.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	manaBarMask = (HBITMAP)LoadImage(
		NULL,
		L"manabarmask.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	manaBarAntiMask = (HBITMAP)LoadImage(
		NULL,
		L"manabarmask2.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	manaBarFillImage = (HBITMAP)LoadImage(
		NULL,
		L"manabarfillcoolheart.bmp",////\\GameResources//manabarfillcool
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	manaBarBackgroudImage = (HBITMAP)LoadImage(
		NULL,
		L"manabarbg.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);

	manaBarFillFrame = 0;
}

GameViewState* GameViewState::TheGameView()
{
	static GameViewState theViewState = GameViewState();
	return &theViewState;
}

void GameViewState::InitIntroScreen(HWND hwnd)
{
	//_gameMode = MAINMENU;
	CreateWindowEx(NULL, TEXT("button"), TEXT("Marry Shan"),
		WS_VISIBLE | WS_CHILD,
		120, 50, 80, 25,
		hwnd, (HMENU)BUTTON_STARTNEWGAME, NULL, NULL);
	CreateWindowEx(NULL, TEXT("button"), TEXT("Get a Ring"),
		WS_VISIBLE | WS_CHILD,
		120, 150, 80, 25,
		hwnd, (HMENU)BUTTON_EXITGAME, NULL, NULL);
}

BOOL CALLBACK CleanWindow(HWND hwnd, LPARAM lParam)
{
	DestroyWindow(hwnd);
	return true;
}

void GameViewState::HandleButtonInput(int buttonID)
{
	switch (buttonID)
	{
	case BUTTON_STARTNEWGAME:
		(*_gameState).StartNewGame(mainWindow);

		EnumChildWindows(
			mainWindow,
			CleanWindow,
			NULL);

		nextTurnButtonWindow = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Next Turn"),
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_NOTIFY,
			WINDOWLENGTH - 200, 5, 80, 25,
			mainWindow, (HMENU)BUTTON_NEXTTURN, NULL, NULL);

		_gameMessagesWindow = std::make_unique<GameMessagesWindow>();
		(*_gameMessagesWindow).Create(L"Nothing to see here", WS_OVERLAPPEDWINDOW, 
				0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, mainWindow,0);

		break;
	case BUTTON_EXITGAME:
		if (MessageBox(mainWindow, L"Really quit?", L"Important notice", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(mainWindow);
		}
		break;
	case BUTTON_NEXTTURN:
		//if (MessageBox(mainWindow, L"It lives!", L"Important notice", MB_OKCANCEL) == IDOK)
		//{
			//DestroyWindow(mainWindow);
		//}
		(*_gameState).NextTurn();
		break;
	default:
		break;
	}
}


//typically only called by the timer
void GameViewState::DrawNextFrame(HWND hwnd)
{
	if ((*_gameState).GetGameMode() == INGAME)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = GetDC(hwnd);// , NULL, DCX_WINDOW);
						  //ps.fErase = 1;
						  //SelectObject(ps.hdc, GetStockObject(BLACK_BRUSH));
						  ////ps.rcPaint = { 0, 0, 700, 400 };
		GetClientRect(hwnd, &ps.rcPaint);
		DrawGameScene(hdc, ps);
		ReleaseDC(hwnd, hdc);
	}
}

void GameViewState::DrawGameScene(HDC hdc, PAINTSTRUCT ps)
{
	deviceContext = hdc;
	memoryDC = CreateCompatibleDC(deviceContext);
	//HDC memoryDC = (*GameViewState::TheGameView()).memoryDC;

	memoryBMP = (HBITMAP)CreateCompatibleBitmap(hdc, ps.rcPaint.right, ps.rcPaint.bottom);
	HBITMAP screenBMP = memoryBMP;
	HBITMAP selectedBMP = (HBITMAP)SelectObject(memoryDC, screenBMP);
	//(*GameViewState::TheGameView()).memoryBMP = (HBITMAP)CreateCompatibleBitmap(memoryDC, ps.rcPaint.right, ps.rcPaint.bottom);
	//HBITMAP screenBMP = (*GameViewState::TheGameView()).memoryBMP;
	gameViewArea = { SIDEBARWIDTH, 40,  WINDOWLENGTH - SIDEBARWIDTH, ps.rcPaint.bottom - DETAILSECTIONHEIGHT + 35};
	ProcessMousePosition();

	//do the cool game center area
	HBRUSH brush2 = CreateSolidBrush(RGB(25, 25, 25));
	RECT gameRect = { SIDEBARWIDTH, 0, WINDOWLENGTH - SIDEBARWIDTH, ps.rcPaint.bottom - DETAILSECTIONHEIGHT + 35};
	FillRect(memoryDC, &gameRect, brush2);//(RGB(250, 250, 250)));
	DeleteObject(brush2);

	AnimateWorldFrame();
	AnimateUI(memoryDC, &ps);

	BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, memoryDC, 0, 0, SRCCOPY);

	DeleteObject(selectedBMP);
	DeleteObject(screenBMP);
	DeleteDC(memoryDC);
}

void GameViewState::AnimateUI(HDC memdc, PAINTSTRUCT* paintstruct)
{
	HDC hdc = deviceContext;

	PAINTSTRUCT ps = (*paintstruct);

	RECT player1Rect = { WINDOWLENGTH - SIDEBARWIDTH, 0,  ps.rcPaint.right, ps.rcPaint.bottom };
	RECT enemyRect = { 0, 0,  SIDEBARWIDTH,  ps.rcPaint.bottom };
	HBRUSH brush1 = CreateSolidBrush(RGB(0, 32, 61));//RGB(0, 32, 61));
	FillRect(memoryDC, &enemyRect, brush1);
	FillRect(memoryDC, &player1Rect, brush1);
	DeleteObject(brush1);

	RECT TopBarRect = { 0, 0, ps.rcPaint.right, 40 };// ps.rcPaint.bottom / 12};

	HBRUSH interfaceBrush = CreateSolidBrush(RGB(220, 220, 50));
	FillRect(memdc, &TopBarRect, interfaceBrush);
	DeleteObject(interfaceBrush);

	HDC tempDC = CreateCompatibleDC(memdc);
	HBITMAP saveTempDCImage = (HBITMAP)SelectObject(tempDC, manaBarMask);
	BitBlt(memdc, ps.rcPaint.right * 0.75, 5, 150, 30, tempDC, 0, 0, SRCPAINT);

	HDC containerDC = CreateCompatibleDC(memdc);
	HBITMAP containerBmp = (HBITMAP)SelectObject(containerDC, manaBarBackgroudImage);

	SelectObject(tempDC, manaBarFillImage);
	BitBlt(containerDC, 0, 0, 150.0 * (*(*_gameState).player1).CurrentEnergy() / (*(*_gameState).player1).MaxEnergy(), 30, tempDC, 0, manaBarFillFrame, SRCCOPY);
	manaBarFillFrame = (manaBarFillFrame + 1) % 120;

	SelectObject(tempDC, manaBarAntiMask);
	BitBlt(containerDC, 0, 0, 150, 30, tempDC, 0, 0, SRCPAINT);
	SelectObject(tempDC, manaBarCoverMask);
	BitBlt(containerDC, 0, 0, 150, 30, tempDC, 0, 0, SRCPAINT);

	SelectObject(tempDC, manaBarImage);
	BitBlt(containerDC, 0, 0, 150, 30, tempDC, 0, 0, SRCAND);

	BitBlt(memdc, ps.rcPaint.right * 0.75, 5, 150, 30, containerDC, 0, 0, SRCAND);

	SelectObject(containerDC, containerBmp);
	SelectObject(tempDC, saveTempDCImage);
	DeleteDC(tempDC);
	DeleteDC(containerDC);

	AnimateChildUIWindows();
}

//BOOL CALLBACK blahhgalah(_In_ HWND   hwnd,
//	_In_ LPARAM lParam)
//{
//	int i = 0;
//	i++;
//	return true;
//}

void GameViewState::AnimateChildUIWindows()
{
	/*EnumChildWindows(
		mainWindow,
		blahhgalah,
		0L
	);*/
	////_gameMessagesWindow
	HDC otherDC = GetDC((*_gameMessagesWindow).Window());
	HDC uiMemoryDC = CreateCompatibleDC(otherDC);
	HBITMAP uiMemoryBMP = (HBITMAP)CreateCompatibleBitmap(otherDC, WINDOWLENGTH - 2 * SIDEBARWIDTH, 700);
	HBITMAP uiOriginalBMP = (HBITMAP)SelectObject(uiMemoryDC, uiMemoryBMP);
	//HBITMAP saveTempDCImage = (HBITMAP)SelectObject(uiMemoryDC, manaBarMask);
	RECT buddyrect = { 5, 5, WINDOWLENGTH - 2 * SIDEBARWIDTH, 700 };
	HBRUSH brush2 = CreateSolidBrush(RGB(200, 200, 200));
	FillRect(uiMemoryDC, &buddyrect, brush2);
	std::list<std::wstring>::reverse_iterator it;
	////int messageCount = 0;
	RECT messageRect = { 5, 5, WINDOWLENGTH - 2 * SIDEBARWIDTH, 700 };
	for (it = (*_gameState).gameMessages.rbegin(); it != (*_gameState).gameMessages.rend(); it++) {
		DrawText(uiMemoryDC, (*it).c_str(), -1, &messageRect, NULL);
		messageRect.top += 20;
		messageRect.bottom += 20;
	}
	BitBlt(otherDC, 0, 0, WINDOWLENGTH - 2 * SIDEBARWIDTH, 700, uiMemoryDC, 0, 0, SRCCOPY);
	DeleteObject(brush2);
	ReleaseDC((*_gameMessagesWindow).Window(), otherDC);


	HDC nextTurnButtonDC = GetDC(nextTurnButtonWindow);
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(nextTurnButtonWindow, &mousePosition);
	RECT buttRect = { 5, 5, 80, 25 };
	int buttonState = Button_GetState(nextTurnButtonWindow);
	HBRUSH brush1;
	if (buttonState & BST_PUSHED)
	{
		brush1 = CreateSolidBrush(RGB(0, 200, 0));

		/*if (buttonState != 108 && buttonState != 8 && buttonState != 104)
		{
			buttRect = { 0, 0, 80, 25 };
		}*/
	}
	else
	{
		if (mousePosition.x > 0 && mousePosition.x < 80 && 
			mousePosition.y > 0 && mousePosition.y < 25)
		{
			brush1 = CreateSolidBrush(RGB(200, 0, 0));
		}
		else
		{
			brush1 = CreateSolidBrush(RGB(0, 0, 200));
		}
	}
	SelectObject(uiMemoryDC, uiOriginalBMP);
	FillRect(nextTurnButtonDC, &buttRect, brush1);
	DeleteObject(brush1);
	DeleteObject(uiMemoryBMP);
	DeleteDC(uiMemoryDC);
	ReleaseDC(nextTurnButtonWindow, nextTurnButtonDC);
}

void GameViewState::AnimateWorldFrame()
{
	std::list<std::unique_ptr<Denizen>>::iterator i;
	for (i = (*_gameState).theLiving.begin(); i != (*_gameState).theLiving.end(); i++)
	{
		DrawObject( &(*(*i)) );
	}
}


void GameViewState::PaintScene(HWND hwnd)
{
	//PAINTSTRUCT ps;
	//HDC hdc = BeginPaint(m_hwnd, &ps);
	//// TODO: Add any drawing code that uses hdc here...
	//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BTNSHADOW));
	//HBRUSH* innerBrush = new HBRUSH();
	//EndPaint(m_hwnd, &ps);
	PAINTSTRUCT ps;
	HDC hdc;

	switch ((*_gameState).GetGameMode())
	{
	case(MAINMENU):
		hdc = BeginPaint(hwnd, &ps);
		//SelectObject(ps.hdc, GetStockObject(DKGRAY_BRUSH));

		//Rectangle(ps.hdc, 5, 5,//rcTarget.left, rcTarget.top,
		//	700, 400);// rcTarget.right, rcTarget.bottom);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BTNSHADOW + 1));
		EndPaint(hwnd, &ps);
		break;
	case(INGAME):

		hdc = BeginPaint(hwnd, &ps);
		//hdc = GetDC(hwnd);// , NULL, DCX_WINDOW);
		//ps.fErase = 1;
		//SelectObject(ps.hdc, GetStockObject(BLACK_BRUSH));
		////FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(50, 200, 200)));
		//Rectangle(ps.hdc, 5, 5,//rcTarget.left, rcTarget.top,
		//	700, 400);// rcTarget.right, rcTarget.bottom);
		GetClientRect(hwnd, &ps.rcPaint);
		DrawGameScene(hdc, ps);
		EndPaint(hwnd, &ps);
		ReleaseDC(hwnd, hdc);
		break;
	default:
		break;
	}
}

void GameViewState::DrawObject(Denizen* gameObject)
{
	HDC hdc = deviceContext;
	//HDC memdc = CreateCompatibleDC(hdc);
	HDC memdc = memoryDC;

	HDC tempDC = CreateCompatibleDC(memdc);// (hdc);
										   ////HDC tempDC2 = CreateCompatibleDC(memdc);
										   //HBITMAP saveOldBitmap = (HBITMAP)SelectObject(memdc, denizenImageBase);
	int screenLocX = (*gameObject).worldX * 50 + 200 - mapViewX;
	int screenLocY = (*gameObject).worldY * 50 + 50 - mapViewY;
	HBITMAP saveOldTempBmp = (HBITMAP)SelectObject(tempDC, (*gameObject).imageMask);
	BitBlt(memdc, screenLocX, screenLocY, 50, 50, tempDC, ((*gameObject).currentFrame * 50), 0, SRCPAINT);// SRCAND);
	if ((*gameObject).gender == male)
	{
		SelectObject(tempDC, (*gameObject).imageBase);
	}
	else
	{
		SelectObject(tempDC, (*gameObject).ladyImageBase);
	}
	BitBlt(memdc, screenLocX, screenLocY, 50, 50, tempDC, ((*gameObject).currentFrame * 50), 0, SRCAND);

	(*gameObject).currentFrame++;
	(*gameObject).currentFrame = (*gameObject).currentFrame % (*gameObject).totalFrames;
	//////DeleteObject(saveOldTempBmp);
	////DeleteObject(saveOldBitmap);
	////DeleteDC(tempDC2);
	SelectObject(tempDC, saveOldTempBmp);
	DeleteDC(tempDC);
}


void GameViewState::ProcessMousePosition()
{
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(mainWindow, &mousePosition);
	if (mousePosition.y > gameViewArea.top && mousePosition.y < 800)
	{
		if (mousePosition.x < gameViewArea.left + 25 && mousePosition.x > gameViewArea.left - 2)
		{
			mapViewX -= 4;
			if (mapViewX < 0)
			{
				mapViewX = 0;
			}
		}
		if (mousePosition.x < gameViewArea.right + 2 && mousePosition.x > gameViewArea.right - 25)
		{
			mapViewX += 4;
			if ((mapViewX + gameViewArea.right) > mapWidth)
			{
				mapViewX = mapWidth - gameViewArea.right;
			}
		}
	}
	if (mousePosition.x > gameViewArea.left && mousePosition.x < gameViewArea.right)
	{
		if (mousePosition.y < gameViewArea.top + 25 && mousePosition.y > gameViewArea.top - 2)
		{
			mapViewY -= 4;
			if (mapViewY < 0)
			{
				mapViewY = 0;
			}
		}
		if (mousePosition.y < gameViewArea.bottom + 2 && mousePosition.y > gameViewArea.bottom - 25)
		{
			mapViewY += 4;
			if ((mapViewY + gameViewArea.bottom) > mapHeight)
			{
				mapViewY = mapHeight - gameViewArea.bottom;
			}
		}
	}
}

