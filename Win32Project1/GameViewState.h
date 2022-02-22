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

		void InitIntroScreen(HWND hwnd);
		void PaintScene(HWND hwnd);
		void DrawNextFrame(HWND hwnd);
		void HandleButtonInput(int buttonID);
		void DoNextStep();

		int mapWidth;
		int mapHeight;
		RECT gameViewArea;
		bool mouseJustClicked;

		bool IsOnAutoTime;
		int autoTimeProgress;

	private:
		std::unique_ptr<GameState> _gameState;
		std::unique_ptr<GameInfoWindow> _gameInfoWindow;
		std::unique_ptr<GameInfoTabsWindow> _gameInfoTabsWindow;
		int currentGameInfoTab;
		GameObject* objectToHighlight;

		HBITMAP defaultHighlightImage;
		HBITMAP defaultSelectorImage;
		HBITMAP manaBarImage;
		HBITMAP manaBarCoverMask;
		HBITMAP manaBarMask;
		HBITMAP manaBarAntiMask;
		HBITMAP manaBarBackgroudImage;
		HBITMAP manaBarFillImage;
		HBITMAP manaBarFillMask;

		void DrawGameScene(HDC hdc, PAINTSTRUCT ps);
		void AnimateWorldFrame();
		void AnimateUI(HDC hdc, PAINTSTRUCT* ps);
		void AnimateChildUIWindows();
		void DrawGameInfoWindow();
		void AnimateGameInfoTabsWindow();
		void DrawObject(Denizen* gameObject);
		void ProcessMousePosition();

		void DrawMessagesInfoGameInfoWindow(HDC uiMemoryDC);
		void DrawAllelesInfoGameInfoWindow(HDC uiMemoryDC);

		void ProcessMouseOverGameWorldObjects(int mouseX, int mouseY);
		bool IsMouseOverObject(int mouseX, int mouseY, GameObject& gobject);
};


#endif GAMEVIEWSTATEDEF
