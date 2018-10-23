#pragma once


#include <list>
#include <map>


#ifndef GENEDEF
#define GENEDEF


enum GeneSection { nogene = -1, targetHeight, eyecolor, maleness, blood, skincolor, targetMuscleMass, targetMuscleTone };
enum GeneEffect { baseHeight, eyeColor, gender, bloodType, skinColor, baseMuscleMass, baseMuscleTone};
const GeneSection maxGenesection = targetMuscleTone;
const GeneEffect maxGeneEffect = baseMuscleTone;


class Gene
{
public:
	int id;
	GeneSection geneType;

	int mainFeatureModifier;
	GeneEffect mainEffect;

	std::map<GeneEffect, int> otherGeneEffects;

	std::list<int> dominantAgainst;
	std::list<int> codominantWith;
};


#endif GENEDEF

