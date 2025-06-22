# Final Project Data Structure 2025

This project is a console-based application designed to manage vehicle component data using **binary files**, developed as a final project for the Data Structure course (Class A), 2025.

## Group Project (Group 08)

|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241135 | Farikh Muhammad Fauzan |
| 5025241140 | Brave Juliada |

## 📦 Features

- Add new component data
- Edit existing component data
- Display all data with indexing (sorted by Component Number)
- Display all data without indexing (based on file order)
- Delete component data
- Calculate and display total asset value of all components
- Interactive menu navigation (via numbers, underlined letters, or arrow keys)
- Minimum of 50 records handled via binary file operations

## 🧱 Data Structure

```c
typedef struct {
    char number[7];      // 6-digit component number
    char name[26];       // Component name (up to 25 characters + null terminator)
    int stock;           // Available stock quantity
    float price;         // Unit price
} Component;
