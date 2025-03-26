#pragma once

#include <string>
#include <vector>

class Food {
protected:
    std::string identifier;
    std::vector<std::string> keywords;
    double caloriesPerServing;

public:
    Food(const std::string& id, const std::vector<std::string>& keys, double calories);
    virtual ~Food() = default;

    // Getters
    std::string getIdentifier() const;
    std::vector<std::string> getKeywords() const;
    double getCaloriesPerServing() const;

    // Virtual methods
    virtual double calculateCalories(int servings) const = 0;
    virtual bool isComposite() const = 0;
    virtual std::string toString() const = 0;

    // Debug print
    #ifdef DEBUG
    virtual void debugPrint() const;
    #endif
}; 