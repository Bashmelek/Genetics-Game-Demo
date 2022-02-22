#pragma once


#include <list>
#include <map>


#ifndef GENEDEF
#define GENEDEF


namespace GeneSection 
{
	enum GeneSection { nogene = -1, targetHeight, eyecolor, maleness, blood, skincolor, targetMuscleMass, targetMuscleTone };
}
namespace GeneEffect
{
	enum GeneEffect { baseHeight, eyeColor, gender, bloodType, skinColor, baseMuscleMass, baseMuscleTone };
}
const GeneSection::GeneSection maxGenesection = GeneSection::targetMuscleTone;
const GeneEffect::GeneEffect maxGeneEffect = GeneEffect::baseMuscleTone;


class Gene
{
public:
	int id;
	GeneSection::GeneSection geneType;

	int mainFeatureModifier;
	GeneEffect::GeneEffect mainEffect;

	std::map<GeneEffect::GeneEffect, int> otherGeneEffects;

	std::list<int> dominantAgainst;
	std::list<int> codominantWith;
};


#endif GENEDEF

