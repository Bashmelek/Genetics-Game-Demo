#pragma once

#include "Biology.h"
#include <string>



#ifndef GENOMEDEF
#define GENOMEDEF

class Genome
{
public:
	std::vector<Gene*> maternalGenes;
	std::vector<Gene*> paternalGenes;

	void InitializeGenome(Genome* dadgenome, Genome* momgenome);

private:
	char ChromosonalSpecialSection(GeneSection::GeneSection geneSection);
};




#endif GENOMEDEF

