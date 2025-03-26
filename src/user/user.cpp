#include "user/user.h"
#include <sstream>
#include <iostream>
#include <cmath>

User::User(const std::string& username, const std::string& passwordHash,
           Gender gender, double height, int age, double weight,
           ActivityLevel activityLevel)
    : username(username), passwordHash(passwordHash), gender(gender),
      height(height), age(age), weight(weight), activityLevel(activityLevel),
      calorieCalculationMethod("Mifflin-St Jeor") {
    #ifdef DEBUG
    std::cout << "DEBUG: Created User object with username: " << username << std::endl;
    #endif
}

std::string User::getUsername() const { return username; }
std::string User::getPasswordHash() const { return passwordHash; }
Gender User::getGender() const { return gender; }
double User::getHeight() const { return height; }
int User::getAge() const { return age; }
double User::getWeight() const { return weight; }
ActivityLevel User::getActivityLevel() const { return activityLevel; }
std::string User::getCalorieCalculationMethod() const { return calorieCalculationMethod; }

void User::setHeight(double height) { this->height = height; }
void User::setAge(int age) { this->age = age; }
void User::setWeight(double weight) { this->weight = weight; }
void User::setActivityLevel(ActivityLevel level) { activityLevel = level; }
void User::setCalorieCalculationMethod(const std::string& method) { calorieCalculationMethod = method; }

double User::calculateBMR() const {
    // Mifflin-St Jeor Equation
    double bmr;
    if (gender == Gender::MALE) {
        bmr = (10 * weight) + (6.25 * height) - (5 * age) + 5;
    } else {
        bmr = (10 * weight) + (6.25 * height) - (5 * age) - 161;
    }
    return bmr;
}

double User::calculateTDEE() const {
    double bmr = calculateBMR();
    double activityMultiplier;
    
    switch (activityLevel) {
        case ActivityLevel::SEDENTARY:
            activityMultiplier = 1.2;
            break;
        case ActivityLevel::LIGHTLY_ACTIVE:
            activityMultiplier = 1.375;
            break;
        case ActivityLevel::MODERATELY_ACTIVE:
            activityMultiplier = 1.55;
            break;
        case ActivityLevel::VERY_ACTIVE:
            activityMultiplier = 1.725;
            break;
        case ActivityLevel::EXTRA_ACTIVE:
            activityMultiplier = 1.9;
            break;
        default:
            activityMultiplier = 1.2;
    }
    
    return bmr * activityMultiplier;
}

double User::calculateTargetCalories() const {
    // For weight maintenance, use TDEE
    // For weight loss, subtract 500 calories
    // For weight gain, add 500 calories
    return calculateTDEE() - 500; // Default to weight loss
}

void User::updateProfile(double height, int age, double weight, ActivityLevel level) {
    this->height = height;
    this->age = age;
    this->weight = weight;
    this->activityLevel = level;
    #ifdef DEBUG
    std::cout << "DEBUG: Updated profile for user: " << username << std::endl;
    #endif
}

std::string User::toString() const {
    std::stringstream ss;
    ss << "User Profile:\n";
    ss << "Username: " << username << "\n";
    ss << "Gender: " << (gender == Gender::MALE ? "Male" : 
                        gender == Gender::FEMALE ? "Female" : "Other") << "\n";
    ss << "Height: " << height << " cm\n";
    ss << "Age: " << age << " years\n";
    ss << "Weight: " << weight << " kg\n";
    ss << "Activity Level: ";
    switch (activityLevel) {
        case ActivityLevel::SEDENTARY: ss << "Sedentary"; break;
        case ActivityLevel::LIGHTLY_ACTIVE: ss << "Lightly Active"; break;
        case ActivityLevel::MODERATELY_ACTIVE: ss << "Moderately Active"; break;
        case ActivityLevel::VERY_ACTIVE: ss << "Very Active"; break;
        case ActivityLevel::EXTRA_ACTIVE: ss << "Extra Active"; break;
    }
    ss << "\nCalorie Calculation Method: " << calorieCalculationMethod << "\n";
    ss << "BMR: " << calculateBMR() << " calories\n";
    ss << "TDEE: " << calculateTDEE() << " calories\n";
    ss << "Target Calories: " << calculateTargetCalories() << " calories";
    return ss.str();
}

#ifdef DEBUG
void User::debugPrint() const {
    std::cout << "DEBUG: User Object:" << std::endl;
    std::cout << "  Username: " << username << std::endl;
    std::cout << "  Gender: " << (gender == Gender::MALE ? "Male" : 
                                 gender == Gender::FEMALE ? "Female" : "Other") << std::endl;
    std::cout << "  Height: " << height << " cm" << std::endl;
    std::cout << "  Age: " << age << " years" << std::endl;
    std::cout << "  Weight: " << weight << " kg" << std::endl;
    std::cout << "  Activity Level: ";
    switch (activityLevel) {
        case ActivityLevel::SEDENTARY: std::cout << "Sedentary"; break;
        case ActivityLevel::LIGHTLY_ACTIVE: std::cout << "Lightly Active"; break;
        case ActivityLevel::MODERATELY_ACTIVE: std::cout << "Moderately Active"; break;
        case ActivityLevel::VERY_ACTIVE: std::cout << "Very Active"; break;
        case ActivityLevel::EXTRA_ACTIVE: std::cout << "Extra Active"; break;
    }
    std::cout << std::endl;
    std::cout << "  BMR: " << calculateBMR() << " calories" << std::endl;
    std::cout << "  TDEE: " << calculateTDEE() << " calories" << std::endl;
    std::cout << "  Target Calories: " << calculateTargetCalories() << " calories" << std::endl;
}
#endif 