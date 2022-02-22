#include "stdafx.h"
#include "Denizen.h"

Denizen::Denizen()
{
	
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

Denizen::Denizen(genderEnum::genderEnum gender, std::wstring shortName, int ageMonths, std::list<Gene*> momgenes, std::list<Gene*> dadgenes, std::list<std::unique_ptr<Denizen>>* currentLiving, int worldX, int worldY)
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
	(*this).worldX = 0;
	(*this).worldY = 0;
	this->SetWorldPositionToClosestPoint(worldX, worldY);
	(*this).shortName = shortName;
	(*this).gender = gender;
	(*this).ageMonths = ageMonths;
	age = ageMonths/12;
	if (gender == genderEnum::female)
	{
		courtshipStyle = CourtshipStyle::beingChased;
	}
	else
	{
		courtshipStyle = CourtshipStyle::chaser;
	}
	InitializeRelationships(currentLiving, 0, 0);
	InitializeStarterAlleles();
}

Denizen::Denizen(Denizen* parent1, Denizen* parent2)
{
	BaseInit();

	(*this).isBorn = false;
	(*this).monthsGestating = 0;
	(*this).worldX = 0;
	(*this).worldY = 0;

	if ((*parent1).gender == genderEnum::female)
	{
		(*this).genome.InitializeGenome(&(*parent2).genome, &(*parent1).genome);
		InitializeSingleRelationship(parent1, RelationshipStatus::mother, RelationshipStatus::child);
		InitializeSingleRelationship(parent2, RelationshipStatus::father, RelationshipStatus::child);
		(*parent1).isPregnant = true;
		(*parent1).monthsPregnant = 0;
		(*parent1).kidsPregnantWith.push_back(this);
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
	(*this).mother = socialSphere.relationshipTypes[RelationshipStatus::mother].front();
	(*this).father = socialSphere.relationshipTypes[RelationshipStatus::father].front();
	InitializeStarterAlleles();

	if (alleles[GeneSection::maleness] == 1)
	{
		gender = genderEnum::male;
	}
	else
	{
		gender = genderEnum::female;
	}
}


void Denizen::InitializeRelationships(std::list<std::unique_ptr<Denizen>>* currentLiving)
{
	InitializeRelationships(currentLiving, (*(*this).father).person, (*(*this).mother).person);
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
	}
}


void Denizen::InitializeSingleRelationship(Denizen* otherGuy, RelationshipStatus::RelationshipStatus whatThisGuyThinks, RelationshipStatus::RelationshipStatus whatOtherGuyThinks)
{
	(*this).socialSphere.relationships.push_back(SocialNode(otherGuy, whatThisGuyThinks));// , this, true));
	SocialNode* newGuysNodeForExistingPerson = &(socialSphere.relationships.back());
	(*otherGuy).socialSphere.relationships.push_back(SocialNode(this, whatOtherGuyThinks));//, &(*(*i)), false));
	SocialNode* existingGuysNodeForNewPerson = &((*otherGuy).socialSphere.relationships.back());
	(*newGuysNodeForExistingPerson).ownerNode = existingGuysNodeForNewPerson;
	(*existingGuysNodeForNewPerson).ownerNode = newGuysNodeForExistingPerson;
	(*otherGuy).socialSphere.relationshipTypes[whatOtherGuyThinks].push_back(existingGuysNodeForNewPerson);
	(*this).socialSphere.relationshipTypes[whatThisGuyThinks].push_back(newGuysNodeForExistingPerson);
}


void Denizen::InitializeStarterAlleles()
{
	for (int c = 0; c < maxGeneEffect; c++)
	{
		alleles[c] = -1;
	}

	for (int i = 0; i < maxGenesection; i++)
	{
		geneWinType::geneWinType winType;
		if (genome.maternalGenes.size() <= i)
		{
			i = maxGenesection;
		}
		else
		{
			Gene* dadGene = genome.paternalGenes[i];
			Gene* momGene = genome.maternalGenes[i];
			if ((*momGene).id == 0)
			{
				if ((*dadGene).id == 0)
				{
					winType = geneWinType::noneWin;
				}
				else
				{
					winType = geneWinType::fatherWin;
				}
			}
			else if ((*dadGene).id == 0)
			{
				winType = geneWinType::motherWin;
			}
			else
			{
				if (std::find((*dadGene).dominantAgainst.begin(), (*dadGene).dominantAgainst.end(), (*momGene).id) != (*dadGene).dominantAgainst.end())
				{
					winType = geneWinType::fatherWin;
				}
				else if (std::find((*momGene).dominantAgainst.begin(), (*momGene).dominantAgainst.end(), (*dadGene).id) != (*momGene).dominantAgainst.end())
				{
					winType = geneWinType::motherWin;
				}
				else//do codominance another day
				{
					winType = geneWinType::mixWin;
				}
			}
			SetAlleleForGene(dadGene, momGene, winType);
		}
	}

	//yes normally maleness is only determined by the father's genes, but...you never know
	/*if ((*genome.maternalGenes[maleness]).mainFeatureModifier == 1 || (*genome.paternalGenes[maleness]).mainFeatureModifier == 1)
	{
		gender = male;
	}
	else
	{
		gender = female;
	}*/


}

void Denizen::SetAlleleForGene(Gene* dadgene, Gene* momgene, geneWinType::geneWinType wintype)
{
	if (wintype == geneWinType::motherWin)
	{
		alleles[(*momgene).mainEffect] = (*momgene).mainFeatureModifier;
	}
	else if (wintype == geneWinType::fatherWin)
	{
		alleles[(*dadgene).mainEffect] = (*dadgene).mainFeatureModifier;
	}
	else if (wintype == geneWinType::mixWin)
	{
		alleles[(*dadgene).mainEffect] = ((*dadgene).mainFeatureModifier + (*momgene).mainFeatureModifier) / 2;
	}
}


bool Denizen::TryHaveChildren(Denizen& partner)
{
	if (gender == genderEnum::female)
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


	if (ageMonths / 12 < 49 && ageMonths / 12 > 11 && 
		partner.ageMonths / 12 < 49 && partner.ageMonths / 12 > 11 &&
		(rand() % 2) == 0)
	{
		return true;
	}
	return false;
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

int Denizen::TryToBeBorn()
{
	if (monthsGestating > 11)
	{
		return 1;
	}
	else if (monthsGestating > 6)
	{
		int chanceVar = rand() % 100 + 40;
		if (chanceVar < ageMonths / 12)
		{
			return 1;
		}
	}
	return 0;
}

std::wstring Denizen::BeBorn(std::list<std::unique_ptr<Denizen>>* currentLiving)
{
	monthsGestating = 0;
	ageMonths = 0;
	(*(*mother).person).monthsPregnant = 0;
	(*(*mother).person).isPregnant = false;
	//must add bit to give birth to all kids being carried
	(*(*mother).person).kidsPregnantWith.remove(this);
	InitializeRelationships(currentLiving);
	if (gender == genderEnum::male)
	{
		shortName = L"Sam";//get name
	}
	else
	{
		shortName = L"Sally";
	}
	if (gender == genderEnum::female)
	{
		courtshipStyle = CourtshipStyle::beingChased;
	}
	else
	{
		courtshipStyle = CourtshipStyle::chaser;
	}

	this->SetWorldPositionToClosestPoint((*(*mother).person).worldX, (*(*mother).person).worldY);

	isBorn = true;
	std::wstring birthMessage = shortName;
	birthMessage.append(L" was born!");

	return birthMessage;
}

requestResponse::requestResponse Denizen::HearRequest(request::request req, SocialNode * source)
{
	return requestResponse::yes;
}

int Denizen::EvaluateAsMate(SocialNode* mate)
{
	if (mate == NULL)
	{
		return 0;
	}
	int score = 0;
	Denizen* potentialMate = (*mate).person;
	genderEnum::genderEnum oppositeGender = gender == genderEnum::male ? genderEnum::female : genderEnum::male;
	if ((*potentialMate).gender != oppositeGender)
	{
		return 0;
	}
	if ((*mate).currentRelationship == RelationshipStatus::courting)
	{
		score += 2;
		if ((*mate).currentRelationshipAge > 3)
		{
			score += 2;
		}
	}
	else if ((*mate).currentRelationship == RelationshipStatus::married)
	{
		score += 6;
		if ((*mate).currentRelationshipAge > 72)
		{
			score -= 2;
		}
	}
	else if ((*mate).currentRelationship == RelationshipStatus::ex)
	{
		return score -= 2;
	}
	else if ((*mate).isDead)
	{
		return 0;
	}
	return ((*potentialMate).ageMonths / 12) > 15 && ((*potentialMate).ageMonths/12) < (ageMonths / 12) ? score + (rand() % 10) : 0;
}

void Denizen::ChangeRelationshipWithOther(RelationshipStatus::RelationshipStatus newRelationship, SocialNode * other)
{
	int oldRelationship = (*other).currentRelationship;
	std::list<SocialNode*>::iterator i;

	//beware the linearity of remove!
	socialSphere.relationshipTypes[oldRelationship].remove(other);
	socialSphere.relationshipTypes[newRelationship].push_back(other);
	(*other).currentRelationshipAge = 0;
	(*other).currentRelationship = newRelationship;
}



void Denizen::CreateStrategyForMonth()
{
	for (int i = 0; i < ACTIONPOINTS; i++)
	{
		if (i > 3 && i < 15)
		{
			strategy.plannedActions[i].ResetWithType(ActivityType::gatherFood);
		}
		else if (i >= 16 && i < 18)
		{
			strategy.plannedActions[i].ResetWithType(ActivityType::eat);
		}
		else if (i >= 19 && i < 31)
		{
			strategy.plannedActions[i].ResetWithType(ActivityType::gatherFood);
		}
		else if (i >= 72)
		{
			strategy.plannedActions[i].ResetWithType(ActivityType::sleep);
		}
	}
}

void Denizen::Act(int dayTimeSlot)
{

	ActivityType::ActivityType chosenActivity = strategy.plannedActions[dayTimeSlot].action;
	switch (chosenActivity)
	{
		case ActivityType::sleep:
			break;

		case ActivityType::gatherFood:
			GatherFood();
			break;

		case ActivityType::eat:
			break;

		default:
			break;
	}
}

void Denizen::GatherFood()
{

}

