#include "food/food.h"
#include <iostream>

Food::Food(const std::string& id, const std::vector<std::string>& keys, double calories)
    : identifier(id), keywords(keys), caloriesPerServing(calories) {
    #ifdef DEBUG
    std::cout << "DEBUG: Created Food object with ID: " << id << std::endl;
    #endif
}

std::string Food::getIdentifier() const {
    return identifier;
}

std::vector<std::string> Food::getKeywords() const {
    return keywords;
}

double Food::getCaloriesPerServing() const {
    return caloriesPerServing;
}

#ifdef DEBUG
void Food::debugPrint() const {
    std::cout << "DEBUG: Food Object:" << std::endl;
    std::cout << "  ID: " << identifier << std::endl;
    std::cout << "  Keywords: ";
    for (const auto& keyword : keywords) {
        std::cout << keyword << " ";
    }
    std::cout << std::endl;
    std::cout << "  Calories per serving: " << caloriesPerServing << std::endl;
}
#endif 