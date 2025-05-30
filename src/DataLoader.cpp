#include "DataLoader.h"

std::unique_ptr<TerritorialUnit> loadTerritorialHierarchy(const std::string& hierarchyPath, const std::string& municipalityPath) {
    std::ifstream file(hierarchyPath);
    if (!file.is_open()) {
        std::cerr << "Cannot open hierarchy file: " << hierarchyPath << std::endl;
        return nullptr;
    }

    std::unordered_map<std::string, TerritorialUnit*> codeToUnit;
    auto root = std::make_unique<TerritorialUnit>("Austria", "AT");
    codeToUnit["AT"] = root.get();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string name, code;

        std::getline(ss, name, ';');
        std::getline(ss, code, ';');
        code = code.substr(1, code.size() - 2);

        auto unit = std::make_unique<TerritorialUnit>(name, code);
        if (code.size() == 3) {
            unit->type_ = TerritorialUnitType::GEOGRAPHICAL_DIVISION;
        }else if (code.size() == 4) {
            unit->type_ = TerritorialUnitType::FEDERAL_STATE;
        }else if (code.size() == 5) {
            unit->type_ = TerritorialUnitType::REGION;
        }
		TerritorialUnit* unitPtr = unit.get();
        territorialUnitsByType[std::to_string(static_cast<int>(unit->type_))].push_back(unitPtr);
        territorialUnitsByName[name] = unitPtr;
        codeToUnit[code] = unitPtr;

        if (code.size() == 3 && code.substr(0, 2) == "AT") {
            root->addChild(std::move(unit));
        } else {
            std::string parentCode = code.substr(0, code.size() - 1);
            auto parentIt = codeToUnit.find(parentCode);
            if (parentIt != codeToUnit.end()) {
                parentIt->second->addChild(std::move(unit));
            } else {
                std::cerr << "Parent not found for: " << code << " (expected: " << parentCode << ")\n";
            }
        }
    }

    file.close();

    std::ifstream muniFile(municipalityPath);
    if (!muniFile.is_open()) {
        std::cerr << "Cannot open municipality file: " << municipalityPath << std::endl;
        return root;
    }

    while (std::getline(muniFile, line)) {
        std::istringstream ss(line);
        std::string name, rawCode, parentCode;
        std::getline(ss, name, ';');
        std::getline(ss, rawCode, ';');
        std::getline(ss, parentCode, ';');

        if (parentCode.empty()) {
            std::cerr << "Parent code not found for municipality: " << rawCode << "\n";
            continue;
        }

        rawCode = rawCode.substr(1, rawCode.size() - 2); // nettoie le code
        auto municipality = std::make_unique<Municipality>(name, rawCode);
        municipality->type_ = TerritorialUnitType::MUNICIPALITY;
		Municipality* municipalityPtr = municipality.get();
        territorialUnitsByType[std::to_string(static_cast<int>(TerritorialUnitType::MUNICIPALITY))].push_back(municipalityPtr);
        territorialUnitsByName[name] = municipalityPtr;

        auto parentIt = codeToUnit.find(parentCode);
        if (parentIt != codeToUnit.end()) {
            parentIt->second->addChild(std::move(municipality));
        } else {
            std::cerr << "Parent not found for municipality: " << rawCode << " (parent: " << parentCode << ")\n";
        }

        codeToUnit[rawCode] = municipalityPtr;
    }

    muniFile.close();
    return root;
}

void loadPopulationData(TerritorialUnit* root, const std::string& popFilePath, int yearIndex) {
    std::ifstream file(popFilePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open population file: " << popFilePath << std::endl;
        return;
    }

    std::unordered_map<std::string, Municipality*> municipalities;

    std::function<void(TerritorialUnit*)> collectMunicipalities;
    collectMunicipalities = [&](TerritorialUnit* unit) {
        if (auto muni = dynamic_cast<Municipality*>(unit)) {
            municipalities[muni->code] = muni;
        }
        for (const auto& child : unit->children) {
            collectMunicipalities(child.get());
        }
    };
    collectMunicipalities(root);

    std::string line;

    while (std::getline(file, line)) {
        if (line.find('<') != std::string::npos && line.find('>') != std::string::npos) {
            break; 
        }
    }

    do {
        std::istringstream ss(line);
        std::string name, rawCode, maleStr, skip, femaleStr;

        std::getline(ss, name, ';');
        std::getline(ss, rawCode, ';');
        std::getline(ss, maleStr, ';');
        std::getline(ss, skip, ';');       // colonnes vides
        std::getline(ss, femaleStr, ';');

        try {
            std::string code = rawCode.substr(1, rawCode.size() - 2); // Retire les < >
            
            int male = std::stoi(maleStr);
            int female = std::stoi(femaleStr);

            auto it = municipalities.find(code);
            if (it != municipalities.end()) {
                it->second->setPopulation(yearIndex, male, female);
            } else {
                std::ofstream logFile("../data/error_log.txt", std::ios_base::app);
                logFile  << "Municipality not found for population: " << code << "\n";
            }
        } catch (...) {
            std::ofstream logFile("../data/error_log.txt", std::ios_base::app);
            logFile << "Error processing line: " << line << " -> "  << "\n";
        }
    } while (std::getline(file, line));

    file.close();
}