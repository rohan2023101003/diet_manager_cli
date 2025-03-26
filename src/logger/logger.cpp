#include "logger/logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <iomanip>

Logger::Logger(const std::string& logDirectory, const std::string& username)
    : logDirectory(logDirectory), username(username) {
    // Create user-specific log directory
    std::string userLogDir = logDirectory + "/" + username;
    if (!std::filesystem::exists(userLogDir)) {
        std::filesystem::create_directories(userLogDir);
    }
    #ifdef DEBUG
    std::cout << "DEBUG: Created Logger object for user " << username 
              << " with directory: " << userLogDir << std::endl;
    #endif
}

void Logger::loadLog(const std::string& date) {
    std::string filePath = getLogFilePath(date);
    std::ifstream file(filePath);
    if (!file.is_open()) {
        #ifdef DEBUG
        std::cout << "DEBUG: Could not open log file: " << filePath << std::endl;
        #endif
        return;
    }

    std::vector<LogEntry> entries;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        LogEntry entry;
        std::string timestampStr;
        
        std::getline(ss, entry.foodId, '|');
        ss >> entry.servings;
        ss.ignore(); // Skip the separator
        std::getline(ss, timestampStr);
        
        entry.timestamp = std::stoll(timestampStr);
        entries.push_back(entry);
    }

    dailyLogs[date] = entries;
    #ifdef DEBUG
    std::cout << "DEBUG: Loaded " << entries.size() << " entries for date: " << date 
              << " for user: " << username << std::endl;
    #endif
}

void Logger::saveLog(const std::string& date) const {
    std::string filePath = getLogFilePath(date);
    std::ofstream file(filePath);
    if (!file.is_open()) {
        #ifdef DEBUG
        std::cout << "DEBUG: Could not open log file for writing: " << filePath << std::endl;
        #endif
        return;
    }

    const auto& entries = dailyLogs.at(date);
    for (const auto& entry : entries) {
        file << entry.foodId << "|" << entry.servings << "|" << entry.timestamp << "\n";
    }
    #ifdef DEBUG
    std::cout << "DEBUG: Saved " << entries.size() << " entries for date: " << date 
              << " for user: " << username << std::endl;
    #endif
}

std::string Logger::getLogFilePath(const std::string& date) const {
    return logDirectory + "/" + username + "/" + date + ".log";
}

void Logger::pushUndoState(const std::string& date) {
    if (dailyLogs.find(date) != dailyLogs.end()) {
        undoStack.push_back({date, dailyLogs[date]});
    }
}

void Logger::addEntry(const std::string& date, const std::string& foodId, int servings) {
    if (dailyLogs.find(date) == dailyLogs.end()) {
        loadLog(date);
    }

    pushUndoState(date);

    LogEntry entry;
    entry.foodId = foodId;
    entry.servings = servings;
    entry.timestamp = std::time(nullptr);
    
    dailyLogs[date].push_back(entry);
    saveLog(date);
    #ifdef DEBUG
    std::cout << "DEBUG: Added entry for food " << foodId 
              << " with " << servings << " servings on " << date << std::endl;
    #endif
}

void Logger::removeEntry(const std::string& date, size_t index) {
    if (dailyLogs.find(date) == dailyLogs.end()) {
        return;
    }

    pushUndoState(date);

    if (index < dailyLogs[date].size()) {
        dailyLogs[date].erase(dailyLogs[date].begin() + index);
        saveLog(date);
        #ifdef DEBUG
        std::cout << "DEBUG: Removed entry at index " << index 
                  << " for date: " << date << std::endl;
        #endif
    }
}

std::vector<LogEntry> Logger::getLog(const std::string& date) const {
    if (dailyLogs.find(date) == dailyLogs.end()) {
        const_cast<Logger*>(this)->loadLog(date);
    }
    auto it = dailyLogs.find(date);
    if (it == dailyLogs.end()) {
        return std::vector<LogEntry>();
    }
    return it->second;
}

double Logger::calculateTotalCalories(const std::string& date, 
    const std::map<std::string, std::shared_ptr<Food>>& foodDatabase) const {
    double total = 0.0;
    const auto& entries = getLog(date);
    
    for (const auto& entry : entries) {
        auto it = foodDatabase.find(entry.foodId);
        if (it != foodDatabase.end()) {
            total += it->second->calculateCalories(entry.servings);
        }
    }
    
    return total;
}

void Logger::undo() {
    if (!undoStack.empty()) {
        const auto& [date, entries] = undoStack.back();
        dailyLogs[date] = entries;
        saveLog(date);
        undoStack.pop_back();
        #ifdef DEBUG
        std::cout << "DEBUG: Undid last operation for date: " << date << std::endl;
        #endif
    }
}

bool Logger::canUndo() const {
    return !undoStack.empty();
}

void Logger::save() const {
    for (const auto& [date, entries] : dailyLogs) {
        saveLog(date);
    }
    #ifdef DEBUG
    std::cout << "DEBUG: Saved all logs" << std::endl;
    #endif
}

void Logger::load() {
    dailyLogs.clear();
    undoStack.clear();
    
    std::string userLogDir = logDirectory + "/" + username;
    if (!std::filesystem::exists(userLogDir)) {
        std::filesystem::create_directories(userLogDir);
    }
    
    for (const auto& entry : std::filesystem::directory_iterator(userLogDir)) {
        if (entry.path().extension() == ".log") {
            std::string date = entry.path().stem().string();
            loadLog(date);
        }
    }
    #ifdef DEBUG
    std::cout << "DEBUG: Loaded all logs for user: " << username << std::endl;
    #endif
}

#ifdef DEBUG
void Logger::debugPrint() const {
    std::cout << "DEBUG: Logger Contents:" << std::endl;
    for (const auto& [date, entries] : dailyLogs) {
        std::cout << "Date: " << date << std::endl;
        std::cout << "Entries:" << std::endl;
        for (const auto& entry : entries) {
            std::cout << "  - Food ID: " << entry.foodId 
                      << ", Servings: " << entry.servings 
                      << ", Timestamp: " << entry.timestamp << std::endl;
        }
    }
    std::cout << "Undo Stack Size: " << undoStack.size() << std::endl;
}
#endif 