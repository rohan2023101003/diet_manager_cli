#include "food/composite_food.h"
#include <sstream>
#include <iostream>

CompositeFood::CompositeFood(const std::string& id, const std::vector<std::string>& keys)
    : Food(id, keys, 0.0) {
    #ifdef DEBUG
    std::cout << "DEBUG: Created CompositeFood object with ID: " << id << std::endl;
    #endif
}

void CompositeFood::addComponent(std::shared_ptr<Food> food, int servings) {
    components[food->getIdentifier()] = {food, servings};
    // Recalculate calories per serving
    caloriesPerServing = calculateCalories(1);
    #ifdef DEBUG
    std::cout << "DEBUG: Added component " << food->getIdentifier() 
              << " with " << servings << " servings to composite food " 
              << identifier << std::endl;
    #endif
}

void CompositeFood::removeComponent(const std::string& foodId) {
    components.erase(foodId);
    // Recalculate calories per serving
    caloriesPerServing = calculateCalories(1);
    #ifdef DEBUG
    std::cout << "DEBUG: Removed component " << foodId 
              << " from composite food " << identifier << std::endl;
    #endif
}

const std::map<std::string, std::pair<std::shared_ptr<Food>, int>>& 
CompositeFood::getComponents() const {
    return components;
}

double CompositeFood::calculateCalories(int servings) const {
    double totalCalories = 0.0;
    for (const auto& component : components) {
        totalCalories += component.second.first->calculateCalories(component.second.second);
    }
    return totalCalories * servings;
}

bool CompositeFood::isComposite() const {
    return true;
}

std::string CompositeFood::toString() const {
    std::stringstream ss;
    ss << "Composite Food: " << identifier << "\n";
    ss << "Keywords: ";
    for (const auto& keyword : keywords) {
        ss << keyword << " ";
    }
    ss << "\nComponents:\n";
    for (const auto& component : components) {
        ss << "  - " << component.first << " (" 
           << component.second.second << " servings)\n";
    }
    ss << "Total calories per serving: " << caloriesPerServing;
    return ss.str();
}

#ifdef DEBUG
void CompositeFood::debugPrint() const {
    std::cout << "DEBUG: CompositeFood Object:" << std::endl;
    Food::debugPrint();
    std::cout << "  Components:" << std::endl;
    for (const auto& component : components) {
        std::cout << "    - " << component.first 
                  << " (" << component.second.second << " servings)" << std::endl;
    }
}
#endif 