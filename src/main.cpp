#include "DataLoader.h"
#include "Municipality.h"
#include "MunicipalityProcessor.h"
#include "Predicates.h"
#include <Windows.h>
#include <locale>

void setupUtf8() {
    // Set the console to use UTF-8 code page (65001) for input and output
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // Set the global locale to a German locale with UTF-8 support
    try {
        std::locale::global(std::locale("de_DE.UTF-8"));
        std::cin.imbue(std::locale());
        std::cout.imbue(std::locale());
    } catch (const std::exception &e) {
        std::ofstream logFile("../data/error_log.txt", std::ios_base::app);
        logFile << "Error setting UTF-8 locale: " << e.what() << std::endl;
    }
}

int getValidYear() {
    int year;
    std::cout << "Enter a year between 2020 and 2024: ";

    while (!(std::cin >> year) || year < 2020 || year > 2024) {
        std::cin.clear(); // Réinitialise cin
        std::cin.ignore(10000, '\n'); // Vide le buffer
        std::cout << "Invalid input. Please enter a valid year (2020-2024): ";
    }

    return year;
}

int main() {
    setupUtf8();

    std::vector<std::string> files = {
        "../data/2020.csv", "../data/2021.csv", "../data/2022.csv", "../data/2023.csv", "../data/2024.csv"
    };
    std::vector<Municipality> allMunicipalities;

    for (const auto &file: files) {
        std::vector<Municipality> municipalities = parseCSV(file);
        for (const auto &municipality: municipalities) {
            // Find existing municipality or create a new entry
            auto it = std::find_if(allMunicipalities.begin(), allMunicipalities.end(),
                                   [&](const Municipality &m) { return m.cityCode == municipality.cityCode; });
            if (it != allMunicipalities.end()) {
                it->malePopulation.push_back(municipality.malePopulation[0]);
                it->femalePopulation.push_back(municipality.femalePopulation[0]);
            } else {
                allMunicipalities.push_back(municipality);
            }
        }
    }
    std::vector<Municipality> filteredMunicipalities = allMunicipalities;
    int choice;
    std::string name;
    int maxResidents, minResidents;
    std::function<bool(const Municipality &)> predicate;

    while (true) {
        filteredMunicipalities = allMunicipalities;
        std::cout << "Municipality filter\n";
        std::cout << "Number of municipalities: " << filteredMunicipalities.size() << std::endl;
        std::cout << "Choose a predicate:\n";
        std::cout << "1. Name\n";
        std::cout << "2. Max residents\n";
        std::cout << "3. Min residents\n";
        std::cout << "4. Name + Max residents\n";
        std::cout << "5. Name + Min residents\n";
        std::cout << "6. Max residents + Min residents\n";
        std::cout << "7. Name + Max residents + Min residents\n";
        std::cout << "8. Quit\n";
        std::cin >> choice;
        int year;
        if (choice == 8) break;
        if (choice != 1)year = getValidYear();


        switch (choice) {
            case 1:
                std::cout << "Enter the name: ";
                std::cin >> name;
                predicate = [name](const Municipality &m) { return containsStr(name)(m); };
                break;
            case 2:
                std::cout << "Enter the max residents: ";
                std::cin >> maxResidents;
                predicate = [year, maxResidents](const Municipality &m) {
                    return hasMaxResidents(year, maxResidents)(m);
                };
                break;
            case 3:
                std::cout << "Enter the min residents: ";
                std::cin >> minResidents;
                predicate = [year, minResidents](const Municipality &m) {
                    return hasMinResidents(year, minResidents)(m);
                };
                break;
            case 4:
                std::cout << "Enter the name: ";
                std::cin >> name;
                std::cout << "Enter the max residents: ";
                std::cin >> maxResidents;
                predicate = [name, year, maxResidents](const Municipality &m) {
                    return containsStr(name)(m) && hasMaxResidents(year, maxResidents)(m);
                };
                break;
            case 5:
                std::cout << "Enter the name: ";
                std::cin >> name;
                std::cout << "Enter the min residents: ";
                std::cin >> minResidents;
                predicate = [name, year, minResidents](const Municipality &m) {
                    return containsStr(name)(m) && hasMinResidents(year, minResidents)(m);
                };
                break;
            case 6:
                std::cout << "Enter the max residents: ";
                std::cin >> maxResidents;
                std::cout << "Enter the min residents: ";
                std::cin >> minResidents;
                predicate = [year, maxResidents, minResidents](const Municipality &m) {
                    return hasMaxResidents(year, maxResidents)(m) && hasMinResidents(year, minResidents)(m);
                };
                break;
            case 7:
                std::cout << "Enter the name: ";
                std::cin >> name;
                std::cout << "Enter the max residents: ";
                std::cin >> maxResidents;
                std::cout << "Enter the min residents: ";
                std::cin >> minResidents;
                predicate = [name,year, maxResidents, minResidents](const Municipality &m) {
                    return containsStr(name)(m) && hasMaxResidents(year, maxResidents)(m) && hasMinResidents(
                               year, minResidents)(m);
                };
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                continue;
        }

        filteredMunicipalities = MunicipalityProcessor::filterMunicipalities(filteredMunicipalities, predicate);

        for (const auto &m: filteredMunicipalities) {
            std::cout << "Municipality: " << m.name_ << " | Code: " << m.cityCode << std::endl;
            for (int i = 0; i < m.malePopulation.size(); i++) {
                std::cout << " | Population " << 2020 + i << ": " << m.malePopulation[i] + m.femalePopulation[i] <<
                        std::endl;
            }
        }
        std::cout << "Result of filter : " << filteredMunicipalities.size() << std::endl;
        std::cout << "Continue ?\n";
        std::cin.ignore(10000, '\n');
        std::cin.get();
    }
    return 0;
}
