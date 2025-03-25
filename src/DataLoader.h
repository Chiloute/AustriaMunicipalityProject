//
// Created by chilo on 24/03/2025.
//

#ifndef DATALOADER_H
#define DATALOADER_H

#include "Municipality.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>


    std::vector<Municipality> parseCSV(const std::string& filePath);
#endif //DATALOADER_H
