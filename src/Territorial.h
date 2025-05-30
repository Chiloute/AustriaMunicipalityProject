#ifndef TERRITORIAL_H
#define TERRITORIAL_H
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <unordered_map>


enum class TerritorialUnitType {
    MUNICIPALITY,
    REGION,
    FEDERAL_STATE,
    GEOGRAPHICAL_DIVISION
};

struct PopulationData {
    int male;
    int female;

    int total() const {
        return male + female;
    }
};


struct TerritorialUnit {
    
        std::string name;
        std::string code;
        TerritorialUnitType type_;
        std::vector<std::unique_ptr<TerritorialUnit>> children;
        TerritorialUnit* parent = nullptr;

        TerritorialUnit(const std::string& name_, const std::string& code_)
            : name(name_), code(code_) {}

        void addType(TerritorialUnitType type) {
            this->type_ = type;
        }

        void addChild(std::unique_ptr<TerritorialUnit> child) {
            child->parent = this;
            children.push_back(std::move(child));
        }

        virtual ~TerritorialUnit() = default;
};

class TerritorialIterator {
public:
    explicit TerritorialIterator(TerritorialUnit* root)
        : currentUnit(root) {}

    bool moveToChild(int index) {
        if (currentUnit && index >= 0 && index < currentUnit->children.size()) {
            currentUnit = currentUnit->children[index].get();
            return true;
        }
        return false;
    }

    bool moveToParent() {
        auto parent = currentUnit->parent;
        if (parent) {
            currentUnit = parent;
            return true;
        }
        return false;
    }
    TerritorialUnit* getCurrentUnit() const {
        return currentUnit;
    }

    void printCurrentUnit() const {
        if (currentUnit) {
            std::cout << "Current Unit: " << currentUnit->name << " (" << currentUnit->code << ")\n";
        }
    }

private:
    TerritorialUnit* currentUnit;
};


struct Municipality : public TerritorialUnit {
    std::array<PopulationData, 5> yearlyPopulation;

    Municipality(const std::string& name_, const std::string& code_)
        : TerritorialUnit(name_, code_) {}

    void setPopulation(int yearIndex, int male, int female) {
        if (yearIndex >= 0 && yearIndex < 5)
            yearlyPopulation[yearIndex] = {male, female};
    }

    PopulationData getPopulation(int yearIndex) const {
        return yearlyPopulation[yearIndex];
    }

    int getTotalPopulation(int yearIndex) const {
        return yearlyPopulation[yearIndex].total();
    }
};

inline std::unordered_map<std::string, std::vector<TerritorialUnit*>> territorialUnitsByType;
inline std::unordered_map<std::string, TerritorialUnit*> territorialUnitsByName;

#endif //TERRITORIAL_H
