# YADA (Yet Another Diet Assistant)

YADA is a comprehensive diet management system that helps users track their daily food intake, manage their diet goals, and maintain a healthy lifestyle.

## Features

- User Authentication (Login/Registration)
- Food Database Management
  - Basic foods with calories and keywords
  - Composite foods creation from basic foods
  - Persistent storage in text files
- Daily Food Log Management
  - Add/Delete foods with serving counts
  - View and update logs for any date
  - Undo functionality for all operations
- Diet Goal Profile
  - Track user's gender, height, age, weight, and activity level
  - Multiple calorie calculation methods
  - Daily calorie tracking and comparison

## Project Structure

```
yada/
├── src/
│   ├── main.cpp
│   ├── user/
│   │   ├── user.h
│   │   └── user.cpp
│   ├── food/
│   │   ├── food.h
│   │   ├── food.cpp
│   │   ├── basic_food.h
│   │   ├── basic_food.cpp
│   │   ├── composite_food.h
│   │   └── composite_food.cpp
│   ├── database/
│   │   ├── database.h
│   │   └── database.cpp
│   ├── logger/
│   │   ├── logger.h
│   │   └── logger.cpp
│   └── utils/
│       ├── utils.h
│       └── utils.cpp
├── data/
│   ├── users.txt
│   ├── basic_foods.txt
│   ├── composite_foods.txt
│   └── daily_logs/
├── include/
├── build/
└── README.md
```

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Running the Program

```bash
./yada
```

## Data Files

- `users.txt`: Stores user registration information
- `basic_foods.txt`: Contains basic food database
- `composite_foods.txt`: Contains composite food definitions
- `daily_logs/`: Directory containing daily food logs

## Debug Mode

The program includes debug print statements that can be enabled by setting the DEBUG flag during compilation.

## Requirements

- C++17 or later
- CMake 3.10 or later
- Standard C++ Library

## Notes

- All data is stored in text files for easy viewing and modification
- The system supports multiple calorie calculation methods
- Undo functionality is available for all operations
- The program maintains separate databases for basic and composite foods 