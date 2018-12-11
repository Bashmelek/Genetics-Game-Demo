#include "stdafx.h"
#include "GameState.h"



GameState::GameState()
{
	_gameMode = MAINMENU;
}

void GameState::StartNewGame(HWND hwnd)
{
	_gameMode = ENTERINGGAME;
	srand(time(NULL));
	selectedPerson = NULL;
	//float i = cos(1.0);
	/*if(bio != NULL)
	{ 
		bio.reset();
	}
	bio = std::make_unique<Biology>();*/
	/*EnumChildWindows(
		hwnd,
		CleanWindow,
		NULL);*/
	/*for (int i = 0; i < 8; i++)
	{
		std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(true, i, &theLiving);
		BirthDenizen(&(*newPerson));
		theLiving.push_back(std::move(newPerson));
	}*/
	GenerateFirstPeopleDemo();
	////player1(std::make_unique<DivinePlayer>());
	player1 = std::make_unique<DivinePlayer>();
	WorldMap::TheWorld().Initialize(DEFAULTWORLDWIDTH, DEFAULTWORLDHEIGHT);
	_gameMode = INGAME;

	turnNumber = 0;
	//(*GameViewState::TheGameView()).nextTurnButtonWindow = CreateWindowEx(NULL, TEXT("button"), TEXT("Next Turn"),
	//	WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,// | WS_OVERLAPPED,
	//	900, 5, 80, 25,
	//	hwnd, (HMENU)BUTTON_NEXTTURN, NULL, NULL);
	//InvalidateRect(blah, NULL, FALSE);
	//UpdateWindow(blah);
	/*
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = GetDC(hwnd);// , NULL, DCX_WINDOW);
					  //ps.fErase = 1;
					  //SelectObject(ps.hdc, GetStockObject(BLACK_BRUSH));
	////ps.rcPaint = { 0, 0, 700, 400 };
	GetClientRect(hwnd, &ps.rcPaint);
	////FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 32, 61)));// RGB(50, 200, 200)));
	DrawGameScene(hdc, ps);
	//Rectangle(ps.hdc, 5, 5,//rcTarget.left, rcTarget.top,
		//700, 400);// rcTarget.right, rcTarget.bottom);
				  //EndPaint(hwnd, &ps);
	ReleaseDC(hwnd, hdc);*/
	//////DrawNextFrame(hwnd);
}

GAMEMODE GameState::GetGameMode()
{
	return _gameMode;
}


void GameState::NextTurn()
{
	std::wstring turnString = L"Turn ";// +turnNumber;
	turnString.append(std::to_wstring(turnNumber));
	int oldMessageCount = gameMessages.size();


	ProcessDenizenActivity();

	if (gameMessages.size() == oldMessageCount)
	{
		gameMessages.push_back(L"Nothing happened");
	}

	gameMessages.push_back(turnString);
	(*player1).AddToCurrentEnergy(5);
	turnNumber++;
}

void GameState::BirthDenizen(Denizen * newborn)
{
	//initializeRelationships
	/*std::list<std::unique_ptr<Denizen>>::iterator i;
	for (i = theLiving.begin(); i != theLiving.end(); i != theLiving.end() ? i++ : i = i)
	{
		SocialNode newNode;
		newNode.person = &(*(*i));
		(*newborn).socialSphere.relationships.push_back(newNode);
	}*/
}


void GameState::ProcessDenizenActivity()
{
	std::list<std::unique_ptr<Denizen>>::iterator i;
	for (i = theLiving.begin(); i != theLiving.end(); i++)
	{
		DenizenSelectMate(&(*(*i)));
	}
	i = theLiving.begin();
	while(i != theLiving.end())
	{
		if ((*(*i)).isBorn != true)
		{
			(*(*i)).monthsGestating += 1;
			(*(*(*(*i)).mother).person).monthsPregnant += 1;
			int birthResult = (*(*i)).TryToBeBorn();
			if (birthResult == 1)
			{
				
				std::wstring birthMessage = (*(*i)).BeBorn(&theLiving);
				gameMessages.push_back(birthMessage);
			}
		}
		else
		{
			(*(*i)).ageMonths += 1;
		}

		if ( (*(*i)).gender == male )
		{

		}

		if ((*(*i)).TryNaturalDeath())
		{
			std::wstring deathMessage = (*(*i)).shortName;
			deathMessage.append(L" passed away.");
			theDead.push_back(std::move(*i));
			theLiving.erase(i++);
			//theDead.
			gameMessages.push_back(deathMessage);
		}
		else
		{
			i++;
		}
	}
}

void GameState::DenizenSelectMate(Denizen * seeker)
{
	std::list<SocialNode>::iterator i;
	switch ((*seeker).courtshipStyle)
	{
	case chaser:
		{
		SocialNode* currentPartner = (*seeker).currentPartner;
		SocialNode* potentialNewPartner = NULL;
		int currentPartnerScore = (*seeker).EvaluateAsMate(currentPartner);
		int currentWinningScore = currentPartnerScore;
		for (i = (*seeker).socialSphere.relationships.begin(); i != (*seeker).socialSphere.relationships.end(); i != (*seeker).socialSphere.relationships.end() ? i++ : i = i)
		{
			SocialNode * potentialMate = &(*i);
			int potentialPartnerScore = (*seeker).EvaluateAsMate(potentialMate);
			if (potentialPartnerScore > currentWinningScore)
			{
				currentWinningScore = potentialPartnerScore;
				potentialNewPartner = potentialMate;
			}
			(*i).currentRelationshipAge += 1;
		}
		if (potentialNewPartner != NULL && potentialNewPartner != currentPartner)
		{
			requestResponse courtingResponse = (*(*potentialNewPartner).person).HearRequest(court, (*potentialNewPartner).ownerNode);
			if (courtingResponse == yes)
			{
				if ((*seeker).currentPartner != NULL)
				{
					std::wstring relationshipEndMessage = (*seeker).shortName;
					relationshipEndMessage.append(L" and ");
					relationshipEndMessage.append((*(*(*seeker).currentPartner).person).shortName);
					if ((*(*seeker).currentPartner).currentRelationship == married)
					{
						relationshipEndMessage.append(L" got divorced");
					}
					else if ((*(*seeker).currentPartner).currentRelationship == courting)
					{
						relationshipEndMessage.append(L" broke up");
					}

					(*seeker).ChangeRelationshipWithOther(ex, (*seeker).currentPartner);
					(*(*(*seeker).currentPartner).person).ChangeRelationshipWithOther(ex, (*(*seeker).currentPartner).ownerNode);

					(*(*(*seeker).currentPartner).person).currentPartner = NULL;
					gameMessages.push_back(relationshipEndMessage);
				}
				if ((*(*potentialNewPartner).person).currentPartner != NULL)
				{
					std::wstring relationshipEndMessage = (*(*potentialNewPartner).person).shortName;
					relationshipEndMessage.append(L" and ");
					relationshipEndMessage.append((*(*(*(*potentialNewPartner).person).currentPartner).person).shortName);
					if ((*(*(*potentialNewPartner).person).currentPartner).currentRelationship == married)
					{
						relationshipEndMessage.append(L" got divorced");
					}
					else if ((*(*(*potentialNewPartner).person).currentPartner).currentRelationship == courting)
					{
						relationshipEndMessage.append(L" broke up");
					}

					(*(*potentialNewPartner).person).ChangeRelationshipWithOther(ex, (*(*potentialNewPartner).person).currentPartner);
					(*(*(*(*potentialNewPartner).person).currentPartner).person).ChangeRelationshipWithOther(ex, (*(*(*potentialNewPartner).person).currentPartner).ownerNode);

					(*(*(*(*potentialNewPartner).person).currentPartner).person).currentPartner = NULL;
					gameMessages.push_back(relationshipEndMessage);
				}
				std::wstring relationshipStartMessage = (*seeker).shortName;
				relationshipStartMessage.append(L" and ");
				relationshipStartMessage.append((*(*potentialNewPartner).person).shortName);
				relationshipStartMessage.append(L" started seeing each other");

				(*seeker).currentPartner = potentialNewPartner;
				(*(*potentialNewPartner).person).currentPartner = (*potentialNewPartner).ownerNode;

				(*seeker).ChangeRelationshipWithOther(courting, potentialNewPartner);
				(*(*potentialNewPartner).person).ChangeRelationshipWithOther(courting, (*potentialNewPartner).ownerNode);


				gameMessages.push_back(relationshipStartMessage);
			}
		}
		else if(currentPartner != NULL)
		{
			if ((*currentPartner).currentRelationship == courting && (*currentPartner).currentRelationshipAge >= 6 && (rand() % 4) == 0)
			{
				requestResponse proposalResponse = (*(*currentPartner).person).HearRequest(marriage, (*currentPartner).ownerNode);
				if (proposalResponse == yes)
				{
					(*seeker).ChangeRelationshipWithOther(married, currentPartner);
					(*(*currentPartner).person).ChangeRelationshipWithOther(married, (*currentPartner).ownerNode);
					std::wstring relationshipStartMessage = (*seeker).shortName;
					relationshipStartMessage.append(L" and ");
					relationshipStartMessage.append((*(*currentPartner).person).shortName);
					relationshipStartMessage.append(L" got married!");

					gameMessages.push_back(relationshipStartMessage);
				}
			}
			else if ((*currentPartner).currentRelationship == married && (rand() % 3) == 0)
			{
				requestResponse loveResponse = (*(*currentPartner).person).HearRequest(copulate, (*currentPartner).ownerNode);
				if (loveResponse == yes)
				{
					bool successfulFertilization = false;
					if ((*seeker).gender == male)
					{
						successfulFertilization = (*(*(*seeker).currentPartner).person).TryHaveChildren(*seeker);
					}
					else
					{
						successfulFertilization = (*seeker).TryHaveChildren(*(*currentPartner).person);
					}

					if (successfulFertilization)
					{
						std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(seeker, (*currentPartner).person);
						theLiving.push_back(std::move(newPerson));
						std::wstring conceptionMessage = (*seeker).shortName;
						conceptionMessage.append(L" and ");
						conceptionMessage.append((*(*currentPartner).person).shortName);
						conceptionMessage.append(L" concieved a child!");

						gameMessages.push_back(conceptionMessage);
					}
				}
			}
		}
		}
		break;
		default: break;
	}
}


void GameState::GenerateFirstPeopleDemo()
{
	std::list<Gene*> momgenes;
	std::list<Gene*> dadgenes;
	for (int i = 0; i < 10; i++)
	{
		momgenes.clear();
		dadgenes.clear();
		switch (i)
		{
		case 0:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[1]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[8]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(male, L"Bobbo", 240, momgenes, dadgenes, &theLiving, 0, 0);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 1:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(female, L"Stephanie", 380, momgenes, dadgenes, &theLiving, 5, 0);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 2:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[4]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[5]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[8]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(male, L"Thomas", 300, momgenes, dadgenes, &theLiving, 0, 5);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 3:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[3]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(female, L"Hannah", 220, momgenes, dadgenes, &theLiving, 5, 5);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 4:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[3]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[8]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(male, L"Erick", 400, momgenes, dadgenes, &theLiving, 2, 8);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 5:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[4]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(female, L"Sarai", 290, momgenes, dadgenes, &theLiving, 2, 8);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 6:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[4]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[8]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(male, L"Mortimer", 640, momgenes, dadgenes, &theLiving, 3, 6);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 7:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(female, L"Lena", 140, momgenes, dadgenes, &theLiving, 7, 7);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 8:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[4]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[8]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(male, L"Justin", 90, momgenes, dadgenes, &theLiving, 2, 10);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 9:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[5]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(female, L"Barbara", 280, momgenes, dadgenes, &theLiving, 4, 10);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		default:
			break;
		}
	}
}

