//
// Created by chilo on 24/03/2025.
//

#ifndef MUNICIPALITY_H
#define MUNICIPALITY_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>


struct Municipality {
    std::string name_;
    int cityCode;
    std::vector<int> malePopulation;
    std::vector<int> femalePopulation;
};

#endif //MUNICIPALITY_H
