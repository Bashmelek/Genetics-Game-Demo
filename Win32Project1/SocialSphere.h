#pragma once

#include <list>
#include <memory>


#ifndef SOCIALSPHERE
#define SOCIALSPHERE

class Denizen;

class SocialNode
{
public:
	SocialNode();
	SocialNode(Denizen* person, int relationship);// , Denizen* owner, bool ownerIsNewPerson);
	SocialNode* ownerNode;
	Denizen * person;
	int relationship;
};

class SocialSphere
{
public:
	SocialSphere();
	void InitSocialSphere(std::list<std::unique_ptr<Denizen>>* intialAcquaintances);
	void InitSocialSphere(std::list<Denizen*> intialAcquaintances, Denizen* mother, Denizen* father);
	std::list<SocialNode> relationships;
	std::list<int> relationshipTypes;
};


#endif SOCIALSPHERE



