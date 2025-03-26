#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <fstream>
#include <sstream>
#include "user/user.h"
#include "database/database.h"
#include "logger/logger.h"
#include "utils/utils.h"

class YADA {
private:
    std::shared_ptr<User> currentUser;
    std::unique_ptr<Database> database;
    std::unique_ptr<Logger> logger;
    std::map<std::string, std::shared_ptr<User>> users;
    std::string currentDate;

    void loadUsers();
    void saveUsers() const;
    bool login(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password,
                     Gender gender, double height, int age, double weight,
                     ActivityLevel activityLevel);
    void showMainMenu();
    void showFoodMenu();
    void showLogMenu();
    void showProfileMenu();
    void addBasicFood();
    void addCompositeFood();
    void searchFoods();
    void addFoodToLog();
    void viewLog();
    void deleteFromLog();
    void updateProfile();
    void showCalorieSummary();
    void showLoginMenu();
    void login();
    void registerUser();

public:
    YADA() : currentDate(utils::getCurrentDate()) {
        #ifdef DEBUG
        utils::printDebug("Initializing YADA");
        #endif

        // Create necessary directories
        utils::createDirectory("data");
        utils::createDirectory("data/daily_logs");

        // Initialize components
        database = std::make_unique<Database>("data/basic_foods.txt", "data/composite_foods.txt");
        logger = std::make_unique<Logger>("data/daily_logs", "");  // Empty username initially

        // Load user data
        loadUsers();
        #ifdef DEBUG
        std::cout << "DEBUG: Created YADA object" << std::endl;
        #endif
    }

    void run();
};

void YADA::loadUsers() {
    std::ifstream file("data/users.txt");
    if (!file.is_open()) {
        #ifdef DEBUG
        utils::printDebug("No users file found, starting fresh");
        #endif
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, passwordHash, genderStr;
        double height;
        int age;
        double weight;
        int activityLevel;

        std::getline(ss, username, '|');
        std::getline(ss, passwordHash, '|');
        std::getline(ss, genderStr, '|');
        ss >> height >> age >> weight >> activityLevel;

        Gender gender = (genderStr == "MALE") ? Gender::MALE :
                       (genderStr == "FEMALE") ? Gender::FEMALE : Gender::OTHER;

        users[username] = std::make_shared<User>(username, passwordHash, gender,
                                                height, age, weight,
                                                static_cast<ActivityLevel>(activityLevel));
    }
    #ifdef DEBUG
    utils::printDebug("Loaded " + std::to_string(users.size()) + " users");
    #endif
}

void YADA::saveUsers() const {
    std::ofstream file("data/users.txt");
    if (!file.is_open()) {
        #ifdef DEBUG
        utils::printDebug("Could not open users file for writing");
        #endif
        return;
    }

    for (const auto& [username, user] : users) {
        file << username << "|" << user->getPasswordHash() << "|"
             << (user->getGender() == Gender::MALE ? "MALE" :
                 user->getGender() == Gender::FEMALE ? "FEMALE" : "OTHER") << "|"
             << user->getHeight() << " " << user->getAge() << " "
             << user->getWeight() << " " << static_cast<int>(user->getActivityLevel()) << "\n";
    }
    #ifdef DEBUG
    utils::printDebug("Saved " + std::to_string(users.size()) + " users");
    #endif
}

bool YADA::login(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it == users.end()) {
        std::cout << "User not found.\n";
        return false;
    }

    if (utils::verifyPassword(password, it->second->getPasswordHash())) {
        currentUser = it->second;
        logger = std::make_unique<Logger>("data/daily_logs", username);
        std::cout << "Login successful!\n";
        return true;
    }

    std::cout << "Invalid password.\n";
    return false;
}

bool YADA::registerUser(const std::string& username, const std::string& password,
                       Gender gender, double height, int age, double weight,
                       ActivityLevel activityLevel) {
    if (users.find(username) != users.end()) {
        std::cout << "Username already exists.\n";
        return false;
    }

    if (!utils::isValidUsername(username)) {
        std::cout << "Invalid username format.\n";
        return false;
    }

    if (!utils::isValidPassword(password)) {
        std::cout << "Invalid password format.\n";
        return false;
    }

    std::string passwordHash = utils::hashPassword(password);
    users[username] = std::make_shared<User>(username, passwordHash, gender,
                                            height, age, weight, activityLevel);
    saveUsers();
    std::cout << "Registration successful!\n";
    return true;
}

void YADA::showMainMenu() {
    while (true) {
        std::cout << "\n=== YADA Main Menu ===\n"
                  << "1. Food Management\n"
                  << "2. Daily Log\n"
                  << "3. Profile Settings\n"
                  << "4. Calorie Summary\n"
                  << "5. Logout\n"
                  << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: showFoodMenu(); break;
            case 2: showLogMenu(); break;
            case 3: showProfileMenu(); break;
            case 4: showCalorieSummary(); break;
            case 5: 
                currentUser = nullptr;
                logger = std::make_unique<Logger>("data/daily_logs", "");
                return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

void YADA::showFoodMenu() {
    while (true) {
        std::cout << "\n=== Food Management ===\n"
                  << "1. Add Basic Food\n"
                  << "2. Add Composite Food\n"
                  << "3. Search Foods\n"
                  << "4. Back to Main Menu\n"
                  << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: addBasicFood(); break;
            case 2: addCompositeFood(); break;
            case 3: searchFoods(); break;
            case 4: return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

void YADA::showLogMenu() {
    while (true) {
        std::cout << "\n=== Daily Log ===\n"
                  << "1. Add Food to Log\n"
                  << "2. View Log\n"
                  << "3. Delete from Log\n"
                  << "4. Undo Last Action\n"
                  << "5. Back to Main Menu\n"
                  << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: addFoodToLog(); break;
            case 2: viewLog(); break;
            case 3: deleteFromLog(); break;
            case 4: 
                if (logger->canUndo()) {
                    logger->undo();
                    std::cout << "Last action undone.\n";
                } else {
                    std::cout << "Nothing to undo.\n";
                }
                break;
            case 5: return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

void YADA::showProfileMenu() {
    while (true) {
        std::cout << "\n=== Profile Settings ===\n"
                  << "1. Update Profile\n"
                  << "2. View Profile\n"
                  << "3. Back to Main Menu\n"
                  << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: updateProfile(); break;
            case 2: std::cout << currentUser->toString() << "\n"; break;
            case 3: return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

void YADA::addBasicFood() {
    std::string id, keyword;
    double calories;
    std::vector<std::string> keywords;

    std::cout << "Enter food identifier: ";
    std::getline(std::cin, id);
    
    std::cout << "Enter calories per serving: ";
    std::cin >> calories;
    std::cin.ignore();

    std::cout << "Enter keywords (comma-separated): ";
    std::getline(std::cin, keyword);
    keywords = utils::splitString(keyword, ',');

    database->addBasicFood(id, keywords, calories);
    database->save();
    std::cout << "Basic food added successfully!\n";
}

void YADA::addCompositeFood() {
    std::string id, keyword;
    std::vector<std::string> keywords;

    std::cout << "Enter composite food identifier: ";
    std::getline(std::cin, id);
    
    std::cout << "Enter keywords (comma-separated): ";
    std::getline(std::cin, keyword);
    keywords = utils::splitString(keyword, ',');

    database->addCompositeFood(id, keywords);

    while (true) {
        std::cout << "Add component (y/n)? ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice != 'y' && choice != 'Y') break;

        std::string componentId;
        int servings;

        std::cout << "Enter component food ID: ";
        std::getline(std::cin, componentId);
        
        std::cout << "Enter number of servings: ";
        std::cin >> servings;
        std::cin.ignore();

        auto food = database->getFood(componentId);
        if (food) {
            auto compositeFood = database->getCompositeFood(id);
            if (compositeFood) {
                compositeFood->addComponent(food, servings);
                std::cout << "Component added successfully!\n";
            }
        } else {
            std::cout << "Food not found.\n";
        }
    }

    database->save();
    std::cout << "Composite food added successfully!\n";
}

void YADA::searchFoods() {
    std::string keyword;
    std::cout << "Enter search keywords (comma-separated): ";
    std::getline(std::cin, keyword);
    auto keywords = utils::splitString(keyword, ',');

    std::cout << "Match all keywords (y/n)? ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();

    bool matchAll = (choice == 'y' || choice == 'Y');
    auto results = database->searchAllFoods(keywords, matchAll);

    if (results.empty()) {
        std::cout << "No foods found matching your search criteria.\n";
        return;
    }

    std::cout << "\nSearch Results:\n";
    for (const auto& food : results) {
        std::cout << "----------------------------------------\n";
        std::cout << "ID: " << food->getIdentifier() << "\n";
        std::cout << "Type: " << (dynamic_cast<CompositeFood*>(food.get()) ? "Composite Food" : "Basic Food") << "\n";
        std::cout << "Keywords: ";
        for (const auto& kw : food->getKeywords()) {
            std::cout << kw << ", ";
        }
        std::cout << "\n";
        std::cout << "Calories per serving: " << food->calculateCalories(1) << "\n";
        
        if (auto composite = dynamic_cast<CompositeFood*>(food.get())) {
            std::cout << "Components:\n";
            for (const auto& [componentId, componentPair] : composite->getComponents()) {
                const auto& [component, servings] = componentPair;
                std::cout << "  - " << componentId << " (" << servings << " servings)\n";
            }
        }
        std::cout << "----------------------------------------\n\n";
    }
}

void YADA::addFoodToLog() {
    std::string foodId;
    int servings;

    std::cout << "Enter food ID: ";
    std::getline(std::cin, foodId);
    
    std::cout << "Enter number of servings: ";
    std::cin >> servings;
    std::cin.ignore();

    auto food = database->getFood(foodId);
    if (food) {
        logger->addEntry(currentDate, foodId, servings);
        std::cout << "Food added to log successfully!\n";
    } else {
        std::cout << "Food not found.\n";
    }
}

void YADA::viewLog() {
    std::string date;
    std::cout << "Enter date (YYYY-MM-DD) or press Enter for today: ";
    std::getline(std::cin, date);
    
    if (date.empty()) {
        date = currentDate;
    }

    if (!utils::isValidDate(date)) {
        std::cout << "Invalid date format.\n";
        return;
    }

    auto entries = logger->getLog(date);
    if (entries.empty()) {
        std::cout << "No entries found for " << date << "\n";
        return;
    }

    std::cout << "\nLog for " << date << ":\n";
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        auto food = database->getFood(entry.foodId);
        if (food) {
            std::cout << i + 1 << ". " << food->getIdentifier() 
                      << " (" << entry.servings << " servings) - "
                      << food->calculateCalories(entry.servings) << " calories\n";
        }
    }
}

void YADA::deleteFromLog() {
    std::string date;
    std::cout << "Enter date (YYYY-MM-DD) or press Enter for today: ";
    std::getline(std::cin, date);
    
    if (date.empty()) {
        date = currentDate;
    }

    if (!utils::isValidDate(date)) {
        std::cout << "Invalid date format.\n";
        return;
    }

    auto entries = logger->getLog(date);
    if (entries.empty()) {
        std::cout << "No entries found for " << date << "\n";
        return;
    }

    std::cout << "\nLog for " << date << ":\n";
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        auto food = database->getFood(entry.foodId);
        if (food) {
            std::cout << i + 1 << ". " << food->getIdentifier() 
                      << " (" << entry.servings << " servings)\n";
        }
    }

    std::cout << "Enter entry number to delete: ";
    size_t index;
    std::cin >> index;
    std::cin.ignore();

    if (index > 0 && index <= entries.size()) {
        logger->removeEntry(date, index - 1);
        std::cout << "Entry deleted successfully!\n";
    } else {
        std::cout << "Invalid entry number.\n";
    }
}

void YADA::updateProfile() {
    double height;
    int age;
    double weight;
    int activityLevel;

    std::cout << "Enter height (cm): ";
    std::cin >> height;
    
    std::cout << "Enter age: ";
    std::cin >> age;
    
    std::cout << "Enter weight (kg): ";
    std::cin >> weight;
    
    std::cout << "Enter activity level (1-5):\n"
              << "1. Sedentary\n"
              << "2. Lightly Active\n"
              << "3. Moderately Active\n"
              << "4. Very Active\n"
              << "5. Extra Active\n"
              << "Choice: ";
    std::cin >> activityLevel;
    std::cin.ignore();

    if (activityLevel >= 1 && activityLevel <= 5) {
        currentUser->updateProfile(height, age, weight, static_cast<ActivityLevel>(activityLevel - 1));
        users[currentUser->getUsername()] = currentUser;
        saveUsers();
        std::cout << "Profile updated successfully!\n";
    } else {
        std::cout << "Invalid activity level.\n";
    }
}

void YADA::showCalorieSummary() {
    std::string date;
    std::cout << "Enter date (YYYY-MM-DD) or press Enter for today: ";
    std::getline(std::cin, date);
    
    if (date.empty()) {
        date = currentDate;
    }

    if (!utils::isValidDate(date)) {
        std::cout << "Invalid date format.\n";
        return;
    }

    double consumedCalories = logger->calculateTotalCalories(date, database->getAllFoods());
    double targetCalories = currentUser->calculateTargetCalories();

    std::cout << "\nCalorie Summary for " << date << ":\n"
              << "Consumed: " << consumedCalories << " calories\n"
              << "Target: " << targetCalories << " calories\n"
              << "Difference: " << (consumedCalories - targetCalories) << " calories\n";
}

void YADA::showLoginMenu() {
    std::cout << "\n=== YADA Login Menu ===\n";
    std::cout << "1. Login\n";
    std::cout << "2. Register\n";
    std::cout << "3. Exit\n";
    std::cout << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            registerUser();
            break;
        case 3:
            std::cout << "Goodbye!\n";
            exit(0);
        default:
            std::cout << "Invalid choice.\n";
    }
}

void YADA::login() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    auto it = users.find(username);
    if (it != users.end() && utils::verifyPassword(password, it->second->getPasswordHash())) {
        currentUser = it->second;
        logger = std::make_unique<Logger>("data/daily_logs", username);
        std::cout << "Login successful!\n";
    } else {
        std::cout << "Invalid username or password.\n";
    }
}

void YADA::registerUser() {
    std::cout << "\nPassword Requirements:\n"
              << "- At least 8 characters long\n"
              << "- Must contain at least one uppercase letter\n"
              << "- Must contain at least one lowercase letter\n"
              << "- Must contain at least one number\n"
              << "- Must contain at least one special character\n\n";
    
    std::string username, password;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    
    if (!utils::isValidUsername(username)) {
        std::cout << "Invalid username format.\n";
        return;
    }

    if (!utils::isValidPassword(password)) {
        std::cout << "Invalid password format.\n";
        return;
    }

    if (users.find(username) != users.end()) {
        std::cout << "Username already exists.\n";
        return;
    }

    std::string genderStr;
    double height;
    int age;
    double weight;
    int activityLevel;

    std::cout << "Gender (M/F/O): ";
    std::getline(std::cin, genderStr);
    std::cout << "Height (cm): ";
    std::cin >> height;
    std::cout << "Age: ";
    std::cin >> age;
    std::cout << "Weight (kg): ";
    std::cin >> weight;
    std::cout << "Activity level (1-5):\n"
              << "1. Sedentary\n"
              << "2. Lightly Active\n"
              << "3. Moderately Active\n"
              << "4. Very Active\n"
              << "5. Extra Active\n"
              << "Choice: ";
    std::cin >> activityLevel;
    std::cin.ignore();

    Gender gender = (genderStr == "M" || genderStr == "m") ? Gender::MALE :
                   (genderStr == "F" || genderStr == "f") ? Gender::FEMALE : Gender::OTHER;

    if (activityLevel >= 1 && activityLevel <= 5) {
        if (registerUser(username, password, gender, height, age, weight,
                       static_cast<ActivityLevel>(activityLevel - 1))) {
            if (login(username, password)) {
                showMainMenu();
            }
        }
    } else {
        std::cout << "Invalid activity level.\n";
    }
}

void YADA::run() {
    while (true) {
        if (!currentUser) {
            showLoginMenu();
        } else {
            showMainMenu();
        }
    }
}

int main() {
    YADA yada;
    yada.run();
    return 0;
} 