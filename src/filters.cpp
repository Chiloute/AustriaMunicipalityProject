#include "filters.h"

void collectMunicipalities(TerritorialUnit* node,
                           std::vector<Municipality*>& results) {
    if (!node) return;

    if (auto muni = dynamic_cast<Municipality*>(node)) {
        results.push_back(muni);
        return;
    }

    for (const auto& child : node->children) {
        collectMunicipalities(child.get(), results);
    }
}


void filterAndPrintMunicipalities(TerritorialUnit* currentNode) {
    if (!currentNode) {
        std::cerr << "Actual unit is invalid.\n";
        return;
    }

    std::vector<Municipality*> municipalities;
    collectMunicipalities(currentNode, municipalities);

    if (municipalities.empty()) {
        std::cout << "No municipality as this name.\n";
        return;
    }

    std::function<bool(const Municipality&)> combinedPredicate = [](const Municipality&) { return true; };

    while (true) {
        std::cout << "\n--- Filters Menu ---\n";
        std::cout << "1. Filter by name\n";
        std::cout << "2. Filter by minimal population\n";
        std::cout << "3. Filter by maximal population\n";
        std::cout << "4. Apply all filters\n";
        std::cout << "Choice : ";

        std::string choix;
        std::getline(std::cin, choix);

        if (choix == "1") {
            std::cout << "Has name : ";
            std::string str;
            std::getline(std::cin, str);
            auto pred = containsStr(str);
            auto old = combinedPredicate;
            combinedPredicate = [old, pred](const Municipality& unit) {
                return old(unit) && pred(unit);
            };
        }
        else if (choix == "2") {
            int year = getValidYear();

            std::cout << "Minimal population : ";
            int minPop;
            while (!(std::cin >> minPop) || minPop < 0) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid value. Enter a minimal population positive : ";
            }
            std::cin.ignore(10000, '\n');

            auto pred = hasMinResidents(year, minPop);
            auto old = combinedPredicate;
            combinedPredicate = [old, pred](const Municipality& unit) {
                return old(unit) && pred(unit);
            };
        }
        else if (choix == "3") {
            int year = getValidYear();

            std::cout << "Maximal population : ";
            int maxPop;
            while (!(std::cin >> maxPop) || maxPop < 0) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid value. Enter a positive value : ";
            }
            std::cin.ignore(10000, '\n');

            auto pred = hasMaxResidents(year, maxPop);
            auto old = combinedPredicate;
            combinedPredicate = [old, pred](const Municipality& unit) {
                return old(unit) && pred(unit);
            };
        }
        else if (choix == "4") {
            break;
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }

    std::cout << "\nMunicipalies  :\n";
    bool found = false;

    for (const auto& muni : municipalities) {
        if (combinedPredicate(*muni)) {
            std::cout << "- " << muni->name << " (" << muni->code << ")\n";
            for (int i = 0; i < 5; ++i) {
                auto popData = muni->getPopulation(i);
                std::cout << "  Year " << 2020 + i << ": Male = " << popData.male << ", Female = " << popData.female << ", Total = " << popData.total() << "\n";
            }
            found = true;
        }
    }

    if (!found) {
        std::cout << "No municipalities.\n";
    }

    //need to press enter to next
    std::cout << "Continue ?\n";
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

std::vector<TerritorialUnit*> searchTerritorialUnits(const std::string& name, TerritorialUnitType type) {
    std::vector<TerritorialUnit*> result;

    auto nameIt = territorialUnitsByName.find(name);
    if (nameIt != territorialUnitsByName.end()) {
        result.push_back(nameIt->second);
    }

    auto typeIt = territorialUnitsByType.find(std::to_string(static_cast<int>(type)));
    if (typeIt != territorialUnitsByType.end()) {
        for (const auto& unit : typeIt->second) {
            if (unit->name == name) {
                result.push_back(unit);
            }
        }
    }

    return result;
}