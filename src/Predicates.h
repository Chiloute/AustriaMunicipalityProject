
#ifndef PREDICATES_H
#define PREDICATES_H
#include <string>
#include <functional>
#include "Territorial.h"

// Prédicat : vérifie si le nom de la municipalité contient une sous-chaîne
inline auto containsStr(const std::string& str) {
    return [str](const Municipality& m) {
        return m.name.find(str) != std::string::npos;
    };
}

// Prédicat : vérifie si la population totale d'une année est inférieure ou égale à maxPop
inline auto hasMaxResidents(int year, int maxPop) {
    return [year, maxPop](const Municipality& m) {
        int index = year - 2020;
        return m.getTotalPopulation(index) <= maxPop;
    };
}

// Prédicat : vérifie si la population totale d'une année est supérieure ou égale à minPop
inline auto hasMinResidents(int year, int minPop) {
    return [year, minPop](const Municipality& m) {
        int index = year - 2020;
        return m.getTotalPopulation(index)>= minPop;
    };
}

inline bool hasType(const TerritorialUnit* unit, TerritorialUnitType type) {
    return unit->type_ == type;
}


#endif //PREDICATES_H
