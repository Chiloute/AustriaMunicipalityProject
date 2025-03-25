#include "DataLoader.h"

std::vector<Municipality> parseCSV(const std::string &filePath) {
    std::vector<Municipality> municipalities;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return municipalities;
    }

    std::string line;
    // Ignore the header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string field;
        std::vector<std::string> tokens;

        // Split the line into tokens
        while (std::getline(ss, field, ';')) {
            tokens.push_back(field);
        }

        if (tokens.size() < 5) continue; // Ensure we have enough data

        /* Debugging: Print tokens
        for (const auto& token : tokens) {
            std::cout << "Token: " << token << std::endl;
        }
*/
        try {
            Municipality municipality;
            municipality.name_ = tokens[0];
            municipality.cityCode = std::stoi(tokens[1].substr(1, tokens[1].size() - 2)); // Remove '<' and '>'

            // Assuming the CSV has columns: Name, Code, MalePopulation, Empty, FemalePopulation, Empty
            municipality.malePopulation.push_back(std::stoi(tokens[2]));
            municipality.femalePopulation.push_back(std::stoi(tokens[4]));

            municipalities.push_back(municipality);
        } catch (const std::invalid_argument &e) {
            std::ofstream logFile("../data/error_log.txt", std::ios_base::app);
            logFile << "Invalid argument error: " << e.what() << std::endl;
        } catch (const std::out_of_range &e) {
            std::ofstream logFile("../data/error_log.txt", std::ios_base::app);
            logFile << "Out of range error: " << e.what() << std::endl;
        }
    }

    file.close();
    return municipalities;
}
