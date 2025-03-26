#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "food/food.h"
#include "food/basic_food.h"
#include "food/composite_food.h"

class Database {
private:
    std::map<std::string, std::shared_ptr<BasicFood>> basicFoods;
    std::map<std::string, std::shared_ptr<CompositeFood>> compositeFoods;
    std::string basicFoodsFile;
    std::string compositeFoodsFile;

    void loadBasicFoods();
    void loadCompositeFoods();
    void saveBasicFoods() const;
    void saveCompositeFoods() const;

public:
    Database(const std::string& basicFoodsFile, const std::string& compositeFoodsFile);

    // Basic food operations
    void addBasicFood(const std::string& id, const std::vector<std::string>& keywords, double calories);
    std::shared_ptr<BasicFood> getBasicFood(const std::string& id) const;
    std::vector<std::shared_ptr<BasicFood>> searchBasicFoods(const std::vector<std::string>& keywords, bool matchAll = true) const;

    // Composite food operations
    void addCompositeFood(const std::string& id, const std::vector<std::string>& keywords);
    std::shared_ptr<CompositeFood> getCompositeFood(const std::string& id) const;
    std::vector<std::shared_ptr<CompositeFood>> searchCompositeFoods(const std::vector<std::string>& keywords, bool matchAll = true) const;

    // General operations
    void save() const;
    std::vector<std::shared_ptr<Food>> searchAllFoods(const std::vector<std::string>& keywords, bool matchAll = true) const;
    std::shared_ptr<Food> getFood(const std::string& id) const;
    std::map<std::string, std::shared_ptr<Food>> getAllFoods() const;

    #ifdef DEBUG
    void debugPrint() const;
    #endif
}; 