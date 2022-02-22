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

	WorldMap::TheWorld().Initialize(DEFAULTWORLDWIDTH, DEFAULTWORLDHEIGHT);
	GenerateFirstPeopleDemo();
	player1 = std::make_unique<DivinePlayer>();
	_gameMode = INGAME;

	turnNumber = 0;
	dayTimeSlot = 0;
}

GAMEMODE GameState::GetGameMode()
{
	return _gameMode;
}

void GameState::ProcessStep()
{
	if (dayTimeSlot == 0)
	{
		std::list<std::unique_ptr<Denizen>>::iterator i;
		for (i = theLiving.begin(); i != theLiving.end(); i++)
		{
			(*(*i)).CreateStrategyForMonth();
		}
		(*player1).AddToCurrentEnergy(5);
		turnNumber++;
	}

	std::list<std::unique_ptr<Denizen>>::iterator c;
	c = theLiving.begin();
	while (c != theLiving.end())
	{
		ProcessDenizenStep((*(*c)));
		c++;
	}

	dayTimeSlot = (dayTimeSlot + 1) % ACTIONPOINTS;
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
	//empty for now
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

		if ( (*(*i)).gender == genderEnum::male )
		{

		}

		if ((*(*i)).TryNaturalDeath())
		{
			std::wstring deathMessage = (*(*i)).shortName;
			deathMessage.append(L" passed away.");
			if (WorldMap::TheWorld().worldCells[(*(*i)).worldX][(*(*i)).worldY].Occupant == &(*(*i)))
			{
				WorldMap::TheWorld().worldCells[(*(*i)).worldX][(*(*i)).worldY].Occupant = NULL;
			}
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

void GameState::ProcessDenizenStep(Denizen& person)
{
	person.Act(this->dayTimeSlot);
}

void GameState::DenizenSelectMate(Denizen * seeker)
{
	std::list<SocialNode>::iterator i;
	switch ((*seeker).courtshipStyle)
	{
	case CourtshipStyle::chaser:
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
			requestResponse::requestResponse courtingResponse = (*(*potentialNewPartner).person).HearRequest(request::court, (*potentialNewPartner).ownerNode);
			if (courtingResponse == requestResponse::yes)
			{
				if ((*seeker).currentPartner != NULL)
				{
					std::wstring relationshipEndMessage = (*seeker).shortName;
					relationshipEndMessage.append(L" and ");
					relationshipEndMessage.append((*(*(*seeker).currentPartner).person).shortName);
					if ((*(*seeker).currentPartner).currentRelationship == RelationshipStatus::married)
					{
						relationshipEndMessage.append(L" got divorced");
					}
					else if ((*(*seeker).currentPartner).currentRelationship == RelationshipStatus::courting)
					{
						relationshipEndMessage.append(L" broke up");
					}

					(*seeker).ChangeRelationshipWithOther(RelationshipStatus::ex, (*seeker).currentPartner);
					(*(*(*seeker).currentPartner).person).ChangeRelationshipWithOther(RelationshipStatus::ex, (*(*seeker).currentPartner).ownerNode);

					(*(*(*seeker).currentPartner).person).currentPartner = NULL;
					gameMessages.push_back(relationshipEndMessage);
				}
				if ((*(*potentialNewPartner).person).currentPartner != NULL)
				{
					std::wstring relationshipEndMessage = (*(*potentialNewPartner).person).shortName;
					relationshipEndMessage.append(L" and ");
					relationshipEndMessage.append((*(*(*(*potentialNewPartner).person).currentPartner).person).shortName);
					if ((*(*(*potentialNewPartner).person).currentPartner).currentRelationship == RelationshipStatus::married)
					{
						relationshipEndMessage.append(L" got divorced");
					}
					else if ((*(*(*potentialNewPartner).person).currentPartner).currentRelationship == RelationshipStatus::courting)
					{
						relationshipEndMessage.append(L" broke up");
					}

					(*(*potentialNewPartner).person).ChangeRelationshipWithOther(RelationshipStatus::ex, (*(*potentialNewPartner).person).currentPartner);
					(*(*(*(*potentialNewPartner).person).currentPartner).person).ChangeRelationshipWithOther(RelationshipStatus::ex, (*(*(*potentialNewPartner).person).currentPartner).ownerNode);

					(*(*(*(*potentialNewPartner).person).currentPartner).person).currentPartner = NULL;
					gameMessages.push_back(relationshipEndMessage);
				}
				std::wstring relationshipStartMessage = (*seeker).shortName;
				relationshipStartMessage.append(L" and ");
				relationshipStartMessage.append((*(*potentialNewPartner).person).shortName);
				relationshipStartMessage.append(L" started seeing each other");

				(*seeker).currentPartner = potentialNewPartner;
				(*(*potentialNewPartner).person).currentPartner = (*potentialNewPartner).ownerNode;

				(*seeker).ChangeRelationshipWithOther(RelationshipStatus::courting, potentialNewPartner);
				(*(*potentialNewPartner).person).ChangeRelationshipWithOther(RelationshipStatus::courting, (*potentialNewPartner).ownerNode);


				gameMessages.push_back(relationshipStartMessage);
			}
		}
		else if(currentPartner != NULL)
		{
			if ((*currentPartner).currentRelationship == RelationshipStatus::courting && (*currentPartner).currentRelationshipAge >= 6 && (rand() % 3) == 0)
			{
				requestResponse::requestResponse proposalResponse = (*(*currentPartner).person).HearRequest(request::marriage, (*currentPartner).ownerNode);
				if (proposalResponse == requestResponse::yes)
				{
					(*seeker).ChangeRelationshipWithOther(RelationshipStatus::married, currentPartner);
					(*(*currentPartner).person).ChangeRelationshipWithOther(RelationshipStatus::married, (*currentPartner).ownerNode);
					std::wstring relationshipStartMessage = (*seeker).shortName;
					relationshipStartMessage.append(L" and ");
					relationshipStartMessage.append((*(*currentPartner).person).shortName);
					relationshipStartMessage.append(L" got married!");

					gameMessages.push_back(relationshipStartMessage);
				}
			}
			else if ((*currentPartner).currentRelationship == RelationshipStatus::married && (rand() % 2) == 0)
			{
				requestResponse::requestResponse loveResponse = (*(*currentPartner).person).HearRequest(request::copulate, (*currentPartner).ownerNode);
				if (loveResponse == requestResponse::yes)
				{
					bool successfulFertilization = false;
					if ((*seeker).gender == genderEnum::male)
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
			dadgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[8]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::male, L"Bobbo", 240, momgenes, dadgenes, &theLiving, 0, 0);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 1:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::female, L"Stephanie", 380, momgenes, dadgenes, &theLiving, 5, 0);
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
			dadgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[8]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::male, L"Thomas", 300, momgenes, dadgenes, &theLiving, 0, 5);
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
			dadgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::female, L"Hannah", 220, momgenes, dadgenes, &theLiving, 5, 5);
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
			dadgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[8]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::male, L"Erick", 400, momgenes, dadgenes, &theLiving, 2, 8);
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
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::female, L"Sarai", 290, momgenes, dadgenes, &theLiving, 2, 8);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 6:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[4]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[8]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::male, L"Mortimer", 540, momgenes, dadgenes, &theLiving, 3, 6);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		case 7:
		{
			momgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[7]));
			momgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[2]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[6]));
			dadgenes.push_back(&(*(*Biology::bio()).genepool[0]));
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::female, L"Lena", 140, momgenes, dadgenes, &theLiving, 7, 7);
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
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::male, L"Justin", 90, momgenes, dadgenes, &theLiving, 2, 10);
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
			std::unique_ptr<Denizen> newPerson = std::make_unique<Denizen>(genderEnum::female, L"Barbara", 270, momgenes, dadgenes, &theLiving, 4, 10);
			BirthDenizen(&(*newPerson));
			theLiving.push_back(std::move(newPerson));
			break;
		}
		default:
			break;
		}
	}
}

