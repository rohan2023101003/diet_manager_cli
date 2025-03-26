#include "database/database.h"
#include "utils/utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Database::Database(const std::string& basicFoodsFile, const std::string& compositeFoodsFile)
    : basicFoodsFile(basicFoodsFile), compositeFoodsFile(compositeFoodsFile) {
    loadBasicFoods();
    loadCompositeFoods();
    #ifdef DEBUG
    std::cout << "DEBUG: Created Database object" << std::endl;
    #endif
}

void Database::loadBasicFoods() {
    std::ifstream file(basicFoodsFile);
    if (!file.is_open()) {
        #ifdef DEBUG
        std::cout << "DEBUG: Could not open basic foods file: " << basicFoodsFile << std::endl;
        #endif
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string id, keywordStr;
        double calories;
        std::vector<std::string> keywords;

        // Read ID and calories
        std::getline(ss, id, '|');
        ss >> calories;
        ss.ignore(); // Skip the separator

        // Read keywords
        std::getline(ss, keywordStr);
        keywords = utils::splitString(keywordStr, ',');

        #ifdef DEBUG
        std::cout << "DEBUG: Loading basic food - ID: " << id 
                  << ", Calories: " << calories 
                  << ", Keywords: ";
        for (const auto& kw : keywords) {
            std::cout << kw << " ";
        }
        std::cout << std::endl;
        #endif

        basicFoods[id] = std::make_shared<BasicFood>(id, keywords, calories);
    }
    #ifdef DEBUG
    std::cout << "DEBUG: Loaded " << basicFoods.size() << " basic foods" << std::endl;
    #endif
}

void Database::loadCompositeFoods() {
    std::ifstream file(compositeFoodsFile);
    if (!file.is_open()) {
        #ifdef DEBUG
        std::cout << "DEBUG: Could not open composite foods file: " << compositeFoodsFile << std::endl;
        #endif
        return;
    }

    std::string line;
    std::shared_ptr<CompositeFood> currentComposite = nullptr;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        if (line == "---") {
            if (currentComposite) {
                compositeFoods[currentComposite->getIdentifier()] = currentComposite;
                currentComposite = nullptr;
            }
            continue;
        }

        std::stringstream ss(line);
        std::string id, keywordStr;
        std::vector<std::string> keywords;

        // Read ID and keywords
        std::getline(ss, id, '|');
        std::getline(ss, keywordStr);
        keywords = utils::splitString(keywordStr, ',');

        if (!currentComposite) {
            currentComposite = std::make_shared<CompositeFood>(id, keywords);
        } else {
            // This is a component line
            std::string componentId;
            int servings;
            std::stringstream componentSs(line);
            std::getline(componentSs, componentId, '|');
            componentSs >> servings;
            
            auto food = getFood(componentId);
            if (food) {
                currentComposite->addComponent(food, servings);
            }
        }
    }

    // Add the last composite food if exists
    if (currentComposite) {
        compositeFoods[currentComposite->getIdentifier()] = currentComposite;
    }

    #ifdef DEBUG
    std::cout << "DEBUG: Loaded " << compositeFoods.size() << " composite foods" << std::endl;
    #endif
}

void Database::saveBasicFoods() const {
    std::ofstream file(basicFoodsFile);
    if (!file.is_open()) {
        #ifdef DEBUG
        std::cout << "DEBUG: Could not open basic foods file for writing: " << basicFoodsFile << std::endl;
        #endif
        return;
    }

    for (const auto& pair : basicFoods) {
        const auto& food = pair.second;
        file << food->getIdentifier() << "|" << food->getCaloriesPerServing() << "|";
        const auto& keywords = food->getKeywords();
        for (size_t i = 0; i < keywords.size(); ++i) {
            file << keywords[i];
            if (i < keywords.size() - 1) file << ",";
        }
        file << "\n";
    }
    #ifdef DEBUG
    std::cout << "DEBUG: Saved " << basicFoods.size() << " basic foods" << std::endl;
    #endif
}

void Database::saveCompositeFoods() const {
    std::ofstream file(compositeFoodsFile);
    if (!file.is_open()) {
        #ifdef DEBUG
        std::cout << "DEBUG: Could not open composite foods file for writing: " << compositeFoodsFile << std::endl;
        #endif
        return;
    }

    for (const auto& pair : compositeFoods) {
        const auto& food = pair.second;
        file << food->getIdentifier() << "|";
        const auto& keywords = food->getKeywords();
        for (size_t i = 0; i < keywords.size(); ++i) {
            file << keywords[i];
            if (i < keywords.size() - 1) file << ",";
        }
        file << "\n";

        // Write components
        const auto& components = food->getComponents();
        for (const auto& component : components) {
            file << component.first << "|" << component.second.second << "\n";
        }
        file << "---\n";
    }
    #ifdef DEBUG
    std::cout << "DEBUG: Saved " << compositeFoods.size() << " composite foods" << std::endl;
    #endif
}

void Database::addBasicFood(const std::string& id, const std::vector<std::string>& keywords, double calories) {
    basicFoods[id] = std::make_shared<BasicFood>(id, keywords, calories);
    #ifdef DEBUG
    std::cout << "DEBUG: Added basic food: " << id << std::endl;
    #endif
}

void Database::addCompositeFood(const std::string& id, const std::vector<std::string>& keywords) {
    compositeFoods[id] = std::make_shared<CompositeFood>(id, keywords);
    #ifdef DEBUG
    std::cout << "DEBUG: Added composite food: " << id << std::endl;
    #endif
}

std::shared_ptr<BasicFood> Database::getBasicFood(const std::string& id) const {
    auto it = basicFoods.find(id);
    return it != basicFoods.end() ? it->second : nullptr;
}

std::shared_ptr<CompositeFood> Database::getCompositeFood(const std::string& id) const {
    auto it = compositeFoods.find(id);
    return it != compositeFoods.end() ? it->second : nullptr;
}

std::shared_ptr<Food> Database::getFood(const std::string& id) const {
    auto basicFood = getBasicFood(id);
    if (basicFood) return basicFood;
    return getCompositeFood(id);
}

std::vector<std::shared_ptr<BasicFood>> Database::searchBasicFoods(
    const std::vector<std::string>& keywords, bool matchAll) const {
    std::vector<std::shared_ptr<BasicFood>> results;
    
    for (const auto& pair : basicFoods) {
        const auto& food = pair.second;
        const auto& foodKeywords = food->getKeywords();
        
        if (matchAll) {
            bool matches = true;
            for (const auto& keyword : keywords) {
                if (std::find(foodKeywords.begin(), foodKeywords.end(), keyword) == foodKeywords.end()) {
                    matches = false;
                    break;
                }
            }
            if (matches) results.push_back(food);
        } else {
            for (const auto& keyword : keywords) {
                if (std::find(foodKeywords.begin(), foodKeywords.end(), keyword) != foodKeywords.end()) {
                    results.push_back(food);
                    break;
                }
            }
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<CompositeFood>> Database::searchCompositeFoods(
    const std::vector<std::string>& keywords, bool matchAll) const {
    std::vector<std::shared_ptr<CompositeFood>> results;
    
    for (const auto& pair : compositeFoods) {
        const auto& food = pair.second;
        const auto& foodKeywords = food->getKeywords();
        
        if (matchAll) {
            bool matches = true;
            for (const auto& keyword : keywords) {
                if (std::find(foodKeywords.begin(), foodKeywords.end(), keyword) == foodKeywords.end()) {
                    matches = false;
                    break;
                }
            }
            if (matches) results.push_back(food);
        } else {
            for (const auto& keyword : keywords) {
                if (std::find(foodKeywords.begin(), foodKeywords.end(), keyword) != foodKeywords.end()) {
                    results.push_back(food);
                    break;
                }
            }
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<Food>> Database::searchAllFoods(
    const std::vector<std::string>& keywords, bool matchAll) const {
    #ifdef DEBUG
    std::cout << "DEBUG: Searching for keywords: ";
    for (const auto& kw : keywords) {
        std::cout << kw << " ";
    }
    std::cout << "matchAll=" << matchAll << std::endl;
    std::cout << "DEBUG: Total basic foods: " << basicFoods.size() << std::endl;
    std::cout << "DEBUG: Total composite foods: " << compositeFoods.size() << std::endl;
    #endif

    std::vector<std::shared_ptr<Food>> results;
    
    // Search basic foods
    for (const auto& [id, food] : basicFoods) {
        bool matches = matchAll;
        for (const auto& keyword : keywords) {
            bool found = false;
            for (const auto& foodKeyword : food->getKeywords()) {
                #ifdef DEBUG
                std::cout << "DEBUG: Comparing search keyword '" << keyword 
                          << "' with food keyword '" << foodKeyword << "'" << std::endl;
                #endif
                if (utils::toLower(foodKeyword) == utils::toLower(keyword)) {
                    found = true;
                    break;
                }
            }
            if (matchAll && !found) {
                matches = false;
                break;
            } else if (!matchAll && found) {
                matches = true;
                break;
            }
        }
        if (matches) {
            results.push_back(food);
        }
    }

    // Search composite foods
    for (const auto& [id, food] : compositeFoods) {
        bool matches = matchAll;
        for (const auto& keyword : keywords) {
            bool found = false;
            for (const auto& foodKeyword : food->getKeywords()) {
                #ifdef DEBUG
                std::cout << "DEBUG: Comparing search keyword '" << keyword 
                          << "' with food keyword '" << foodKeyword << "'" << std::endl;
                #endif
                if (utils::toLower(foodKeyword) == utils::toLower(keyword)) {
                    found = true;
                    break;
                }
            }
            if (matchAll && !found) {
                matches = false;
                break;
            } else if (!matchAll && found) {
                matches = true;
                break;
            }
        }
        if (matches) {
            results.push_back(food);
        }
    }

    #ifdef DEBUG
    std::cout << "DEBUG: Found " << results.size() << " matching foods" << std::endl;
    #endif

    return results;
}

void Database::save() const {
    saveBasicFoods();
    saveCompositeFoods();
    #ifdef DEBUG
    std::cout << "DEBUG: Saved all foods to database files" << std::endl;
    #endif
}

std::map<std::string, std::shared_ptr<Food>> Database::getAllFoods() const {
    std::map<std::string, std::shared_ptr<Food>> allFoods;
    
    // Add basic foods
    for (const auto& [id, food] : basicFoods) {
        allFoods[id] = food;
    }
    
    // Add composite foods
    for (const auto& [id, food] : compositeFoods) {
        allFoods[id] = food;
    }
    
    return allFoods;
}

#ifdef DEBUG
void Database::debugPrint() const {
    std::cout << "DEBUG: Database Contents:" << std::endl;
    std::cout << "Basic Foods:" << std::endl;
    for (const auto& pair : basicFoods) {
        pair.second->debugPrint();
    }
    std::cout << "\nComposite Foods:" << std::endl;
    for (const auto& pair : compositeFoods) {
        pair.second->debugPrint();
    }
}
#endif 