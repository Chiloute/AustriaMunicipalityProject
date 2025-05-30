#include "DataLoader.h"
#include <vector>
#include <string>
#include "Territorial.h"
#include "filters.h"
#include <Windows.h>
#include "heap_monitor.h"
#include "sort.h"


int getValidYear() {
    int year;
    std::cout << "Enter a year between 2020 and 2024: ";

    while (!(std::cin >> year) || year < 2020 || year > 2024) {
        std::cin.clear(); 
        std::cin.ignore(10000, '\n'); 
        std::cout << "Invalid input. Please enter a valid year (2020-2024): ";
    }
    return year;
}

void printParentAndChildren(const TerritorialIterator& iterator) {
    auto currentUnit = iterator.getCurrentUnit();

    if (currentUnit) {
        std::cout << "\n Actual unit : " << currentUnit->name << " (" << currentUnit->code << ")\n";
    }

    if (!currentUnit->children.empty()) {
        std::cout << "Actual childs :\n";
        for (const auto& child : currentUnit->children) {
            std::cout << "- " << child->name << " (" << child->code << ")\n";
        }
    } else {
        std::cout << "This unit as no child.\n";
    }
}

void navigateHierarchy(TerritorialIterator& iterator) {
    std::string command;
    while (true) {
        printParentAndChildren(iterator);

        std::cout << "\nEnter a command (ex: AT1 for a child, 'back' to go back,'filter', 'typeOfUnit', 'sort' or  exit' to quit) : ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        if (command == "sort") {
            std::cout << "Sort by (1: Alphabetical, 2: Population): ";
            std::string sortType;
            std::getline(std::cin, sortType);

            auto units = Sort::collect(iterator.getCurrentUnit(), [](const TerritorialUnit*) { return true; });

            if (sortType == "1") {
                auto compareAlphabetical = [](const TerritorialUnit* a, const TerritorialUnit* b) {
                    return a->name < b->name;
                    };
                Sort::sort(units, compareAlphabetical);
                Sort::print(units);
            }
            else if (sortType == "2") {
                int year = getValidYear();
                std::cout << "Gender (0: Male, 1: Female, 2: Total): ";
                int genderInt;
                std::cin >> genderInt;
                std::cin.ignore(10000, '\n');
                Gender gender = static_cast<Gender>(genderInt);

                auto comparePopulation = [year, gender](const TerritorialUnit* a, const TerritorialUnit* b) {
                    auto getPop = [year, gender](const TerritorialUnit* unit) -> int {
                        auto muni = dynamic_cast<const Municipality*>(unit);
                        if (!muni) return 0;
                        PopulationData pop = muni->getPopulation(year - 2020);
                        if (gender == Gender::MALE) return pop.male;
                        if (gender == Gender::FEMALE) return pop.female;
                        return pop.total();
                        };
                    return getPop(a) < getPop(b);
                    };
                Sort::sort(units, comparePopulation);
                Sort::print(units, year - 2020, gender);
            }
            continue;
        }


        // LVL 3 :
        if (command == "typeOfUnit") {
            std::cout << "Enter name: ";
            std::string name;
            std::getline(std::cin, name);

            std::cout << "Enter type (0: MUNICIPALITY, 1: REGION, 2: FEDERAL_STATE, 3: GEOGRAPHICAL_DIVISION): ";
            int type;
            while (!(std::cin >> type) || type < 0 || type > 3) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid value. Enter a valid type (0-3): ";
            }
            std::cin.ignore(10000, '\n');

            auto results = searchTerritorialUnits(name, static_cast<TerritorialUnitType>(type));
            for (const auto& result : results) {
                std::cout << "Found: " << result->name << " (" << result->code  << ")" << std::endl;
            }
            continue;
        }
        if (command == "filter") {
            filterAndPrintMunicipalities(iterator.getCurrentUnit());
            continue;
        }


        if (command == "back") {
            if (!iterator.moveToParent()) {
                std::cout << "You are in root.\n";
            }
        } else {
            try {
                std::string childCode = command;
                auto& children = iterator.getCurrentUnit()->children;

                auto it = std::find_if(children.begin(), children.end(),
                    [&childCode](const std::unique_ptr<TerritorialUnit>& child) {
                        return child->code == childCode;
                    });

                if (it != children.end()) {
                    iterator.moveToChild(std::distance(children.begin(), it));
                } else {
                    std::cout << "No child with this code: " << command << "\n";
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid command, please retry\n";
            }
        }
    }
}


void setupUtf8() {
    // Set the console to use UTF-8 code page (65001) for input and output unicode characters are used
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}



int main() {
    initHeapMonitor();
    setupUtf8();

    auto root = loadTerritorialHierarchy("../data/country.csv","../data/municipalities.csv");

    std::vector<std::string> files = {
        "../data/2020.csv", "../data/2021.csv", "../data/2022.csv", "../data/2023.csv", "../data/2024.csv"
    };

    for (int i = 0; i < files.size(); i++) {
        loadPopulationData(root.get(), files[i], i);
    }

    TerritorialIterator iterator(root.get());

    navigateHierarchy(iterator);

    return 0;
}