#ifndef SORT_H
#define SORT_H

#include "Territorial.h"
#include <vector>
#include <functional>

enum class Gender{ MALE,FEMALE,TOTAL};

class Sort {
public:
    static void sort(std::vector<TerritorialUnit*>& units,
        const std::function<bool(const TerritorialUnit*, const TerritorialUnit*)>& comparator);
    
    static std::vector<TerritorialUnit*> collect(TerritorialUnit* node,
        const std::function<bool(const TerritorialUnit*)>& predicate);
    static void print(const std::vector<TerritorialUnit*>& units, int yearIndex = -1, Gender gender = Gender::TOTAL);
};

#endif 