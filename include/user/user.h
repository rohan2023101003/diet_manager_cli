#pragma once

#include <string>
#include <ctime>

enum class Gender {
    MALE,
    FEMALE,
    OTHER
};

enum class ActivityLevel {
    SEDENTARY,
    LIGHTLY_ACTIVE,
    MODERATELY_ACTIVE,
    VERY_ACTIVE,
    EXTRA_ACTIVE
};

class User {
private:
    std::string username;
    std::string passwordHash;
    Gender gender;
    double height;  // in centimeters
    int age;
    double weight;  // in kilograms
    ActivityLevel activityLevel;
    std::string calorieCalculationMethod;

public:
    User(const std::string& username, const std::string& passwordHash,
         Gender gender, double height, int age, double weight,
         ActivityLevel activityLevel);

    // Getters
    std::string getUsername() const;
    std::string getPasswordHash() const;
    Gender getGender() const;
    double getHeight() const;
    int getAge() const;
    double getWeight() const;
    ActivityLevel getActivityLevel() const;
    std::string getCalorieCalculationMethod() const;

    // Setters
    void setHeight(double height);
    void setAge(int age);
    void setWeight(double weight);
    void setActivityLevel(ActivityLevel level);
    void setCalorieCalculationMethod(const std::string& method);

    // Calorie calculation methods
    double calculateBMR() const;
    double calculateTDEE() const;
    double calculateTargetCalories() const;

    // Profile management
    std::string toString() const;
    void updateProfile(double height, int age, double weight, ActivityLevel level);

    #ifdef DEBUG
    void debugPrint() const;
    #endif
}; 