#pragma once

#include "food/food.h"
#include <map>
#include <memory>

class CompositeFood : public Food {
private:
    std::map<std::string, std::pair<std::shared_ptr<Food>, int>> components;

public:
    CompositeFood(const std::string& id, const std::vector<std::string>& keys);

    // Component management
    void addComponent(std::shared_ptr<Food> food, int servings);
    void removeComponent(const std::string& foodId);
    const std::map<std::string, std::pair<std::shared_ptr<Food>, int>>& getComponents() const;

    // Implementation of virtual methods
    double calculateCalories(int servings) const override;
    bool isComposite() const override;
    std::string toString() const override;

    #ifdef DEBUG
    void debugPrint() const override;
    #endif
}; 