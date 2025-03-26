#include "food/basic_food.h"
#include <sstream>
#include <iostream>

BasicFood::BasicFood(const std::string& id, const std::vector<std::string>& keys, double calories)
    : Food(id, keys, calories) {
    #ifdef DEBUG
    std::cout << "DEBUG: Created BasicFood object with ID: " << id << std::endl;
    #endif
}

double BasicFood::calculateCalories(int servings) const {
    return caloriesPerServing * servings;
}

bool BasicFood::isComposite() const {
    return false;
}

std::string BasicFood::toString() const {
    std::stringstream ss;
    ss << "Basic Food: " << identifier << "\n";
    ss << "Keywords: ";
    for (const auto& keyword : keywords) {
        ss << keyword << " ";
    }
    ss << "\nCalories per serving: " << caloriesPerServing;
    return ss.str();
}

#ifdef DEBUG
void BasicFood::debugPrint() const {
    std::cout << "DEBUG: BasicFood Object:" << std::endl;
    Food::debugPrint();
}
#endif 