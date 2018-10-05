#pragma once


#include <list>
#include <map>


#ifndef GENEDEF
#define GENEDEF


enum GeneSection { nogene = -1, targetHeight, eyecolor, maleness, blood, skincolor, targeMuscleMass, targetMuscleTone };
enum GeneEffect {};
const GeneSection maxGenesection = targetMuscleTone;


class Gene
{
public:
	int id;
	GeneSection geneType;

	int mainFeatureModifier;

	std::map<GeneEffect, int> geneEffects;

	std::list<int> dominantAgainst;
	std::list<int> codominantWith;
};


#endif GENEDEF

