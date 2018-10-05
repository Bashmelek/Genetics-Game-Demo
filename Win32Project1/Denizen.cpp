#include "stdafx.h"
#include "Denizen.h"

Denizen::Denizen()
{
	imageBase = (HBITMAP)LoadImage(
		NULL,
		L"Lildude.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
}

void Denizen::BaseInit()
{
	imageBase = (HBITMAP)LoadImage(
		NULL,
		L"Lildude2.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	imageMask = (HBITMAP)LoadImage(
		NULL,
		L"Lildude2mask.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	ladyImageBase = (HBITMAP)LoadImage(
		NULL,
		L"Lillady.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	totalFrames = 3;
	currentFrame = 0;

	currentPartner = NULL;
	isPregnant = false;
	monthsPregnant = 0;
}


Denizen::Denizen(bool isDummy)
{
	BaseInit();
	worldX = 0;
	worldY = 0;
}

Denizen::Denizen(bool isDummy, int num, std::list<std::unique_ptr<Denizen>>* currentLiving) : socialSphere()
{
	BaseInit();
	switch (num)
	{
	case 0:
		worldX = 0;
		worldY = 0;
		gender = male;
		age = 20;
		shortName = L"Bob";
		break;
	case 1:
		worldX = 5;
		worldY = 5;
		gender = male;
		shortName = L"Billy";
		age = 40;
		break;
	case 2:
		worldX = 10;
		worldY = 5;
		gender = female;
		shortName = L"Dana";
		age = 10;
		break;
	case 3:
		worldX = 5;
		worldY = 10;
		gender = female;
		age = 24;
		shortName = L"Jane";
		break;
	case 4:
		worldX = 0;
		worldY = 5;
		gender = female;
		shortName = L"Leia";
		age = 60;
		break;
	case 5:
		worldX = 5;
		worldY = 0;
		gender = male;
		shortName = L"Zeno";
		age = 19;
		break;
	case 6:
		worldX = 7;
		worldY = 2;
		gender = female;
		shortName = L"Adelaide";
		age = 38;
		break;
	case 7:
		worldX = 2;
		worldY = 7;
		gender = male;
		age = 27;
		shortName = L"Maron";
		break;
	default:
		break;
	}

	ageMonths = age * 12;
	if (gender == female)
	{
		courtshipStyle = beingChased;
	}
	else
	{
		courtshipStyle = chaser;
	}
	InitializeRelationships(currentLiving, 0, 0);
}

Denizen::Denizen(genderEnum gender, std::wstring shortName, int ageMonths, std::list<Gene*> momgenes, std::list<Gene*> dadgenes, std::list<std::unique_ptr<Denizen>>* currentLiving, int worldX, int worldY)
{
	BaseInit();
	std::list<Gene*>::iterator i;
	for (i = momgenes.begin(); i != momgenes.end(); i++)
	{
		genome.maternalGenes.push_back((*i));
	}

	for (i = dadgenes.begin(); i != dadgenes.end(); i++)
	{
		genome.paternalGenes.push_back((*i));
	}
	
	(*this).isBorn = true;
	(*this).worldX = worldX;
	(*this).worldY = worldY;
	(*this).shortName = shortName;
	(*this).gender = gender;
	(*this).ageMonths = ageMonths;
	age = ageMonths/12;
	if (gender == female)
	{
		courtshipStyle = beingChased;
	}
	else
	{
		courtshipStyle = chaser;
	}
	InitializeRelationships(currentLiving, 0, 0);
}

Denizen::Denizen(Denizen* parent1, Denizen* parent2)
{
	BaseInit();

	(*this).isBorn = false;
	(*this).monthsGestating = 0;

	if ((*parent1).gender == female)
	{
		(*this).genome.InitializeGenome(&(*parent2).genome, &(*parent1).genome);
		InitializeSingleRelationship(parent1, RelationshipStatus::mother, RelationshipStatus::child);
		InitializeSingleRelationship(parent2, RelationshipStatus::father, RelationshipStatus::child);
		(*parent1).isPregnant = true;
		(*parent1).monthsPregnant = 0;
		(*parent1).kidsPregnantWith.push_back(this);
		//socialSphere.relationships.push_back(SocialNode(parent1, RelationshipStatus::mother));// , this, true));
		//SocialNode* newGuysNodeForExistingPerson = &(socialSphere.relationships.back());
		//(*parent1).socialSphere.relationships.push_back(SocialNode(this, RelationshipStatus::child));//, &(*(*i)), false));
		//SocialNode* existingGuysNodeForNewPerson = &((*parent1).socialSphere.relationships.back());
		//(*newGuysNodeForExistingPerson).ownerNode = existingGuysNodeForNewPerson;
		//(*existingGuysNodeForNewPerson).ownerNode = newGuysNodeForExistingPerson;
		//(*parent1).socialSphere.relationshipTypes[0].push_back(existingGuysNodeForNewPerson);
		//socialSphere.relationshipTypes[0].push_back(newGuysNodeForExistingPerson);
	}
	else
	{
		(*this).genome.InitializeGenome(&(*parent1).genome, &(*parent2).genome);
		InitializeSingleRelationship(parent2, RelationshipStatus::mother, RelationshipStatus::child);
		InitializeSingleRelationship(parent1, RelationshipStatus::father, RelationshipStatus::child);
		(*parent2).isPregnant = true;
		(*parent2).monthsPregnant = 0;
		(*parent2).kidsPregnantWith.push_back(this);
	}
}


void Denizen::InitializeRelationships(std::list<std::unique_ptr<Denizen>>* currentLiving, Denizen* father, Denizen* mother)
{
	std::list<std::unique_ptr<Denizen>>::iterator i;
	for (i = (*currentLiving).begin(); i != (*currentLiving).end(); i++)
	{
		if (&(*(*i)) != father && &(*(*i)) != mother)
		{
			InitializeSingleRelationship(&(*(*i)), RelationshipStatus::none, RelationshipStatus::none);
		}
		//socialSphere.relationships.push_back(SocialNode(&(*(*i)), none));// , this, true));
		//SocialNode* newGuysNodeForExistingPerson = &(socialSphere.relationships.back());
		//(*(*i)).socialSphere.relationships.push_back(SocialNode(this, none));//, &(*(*i)), false));
		//SocialNode* existingGuysNodeForNewPerson = &((*(*i)).socialSphere.relationships.back());
		//(*newGuysNodeForExistingPerson).ownerNode = existingGuysNodeForNewPerson;
		//(*existingGuysNodeForNewPerson).ownerNode = newGuysNodeForExistingPerson;
		//(*(*i)).socialSphere.relationshipTypes[none].push_back(existingGuysNodeForNewPerson);
		//socialSphere.relationshipTypes[none].push_back(newGuysNodeForExistingPerson);
	}
}


void Denizen::InitializeSingleRelationship(Denizen* otherGuy, RelationshipStatus whatThisGuyThinks, RelationshipStatus whatOtherGuyThinks)
{
	(*this).socialSphere.relationships.push_back(SocialNode(otherGuy, whatThisGuyThinks));// , this, true));
	SocialNode* newGuysNodeForExistingPerson = &(socialSphere.relationships.back());
	(*otherGuy).socialSphere.relationships.push_back(SocialNode(this, whatOtherGuyThinks));//, &(*(*i)), false));
	SocialNode* existingGuysNodeForNewPerson = &((*otherGuy).socialSphere.relationships.back());
	(*newGuysNodeForExistingPerson).ownerNode = existingGuysNodeForNewPerson;
	(*existingGuysNodeForNewPerson).ownerNode = newGuysNodeForExistingPerson;
	(*otherGuy).socialSphere.relationshipTypes[0].push_back(existingGuysNodeForNewPerson);
	(*this).socialSphere.relationshipTypes[0].push_back(newGuysNodeForExistingPerson);
}



bool Denizen::TryHaveChildren(Denizen& partner)
{
	if (gender == female)
	{
		if (isPregnant)
		{
			return false;
		}
	}
	else if (partner.isPregnant)
	{
		return false;
	}


	if (ageMonths / 12 < 49 && ageMonths / 12 > 14 && 
		partner.ageMonths / 12 < 49 && partner.ageMonths / 12 > 14 &&
		(rand() % 4) == 0)
	{
		return true;
	}
	return false;;
}

bool Denizen::TryNaturalDeath()
{
	bool shallExpire = false;
	if (ageMonths / 12 > 100)
	{
		shallExpire = true;
	}
	else if (ageMonths / 12 > 55)
	{
		srand(time(NULL));

		int chanceVar = rand() % 100 + 40;
		if (chanceVar < ageMonths / 12)
		{
			shallExpire = true;
		}
	}
	return shallExpire;
}

requestResponse Denizen::HearRequest(request req, SocialNode * source)
{
	return yes;
}

int Denizen::EvaluateAsMate(SocialNode* mate)
{
	if (mate == NULL)
	{
		return 0;
	}
	int score = 0;
	Denizen* potentialMate = (*mate).person;
	genderEnum oppositeGender = gender == male ? female : male;
	if ((*potentialMate).gender != oppositeGender)
	{
		return 0;
	}
	if ((*mate).currentRelationship == courting)
	{
		score += 2;
		if ((*mate).currentRelationshipAge > 3)
		{
			score += 2;
		}
	}
	else if ((*mate).currentRelationship == married)
	{
		score += 6;
		if ((*mate).currentRelationshipAge > 18)
		{
			score -= 2;
		}
	}
	else if ((*mate).currentRelationship == ex)
	{
		return score -= 2;
	}
	else if ((*mate).isDead)
	{
		return 0;
	}
	return ((*potentialMate).ageMonths / 12) > 15 && ((*potentialMate).ageMonths/12) < (ageMonths / 12) ? score + (rand() % 10) : 0;
}

void Denizen::ChangeRelationshipWithOther(RelationshipStatus newRelationship, SocialNode * other)
{
	int oldRelationship = (*other).currentRelationship;
	std::list<SocialNode*>::iterator i;
	/*for (i = socialSphere.relationshipTypes[oldRelationship].begin(); i < socialSphere.relationshipTypes[oldRelationship].end();
		i != socialSphere.relationshipTypes[oldRelationship].end() ? i++ : i = i)
	{
		if( (*i) == other )
		{
			socialSphere.relationshipTypes[oldRelationship].remove(other);
		}
	}*/
	//beware the linearity of remove!
	socialSphere.relationshipTypes[oldRelationship].remove(other);
	socialSphere.relationshipTypes[newRelationship].push_back(other);
	(*other).currentRelationshipAge = 0;
	(*other).currentRelationship = newRelationship;
}


//void Denizen::DrawObject()
//{
//	HDC hdc = (*GameViewState::TheGameView()).deviceContext;
//	//HDC memdc = CreateCompatibleDC(hdc);
//	HDC memdc = (*GameViewState::TheGameView()).memoryDC;
//
//	HDC tempDC = CreateCompatibleDC(memdc);// (hdc);
//	////HDC tempDC2 = CreateCompatibleDC(memdc);
//	//HBITMAP saveOldBitmap = (HBITMAP)SelectObject(memdc, denizenImageBase);
//	HBITMAP saveOldTempBmp = (HBITMAP)SelectObject(tempDC, imageMask);
//	BitBlt(memdc, 50, 50, 50, 50, tempDC, (currentFrame * 50), 0, SRCPAINT);// SRCAND);
//	SelectObject(tempDC, imageBase);
//	BitBlt(memdc, 50, 50, 50, 50, tempDC, (currentFrame * 50), 0, SRCAND);
//
//	currentFrame++;
//	currentFrame = currentFrame % totalFrames;
//	//////DeleteObject(saveOldTempBmp);
//	////DeleteObject(saveOldBitmap);
//	////DeleteDC(tempDC2);
//	SelectObject(tempDC, saveOldTempBmp);
//	DeleteDC(tempDC);
//}

