
#ifndef FILTERS_H
#define FILTERS_H

#include "Territorial.h"
#include "Predicates.h"


void filterAndPrintMunicipalities(TerritorialUnit* currentNode);
void collectMunicipalities(TerritorialUnit* node,std::vector<Municipality*>& results);
int getValidYear();
std::vector<TerritorialUnit*> searchTerritorialUnits(const std::string& name, TerritorialUnitType type) ;
#endif //FILTERS_H
