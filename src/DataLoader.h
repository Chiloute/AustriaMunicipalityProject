//
// Created by chilo on 24/03/2025.
//

#ifndef DATALOADER_H
#define DATALOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include "Territorial.h"

std::unique_ptr<TerritorialUnit> loadTerritorialHierarchy(const std::string& csvPath, const std::string& municipalitiesPath);
void loadPopulationData(TerritorialUnit* root, const std::string& popFilePath, int yearIndex) ;

#endif //DATALOADER_H
