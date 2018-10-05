#include "stdafx.h"
#include "Biology.h"



Biology::Biology()
{
	InitBiology();
}


Biology::Biology(std::wstring version)
{
	InitBiology(version);
}

Biology* Biology::bio()
{
	static Biology thebio = Biology();
	return &thebio;
}


void Biology::InitBiology()
{
	genepool.reserve(20);
	geneCategories.reserve(maxGenesection + 1);
	geneCategories.push_back(std::make_unique<std::vector<Gene*>>());
	geneCategories.push_back(std::make_unique<std::vector<Gene*>>());
	geneCategories.push_back(std::make_unique<std::vector<Gene*>>());
	geneCategories.push_back(std::make_unique<std::vector<Gene*>>());
	geneCategories.push_back(std::make_unique<std::vector<Gene*>>());
	for (int i = 0; i <= 8; i++)
	{
		Gene* newGene = GenerateBasicGene(i);
		genepool.push_back(std::make_unique<Gene>(*newGene));
		if ((*newGene).geneType != -1)
		{
			(*geneCategories[(*newGene).geneType]).push_back(newGene);
		}

	}
}


void Biology::InitBiology(std::wstring version)
{

}

Gene* Biology::GenerateBasicGene(int id)
{
	Gene* gene = NULL;
	switch (id)
	{
	case 0:
		gene = new Gene();
		(*gene).geneType = nogene;
		(*gene).mainFeatureModifier = 0;
		break;
	case 1:
		gene = new Gene();
		(*gene).geneType = targetHeight;
		(*gene).mainFeatureModifier = 70;
		break;
	case 2:
		gene = new Gene();
		(*gene).geneType = targetHeight;
		(*gene).mainFeatureModifier = 60;
		break;
	case 3:
		gene = new Gene();
		(*gene).geneType = targetHeight;
		(*gene).mainFeatureModifier = 80;
		break;
	case 4:
		gene = new Gene();
		(*gene).geneType = targetHeight;
		(*gene).mainFeatureModifier = 50;
		break;
	case 5:
		gene = new Gene();
		(*gene).geneType = targetHeight;
		(*gene).mainFeatureModifier = 90;
		break;
	case 6:
		gene = new Gene();
		(*gene).geneType = eyecolor;
		(*gene).mainFeatureModifier = 0;//brown
		(*gene).dominantAgainst.push_back(6);
		break;
	case 7:
		gene = new Gene();
		(*gene).geneType = eyecolor;
		(*gene).mainFeatureModifier = 1;//blue
		break;
	case 8:
		gene = new Gene();
		(*gene).geneType = maleness;
		(*gene).mainFeatureModifier = 1;//blue
		break;
	default:
		break;
	}
	(*gene).id = id;

	return gene;
}
