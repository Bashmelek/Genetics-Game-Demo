#pragma once

#include "GameState.h"
#include "SubWindows.h"

#ifndef GAMEVIEWSTATEDEF
#define GAMEVIEWSTATEDEF


class GameViewState
{
	public:
		GameViewState();
		int mapViewX;
		int mapViewY;
		HDC deviceContext;
		HWND mainWindow;
		HWND nextTurnButtonWindow;
		HDC memoryDC;
		HBITMAP memoryBMP;
		static GameViewState* TheGameView();
		int manaBarFillFrame;
		HBITMAP manaBarImage;
		HBITMAP manaBarCoverMask;
		HBITMAP manaBarMask;
		HBITMAP manaBarAntiMask;
		HBITMAP manaBarBackgroudImage;
		HBITMAP manaBarFillImage;
		HBITMAP manaBarFillMask;

		void InitIntroScreen(HWND hwnd);
		void PaintScene(HWND hwnd);
		void DrawNextFrame(HWND hwnd);
		void HandleButtonInput(int buttonID);

		int mapWidth;
		int mapHeight;
		RECT gameViewArea;

	private:
		std::unique_ptr<GameState> _gameState;
		std::unique_ptr<GameMessagesWindow> _gameMessagesWindow;
		std::unique_ptr<GameInfoTabsWindow> _gameInfoTabsWindow;
		int currentGameInfoTab;
		void DrawGameScene(HDC hdc, PAINTSTRUCT ps);
		void AnimateWorldFrame();
		void AnimateUI(HDC hdc, PAINTSTRUCT* ps);
		void AnimateChildUIWindows();
		void DrawGameMessagesWindow();
		void AnimateGameInfoTabsWindow();
		void DrawObject(Denizen* gameObject);
		void GameViewState::ProcessMousePosition();
};


#endif GAMEVIEWSTATEDEF
