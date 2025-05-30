#include "Sort.h"
#include <algorithm>
#include <iostream>

std::vector<TerritorialUnit*> Sort::collect(TerritorialUnit* node, const std::function<bool(const TerritorialUnit*)>& predicate) {
    std::vector<TerritorialUnit*> result;
    if (!node) return result;
    for (const auto& child : node->children) {
        if (predicate(child.get())) {
            result.push_back(child.get());
        }
    }
    return result;
}

void Sort::sort(std::vector<TerritorialUnit*>& units, const std::function<bool(const TerritorialUnit*, const TerritorialUnit*)>& comparator) {
    std::sort(units.begin(), units.end(), comparator);
}

void Sort::print(const std::vector<TerritorialUnit*>& units, int yearIndex, Gender gender) {
    for (const auto& unit : units) {
        std::cout << unit->name << " (" << unit->code << ")";
        if (yearIndex >= 0) {
            auto muni = dynamic_cast<const Municipality*>(unit);
            if (muni) {
                PopulationData pop = muni->getPopulation(yearIndex);
                std::cout << " | ";
                if (gender == Gender::MALE) std::cout << "Male: " << pop.male;
                else if (gender == Gender::FEMALE) std::cout << "Female: " << pop.female;
                else std::cout << "Total: " << pop.total();
            }
        }
        std::cout << std::endl;
    }
}
