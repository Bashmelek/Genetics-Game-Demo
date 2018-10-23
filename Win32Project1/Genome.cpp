#include "stdafx.h"
#include "Genome.h"


//
//
//special gene sections

const char NonSpecialSection = '0';

void Genome::InitializeGenome(Genome* dadgenome, Genome* momgenome)
{
	srand(time(NULL));
	std::string specialSectionsContainedOriginally;
	std::string specialSectionsChosen;
	int meiosisRandomizer;
	char section = NonSpecialSection;
	Gene* nullGene = NULL;;
	for (int g = 0; g < 2; g++)
	{
		Genome* currentParentGenome = g == 0 ? dadgenome : momgenome;
		std::vector<Gene*>* currentPersonalGenome = g == 0 ? &paternalGenes : &maternalGenes;
		specialSectionsContainedOriginally = "";
		specialSectionsChosen = "";

		for (int i = 0; i < maxGenesection; i++)
		{
			section = ChromosonalSpecialSection((GeneSection)i);
			if (section != NonSpecialSection && specialSectionsContainedOriginally.find(section) == std::string::npos)
			{
				if ((*(*currentParentGenome).paternalGenes[i]).id != 0 || (*(*currentParentGenome).maternalGenes[i]).id != 0)
				{
					specialSectionsContainedOriginally += section;
				}
				else if (nullGene == NULL)
				{
					if ((*(*currentParentGenome).paternalGenes[i]).id == 0)
					{
						nullGene = (*currentParentGenome).paternalGenes[i];
					}
					else
					{
						nullGene = (*currentParentGenome).maternalGenes[i];
					}
				}
			}

		}

		if (specialSectionsContainedOriginally.find("X") != std::string::npos)
		{
			if (specialSectionsContainedOriginally.find("Y") != std::string::npos && rand() % 2 == 0)
			{
				specialSectionsChosen += "Y";
			}
			else
			{
				specialSectionsChosen += "X";
			}
		}
		else if (specialSectionsContainedOriginally.find("Y") != std::string::npos)
		{
			specialSectionsChosen += "Y";
		}

		for (int i = 0; i < maxGenesection; i++)
		{
			if ((*currentParentGenome).maternalGenes.size() <= i || (*currentParentGenome).paternalGenes.size() <= i) 
			{ 
				continue; 
			}
			section = ChromosonalSpecialSection((GeneSection)i);
			if (section == NonSpecialSection || specialSectionsContainedOriginally.find(section) != std::string::npos)
			{
				if (rand() % 2 == 0)
				{
					(*currentPersonalGenome).push_back((*currentParentGenome).maternalGenes[i]);
				}
				else
				{
					(*currentPersonalGenome).push_back((*currentParentGenome).paternalGenes[i]);
				}
			}
			else
			{
				(*currentPersonalGenome).push_back(nullGene);
			}
		}
	}

}

char Genome::ChromosonalSpecialSection(GeneSection geneSection)
{
	switch (geneSection)
	{
	case maleness:
		return 'Y';
		break;
	default:
		return NonSpecialSection;
	}
	return NonSpecialSection;
}

