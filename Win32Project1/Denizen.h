#pragma once

#include <list>
#include "Genome.h"
#include "GameObject.h"
#include "Strategy.h"
#include "Inventory.h"
/////#include "SocialSphere.h"

#ifndef DENIZENDEF
#define DENIZENDEF

namespace genderEnum
{
	enum genderEnum { female = 0, male = 1 };
}
namespace RelationshipStatus
{
	enum RelationshipStatus { none, courting, beingCourted, ex, married, mother, father, child };
}
namespace CourtshipStyle
{
	enum CourtshipStyle { chaser, beingChased };
}

namespace request
{
	enum request { court, marriage, copulate };
}
namespace requestResponse
{
	enum requestResponse { no, yes };
}

namespace responseReaction
{
	enum responseReaction { accept, reject };
}

namespace geneWinType
{
	enum geneWinType { noneWin, fatherWin, motherWin, mixWin, bothWin };
}

class Denizen;


class SocialNode
{
public:
	SocialNode();
	SocialNode(Denizen* person, RelationshipStatus::RelationshipStatus relationship);// , Denizen* owner, bool ownerIsNewPerson);
	SocialNode* ownerNode;
	Denizen * person;
	int currentRelationship;
	int currentRelationshipAge;
	bool isDead;
};

class SocialSphere
{
public:
	SocialSphere();
	void InitSocialSphere(std::list<std::unique_ptr<Denizen>>* intialAcquaintances);
	void InitSocialSphere(std::list<Denizen*> intialAcquaintances, Denizen* mother, Denizen* father);
	std::list<SocialNode> relationships;
	std::list<SocialNode*> relationshipTypes [10];
};

class Denizen: public GameObject
{
public:
	Denizen();
	Denizen(bool isDummy);
	Denizen(bool isDummy, int num, std::list<std::unique_ptr<Denizen>>* currentLiving);

	Denizen(genderEnum::genderEnum gender, std::wstring shortName, int ageMonths, std::list<Gene*> momgenes, std::list<Gene*> dadgenes, std::list<std::unique_ptr<Denizen>>* currentLiving, int worldX, int worldY);
	Denizen(Denizen* parent1, Denizen* parent2);

	int age;
	int ageMonths;
	genderEnum::genderEnum gender;
	SocialNode* father;
	SocialNode* mother;
	std::wstring shortName;
	SocialNode* currentPartner;
	SocialNode* potentialPartnerInMind;
	CourtshipStyle::CourtshipStyle courtshipStyle;
	RelationshipStatus::RelationshipStatus partnerRelationshipStatus;
	int totalPartnerRelationshipDuration;
	int currentPartnerRelationshipDuration;
	SocialSphere socialSphere;
	Inventory inventory;

	Genome genome;

	bool isPregnant;
	int monthsPregnant;

	bool isBorn;
	int monthsGestating;
	std::list<Denizen*> kidsPregnantWith;

	int alleles[maxGeneEffect];

	////virtual void DrawObject();
//protected:
	///*bool isVisibleInWorld;
	//int worldX;
	//int worldY;
	//int currentFrame;
	//int totalFrames;*/

	HBITMAP imageBase;
	HBITMAP imageMask;
	HBITMAP ladyImageBase;

	bool TryHaveChildren(Denizen&  partner);
	bool TryNaturalDeath();

	requestResponse::requestResponse HearRequest(request::request req, SocialNode * source);
	int EvaluateAsMate(SocialNode* mate);
	void ChangeRelationshipWithOther(RelationshipStatus::RelationshipStatus newRelationship, SocialNode * other);
	int TryToBeBorn();
	void InitializeRelationships(std::list<std::unique_ptr<Denizen>>* currentLiving);
	std::wstring BeBorn(std::list<std::unique_ptr<Denizen>>* currentLiving);

	Strategy strategy;
	void CreateStrategyForMonth();
	void Act(int dayTimeSlot);

protected:
	void BaseInit();
	void InitializeRelationships(std::list<std::unique_ptr<Denizen>>* currentLiving, Denizen* father, Denizen* mother);
	void InitializeSingleRelationship(Denizen* otherGuy, RelationshipStatus::RelationshipStatus whatThisGuyThinks, RelationshipStatus::RelationshipStatus whatOtherGuyThinks);

	void InitializeStarterAlleles();
	void SetAlleleForGene(Gene* dadgene, Gene* momgene, geneWinType::geneWinType wintype);
	
	void GatherFood();
};




#endif DENIZENDEF