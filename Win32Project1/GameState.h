#pragma once

#include <list>
#include "Denizen.h"
#include "DivinePlayer.h"
#include "Biology.h"

#ifndef GAMESTATEDEF
#define GAMESTATEDEF

enum GAMEMODE { MAINMENU, ENTERINGGAME, INGAME };

class GameState
{
	public:
		GameState();
		std::list<std::unique_ptr<Denizen>> theLiving;
		std::list<std::unique_ptr<Denizen>> theDead;
		std::unique_ptr<DivinePlayer> player1;
		void StartNewGame(HWND hwnd);
		GAMEMODE GetGameMode();
		void NextTurn();
		std::list<std::wstring> gameMessages;
		int turnNumber;

		////std::unique_ptr<Biology> bio;
		//void InitIntroScreen(HWND hwnd);
		//void StartNewGame(HWND hwnd);
		//void PaintScene(HWND hwnd);
		//void DrawNextFrame(HWND hwnd);
	private:
		GAMEMODE _gameMode;
		void ProcessDenizenActivity();
		void DenizenSelectMate(Denizen * seeker);
		void BirthDenizen(Denizen * newborn);

		void GenerateFirstPeopleDemo();
		////GameViewState _viewState;
		//void DrawGameScene(HDC hdc, PAINTSTRUCT ps);
		//void AnimateWorldFrame();
		//void AnimateUI(HDC hdc, PAINTSTRUCT* ps);
		//void AnimateChildUIWindows();
		//BOOL CleanWindow(HWND hwnd, LPARAM lParam);
};




#endif GAMESTATEDEF


