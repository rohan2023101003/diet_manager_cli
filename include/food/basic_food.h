#pragma once

#include "food/food.h"

class BasicFood : public Food {
public:
    BasicFood(const std::string& id, const std::vector<std::string>& keys, double calories);

    // Implementation of virtual methods
    double calculateCalories(int servings) const override;
    bool isComposite() const override;
    std::string toString() const override;

    #ifdef DEBUG
    void debugPrint() const override;
    #endif
}; 