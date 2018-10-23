#pragma once



#include "Gene.h"
#include <time.h>
#include <vector>
#include <memory>

#ifndef BIODEF
#define BIODEF

class Biology
{
public:
	Biology();
	Biology(std::wstring version);

	void InitBiology();
	void InitBiology(std::wstring version);

	std::vector<std::unique_ptr<Gene>> genepool;
	std::vector<std::unique_ptr<std::vector<Gene*>>> geneCategories;

	Gene* GenerateBasicGene(int id);

	static Biology* bio();
};





#endif BIODEF

