#pragma once

#include <list>
#include "Genome.h"
#include "GameObject.h"
/////#include "SocialSphere.h"

#ifndef DENIZENDEF
#define DENIZENDEF

enum genderEnum{ female = 0, male = 1 };
enum RelationshipStatus { none, courting, beingCourted, ex, married, mother, father, child };
enum CourtshipStyle { chaser, beingChased };

enum request { court, marriage, copulate };
enum requestResponse { no, yes };
enum responseReaction { accept, reject };

enum geneWinType { noneWin, fatherWin, motherWin, mixWin, bothWin };

class Denizen;


class SocialNode
{
public:
	SocialNode();
	SocialNode(Denizen* person, RelationshipStatus relationship);// , Denizen* owner, bool ownerIsNewPerson);
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

	Denizen(genderEnum gender, std::wstring shortName, int ageMonths, std::list<Gene*> momgenes, std::list<Gene*> dadgenes, std::list<std::unique_ptr<Denizen>>* currentLiving, int worldX, int worldY);
	Denizen(Denizen* parent1, Denizen* parent2);

	int age;
	int ageMonths;
	genderEnum gender;
	SocialNode* father;
	SocialNode* mother;
	std::wstring shortName;
	SocialNode* currentPartner;
	SocialNode* potentialPartnerInMind;
	CourtshipStyle courtshipStyle;
	RelationshipStatus partnerRelationshipStatus;
	int totalPartnerRelationshipDuration;
	int currentPartnerRelationshipDuration;
	SocialSphere socialSphere;

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

	requestResponse HearRequest(request req, SocialNode * source);
	int EvaluateAsMate(SocialNode* mate);
	void ChangeRelationshipWithOther(RelationshipStatus newRelationship, SocialNode * other);
	int TryToBeBorn();
	void InitializeRelationships(std::list<std::unique_ptr<Denizen>>* currentLiving);
	std::wstring BeBorn(std::list<std::unique_ptr<Denizen>>* currentLiving);

protected:
	void BaseInit();
	void InitializeRelationships(std::list<std::unique_ptr<Denizen>>* currentLiving, Denizen* father, Denizen* mother);
	void InitializeSingleRelationship(Denizen* otherGuy, RelationshipStatus whatThisGuyThinks, RelationshipStatus whatOtherGuyThinks);

	void InitializeStarterAlleles();
	void SetAlleleForGene(Gene* dadgene, Gene* momgene, geneWinType wintype);
};




#endif DENIZENDEF