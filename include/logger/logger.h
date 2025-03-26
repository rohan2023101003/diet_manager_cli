#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <ctime>
#include "food/food.h"

struct LogEntry {
    std::string foodId;
    int servings;
    std::time_t timestamp;
};

class Logger {
private:
    std::map<std::string, std::vector<LogEntry>> dailyLogs;
    std::string logDirectory;
    std::string username;
    std::vector<std::pair<std::string, std::vector<LogEntry>>> undoStack;

    void loadLog(const std::string& date);
    void saveLog(const std::string& date) const;
    std::string getLogFilePath(const std::string& date) const;
    void pushUndoState(const std::string& date);

public:
    Logger(const std::string& logDirectory, const std::string& username);

    // Log operations
    void addEntry(const std::string& date, const std::string& foodId, int servings);
    void removeEntry(const std::string& date, size_t index);
    std::vector<LogEntry> getLog(const std::string& date) const;
    double calculateTotalCalories(const std::string& date, const std::map<std::string, std::shared_ptr<Food>>& foodDatabase) const;

    // Undo operations
    void undo();
    bool canUndo() const;

    // File operations
    void save() const;
    void load();

    #ifdef DEBUG
    void debugPrint() const;
    #endif
}; 