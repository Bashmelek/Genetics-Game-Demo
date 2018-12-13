#include "stdafx.h"
#include "GameViewState.h"

GameViewState::GameViewState()
{
	_gameState = std::make_unique<GameState>();
	mapViewX = 0;
	mapViewY = 0;
	mapWidth = 2400;
	mapHeight = 1800;
	objectToHighlight = NULL;
	mouseJustClicked = false;
	IsOnAutoTime = false;
	autoTimeProgress = 0;

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
	defaultHighlightImage = (HBITMAP)LoadImage(
		NULL,
		L"dudehighlight.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	defaultSelectorImage = (HBITMAP)LoadImage(
		NULL,
		L"dudeselected.bmp",////\\GameResources
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

		_gameInfoWindow = std::make_unique<GameInfoWindow>();
		(*_gameInfoWindow).Create(L"Nothing to see here", WS_OVERLAPPEDWINDOW,
				0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, mainWindow,0);

		_gameInfoTabsWindow = std::make_unique<GameInfoTabsWindow>();
		(*_gameInfoTabsWindow).Create(L"Click to see what happens next", WS_OVERLAPPEDWINDOW,
			0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, mainWindow, 0);
		(*_gameInfoTabsWindow).InitInternals();

		currentGameInfoTab = 0;
		IsOnAutoTime = true;///////////-----------------------------normally shall be FALSE!!!!!!!!!!FAAAALLLSSSEEE!!!
		autoTimeProgress = 0;

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
	case BUTTON_MESSAGETAB:
		currentGameInfoTab = 0;
		break;
	case BUTTON_ALLELESTAB:
		currentGameInfoTab = 1;
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
	gameViewArea = { SIDEBARWIDTH, TOPBARHEIGHT,  WINDOWLENGTH - SIDEBARWIDTH, ps.rcPaint.bottom - DETAILSECTIONHEIGHT + DETAILTABSSECTIONHEIGHT };
	ProcessMousePosition();

	//do the cool game center area
	HBRUSH brush2 = CreateSolidBrush(RGB(25, 25, 25));
	RECT gameRect = { SIDEBARWIDTH, 0, WINDOWLENGTH - SIDEBARWIDTH, ps.rcPaint.bottom - DETAILSECTIONHEIGHT + DETAILTABSSECTIONHEIGHT };
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

	RECT TopBarRect = { 0, 0, ps.rcPaint.right, TOPBARHEIGHT };// ps.rcPaint.bottom / 12};

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
	DrawGameInfoWindow();

	AnimateGameInfoTabsWindow();

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
	FillRect(nextTurnButtonDC, &buttRect, brush1);
	DeleteObject(brush1);
	
	ReleaseDC(nextTurnButtonWindow, nextTurnButtonDC);
}

void GameViewState::DrawGameInfoWindow()
{
	HDC otherDC = GetDC((*_gameInfoWindow).Window());
	HDC uiMemoryDC = CreateCompatibleDC(otherDC);
	HBITMAP uiMemoryBMP = (HBITMAP)CreateCompatibleBitmap(otherDC, WINDOWLENGTH - 2 * SIDEBARWIDTH, 700);
	HBITMAP uiOriginalBMP = (HBITMAP)SelectObject(uiMemoryDC, uiMemoryBMP);
	
	switch (currentGameInfoTab)
	{
	case 0:
		DrawMessagesInfoGameInfoWindow(uiMemoryDC);
		break;
	case 1:
		DrawAllelesInfoGameInfoWindow(uiMemoryDC);
		break;
	default:
		DrawMessagesInfoGameInfoWindow(uiMemoryDC);
		break;
	}

	BitBlt(otherDC, 0, 0, WINDOWLENGTH - 2 * SIDEBARWIDTH, 700, uiMemoryDC, 0, 0, SRCCOPY);
	
	ReleaseDC((*_gameInfoWindow).Window(), otherDC);

	SelectObject(uiMemoryDC, uiOriginalBMP);
	DeleteObject(uiMemoryBMP);
	DeleteDC(uiMemoryDC);
}

void GameViewState::DrawMessagesInfoGameInfoWindow(HDC uiMemoryDC)
{
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
	DeleteObject(brush2);
}

void GameViewState::DrawAllelesInfoGameInfoWindow(HDC uiMemoryDC)
{
	RECT buddyrect = { 5, 5, WINDOWLENGTH - 2 * SIDEBARWIDTH, 700 };
	HBRUSH brush2 = CreateSolidBrush(RGB(200, 200, 200));
	FillRect(uiMemoryDC, &buddyrect, brush2);
	//std::list<std::wstring>::reverse_iterator it;
	////int messageCount = 0;
	RECT messageRect = { 5, 5, WINDOWLENGTH - 2 * SIDEBARWIDTH, 700 };
	//for (it = (*_gameState).gameMessages.rbegin(); it != (*_gameState).gameMessages.rend(); it++) {
	//	//DrawText(uiMemoryDC, (*it).c_str(), -1, &messageRect, NULL);
	//	messageRect.top += 20;
	//	messageRect.bottom += 20;
	//}
	if ((*_gameState).selectedPerson != NULL)
	{
		DrawText(uiMemoryDC, (*(*_gameState).selectedPerson).shortName.c_str(), -1, &messageRect, NULL);
		messageRect.top += 20;
		messageRect.bottom += 20;
		if ((*(*_gameState).selectedPerson).alleles[eyecolor] == 1)
		{
			DrawText(uiMemoryDC, L"blue eyes", -1, &messageRect, NULL);
		}
		else
		{
			DrawText(uiMemoryDC, L"brown eyes", -1, &messageRect, NULL);
		}
	}
	DeleteObject(brush2);
}

void GameViewState::AnimateGameInfoTabsWindow()
{
	HDC dc = GetDC((*_gameInfoTabsWindow).Window());
	HDC tabsMemoryDC = CreateCompatibleDC(dc);
	HBITMAP uiMemoryBMP = (HBITMAP)CreateCompatibleBitmap(dc, WINDOWLENGTH - 2 * SIDEBARWIDTH, DETAILTABSSECTIONHEIGHT);
	HBITMAP uiOriginalBMP = (HBITMAP)SelectObject(tabsMemoryDC, uiMemoryBMP);
	//HBITMAP saveTempDCImage = (HBITMAP)SelectObject(uiMemoryDC, manaBarMask);
	RECT buddyrect = { 5, 0, WINDOWLENGTH - 2 * SIDEBARWIDTH, DETAILTABSSECTIONHEIGHT };
	HBRUSH tabBGBrush = CreateSolidBrush(RGB(100, 200, 200));
	FillRect(tabsMemoryDC, &buddyrect, tabBGBrush);
	BitBlt(dc, 0, 0, WINDOWLENGTH - 2 * SIDEBARWIDTH, DETAILTABSSECTIONHEIGHT, tabsMemoryDC, 0, 0, SRCCOPY);

	DeleteObject(tabBGBrush);

	SelectObject(tabsMemoryDC, uiOriginalBMP);
	DeleteObject(uiMemoryBMP);
	DeleteDC(tabsMemoryDC);
	ReleaseDC((*_gameInfoTabsWindow).Window(), dc);
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
	int screenLocX = (*gameObject).worldX * 50 - mapViewX + SIDEBARWIDTH;//200
	int screenLocY = (*gameObject).worldY * 50 - mapViewY + TOPBARHEIGHT + 10;//50
	HBITMAP saveOldTempBmp = 0;
	HBITMAP tempBmp;
	if (gameObject == objectToHighlight)
	{
		saveOldTempBmp = (HBITMAP)SelectObject(tempDC, defaultHighlightImage);
		BitBlt(memdc, screenLocX, screenLocY, 50, 50, tempDC, 0, 0, SRCPAINT);
	}
	if (gameObject == ((*_gameState).selectedPerson))
	{
		tempBmp = (HBITMAP)SelectObject(tempDC, defaultSelectorImage);
		if (saveOldTempBmp == 0)
		{
			saveOldTempBmp = tempBmp;
		}
		BitBlt(memdc, screenLocX, screenLocY, 50, 50, tempDC, 0, 0, SRCPAINT);
	}

	tempBmp = (HBITMAP)SelectObject(tempDC, (*gameObject).imageMask);
	if (saveOldTempBmp == 0)
	{
		saveOldTempBmp = tempBmp;
	}
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

	//if in game area, go over objects
	if (mousePosition.y < gameViewArea.bottom && mousePosition.y > gameViewArea.top
		&& mousePosition.x < gameViewArea.right && mousePosition.x > gameViewArea.left)
	{
		ProcessMouseOverGameWorldObjects(mousePosition.x, mousePosition.y);
	}
}


void GameViewState::ProcessMouseOverGameWorldObjects(int mouseX, int mouseY)
{
	(*this).objectToHighlight = NULL;
	std::list<std::unique_ptr<Denizen>>::iterator i;
	for (i = (*_gameState).theLiving.begin(); i != (*_gameState).theLiving.end(); i++)
	{
		if (IsMouseOverObject(mouseX, mouseY, (*(*i))))
		{
			(*this).objectToHighlight = &(*(*i));
			if (mouseJustClicked)
			{
				(*_gameState).selectedPerson = &(*(*i));
			}
			break;
		}
	}
	mouseJustClicked = false;
}

bool GameViewState::IsMouseOverObject(int mouseX, int mouseY, GameObject& gobject)
{
	int screenLocLeft = gobject.worldX * 50 - mapViewX + SIDEBARWIDTH;
	int screenLocRight = screenLocLeft + TOPBARHEIGHT + 10;

	if (mouseX > screenLocLeft && mouseX < screenLocRight)
	{
		int screenLocTop = gobject.worldY * 50 + 50 - mapViewY;
		int screenLocBottom = screenLocTop + 50;
		if (mouseY > screenLocTop && mouseY < screenLocBottom)
		{
			return true;
		}
	}
	return false;
}


