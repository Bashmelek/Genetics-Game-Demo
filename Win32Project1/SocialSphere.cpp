#include "stdafx.h"
#include "Denizen.h"


SocialNode::SocialNode()
{

}

SocialNode::SocialNode(Denizen* acquaintance, RelationshipStatus::RelationshipStatus relationship)//, Denizen* owner, bool ownerIsNewPerson)
{
	//if (ownerIsNewPerson)
	//{
	//	//(*acquaintance).socialSphere.relationships.push_back(SocialNode(owner, 0, acquaintance, false));
	//	//(*this).ownerNode = &(*acquaintance).socialSphere.relationships.back();
	//}
	//else//target is new person, saying "please when this node is done being made, add it to your node 
	//{

	//}
	(*this).ownerNode = NULL;
	(*this).person = acquaintance;
	(*this).currentRelationship = relationship;
	(*this).currentRelationshipAge = 0;
	(*this).isDead = false;
}

SocialSphere::SocialSphere()
{

}


void SocialSphere::InitSocialSphere(std::list<std::unique_ptr<Denizen>>* intialAcquaintances)
{

}

void SocialSphere::InitSocialSphere(std::list<Denizen*> intialAcquaintances, Denizen* mother, Denizen* father)
{

}