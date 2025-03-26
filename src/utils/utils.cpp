#include "utils/utils.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rand.h>
#include <iostream>

namespace utils {

std::vector<std::string> splitString(const std::string& str, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            result.push_back(trimString(item));
        }
    }
    return result;
}

std::string trimString(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string getCurrentDate() {
    auto now = std::time(nullptr);
    return formatDate(now);
}

std::string formatDate(const std::time_t& timestamp) {
    std::tm* tm = std::localtime(&timestamp);
    std::stringstream ss;
    ss << std::put_time(tm, "%Y-%m-%d");
    return ss.str();
}

std::time_t parseDate(const std::string& date) {
    std::tm tm = {};
    std::stringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::mktime(&tm);
}

bool createDirectory(const std::string& path) {
    return std::filesystem::create_directories(path);
}

bool fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

std::string getFileExtension(const std::string& path) {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    return path.substr(pos);
}

bool isValidDate(const std::string& date) {
    std::tm tm = {};
    std::stringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return !ss.fail();
}

bool isValidNumber(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), 
           [](char c) { return std::isdigit(c) || c == '.' || c == '-'; });
}

bool isValidUsername(const std::string& username) {
    // Username must be 3-20 characters long and contain only letters, numbers, and underscores
    if (username.length() < 3 || username.length() > 20) return false;
    return std::all_of(username.begin(), username.end(),
           [](char c) { return std::isalnum(c) || c == '_'; });
}

bool isValidPassword(const std::string& password) {
    // Password must be at least 8 characters long and contain at least one uppercase letter,
    // one lowercase letter, one number, and one special character
    if (password.length() < 8) return false;
    
    bool hasUpper = false, hasLower = false, hasNumber = false, hasSpecial = false;
    for (char c : password) {
        if (std::isupper(c)) hasUpper = true;
        else if (std::islower(c)) hasLower = true;
        else if (std::isdigit(c)) hasNumber = true;
        else hasSpecial = true;
    }
    
    return hasUpper && hasLower && hasNumber && hasSpecial;
}

std::string hashPassword(const std::string& password) {
    unsigned char salt[16];
    RAND_bytes(salt, sizeof(salt));
    
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, salt, sizeof(salt));
    EVP_DigestUpdate(ctx, password.c_str(), password.length());
    EVP_DigestFinal_ex(ctx, hash, &hashLen);
    EVP_MD_CTX_free(ctx);
    
    // Combine salt and hash
    std::stringstream ss;
    for (unsigned char c : salt) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    for (unsigned int i = 0; i < hashLen; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    
    return ss.str();
}

bool verifyPassword(const std::string& password, const std::string& hash) {
    // Extract salt from hash
    unsigned char salt[16];
    for (size_t i = 0; i < sizeof(salt); ++i) {
        std::string byte = hash.substr(i * 2, 2);
        salt[i] = static_cast<unsigned char>(std::stoi(byte, nullptr, 16));
    }
    
    // Calculate hash with same salt
    unsigned char calculatedHash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, salt, sizeof(salt));
    EVP_DigestUpdate(ctx, password.c_str(), password.length());
    EVP_DigestFinal_ex(ctx, calculatedHash, &hashLen);
    EVP_MD_CTX_free(ctx);
    
    // Compare hashes
    std::stringstream ss;
    for (unsigned int i = 0; i < hashLen; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(calculatedHash[i]);
    }
    
    return ss.str() == hash.substr(sizeof(salt) * 2);
}

#ifdef DEBUG
void printDebug(const std::string& message) {
    std::cout << "DEBUG: " << message << std::endl;
}
#endif

} // namespace utils 