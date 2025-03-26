#pragma once

#include <string>
#include <vector>
#include <ctime>

namespace utils {
    // String manipulation
    std::vector<std::string> splitString(const std::string& str, char delim);
    std::string trimString(const std::string& str);
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);

    // Date handling
    std::string getCurrentDate();
    std::string formatDate(const std::time_t& timestamp);
    std::time_t parseDate(const std::string& date);

    // File operations
    bool createDirectory(const std::string& path);
    bool fileExists(const std::string& path);
    std::string getFileExtension(const std::string& path);

    // Input validation
    bool isValidDate(const std::string& date);
    bool isValidNumber(const std::string& str);
    bool isValidUsername(const std::string& username);
    bool isValidPassword(const std::string& password);

    // Password hashing
    std::string hashPassword(const std::string& password);
    bool verifyPassword(const std::string& password, const std::string& hash);

    // Debug printing
    #ifdef DEBUG
    void printDebug(const std::string& message);
    #endif
} 