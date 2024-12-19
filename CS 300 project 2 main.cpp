//============================================================================
// Name        : HashTable.cpp
// Author      : Josh Hall
// Version     : 1.0
// Description : Project 2
//============================================================================
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

// Class to represent a course
class Course {
public:
    std::string number; // Course number
    std::string title; // Course title
    std::vector<std::string> prerequisites; // List of prerequisite course numbers

    // Default constructor
    Course() : number(""), title(""), prerequisites({}) {}

    // Parameterized constructor
    Course(std::string courseNumber, std::string courseTitle, std::vector<std::string> coursePrerequisites)
        : number(courseNumber), title(courseTitle), prerequisites(coursePrerequisites) {}
};

// Function to load courses from a file into the course table
void LoadCoursesFromFile(std::unordered_map<std::string, Course>& courseTable, const std::string& filePath) {
    std::cout << "Attempting to load file: " << filePath << std::endl; // Debug statement
    std::ifstream file(filePath);
    if (!file.is_open()) { // Check if file was opened successfully
        std::cerr << "Error: Unable to open file" << std::endl;
        return;
    }

    std::unordered_set<std::string> courseNumbers; // Set to store course numbers for validation
    std::string line;

    // First pass: validate data and collect course numbers
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (getline(iss, token, ',')) { // Split the line by commas
            tokens.push_back(token);
        }

        if (tokens.size() < 2) { // Check if the line has at least 2 tokens
            std::cerr << "Error: Invalid line format" << std::endl;
            continue;
        }

        std::string courseNumber = tokens[0]; // First token is the course number
        std::vector<std::string> prerequisites(tokens.begin() + 2, tokens.end()); // Remaining tokens are prerequisites

        // Validate prerequisites
        for (const auto& prerequisite : prerequisites) {
            if (courseNumbers.find(prerequisite) == courseNumbers.end()) { // Check if prerequisite exists in courseNumbers
                std::cerr << "Error: Prerequisite " << prerequisite << " not found in course list" << std::endl;
            }
        }

        courseNumbers.insert(courseNumber); // Add course number to the set
    }
    file.close();

    // Second pass: create course objects and store them in the hash table
    file.open(filePath);
    if (!file.is_open()) { // Check if file was opened successfully
        std::cerr << "Error: Unable to open file" << std::endl;
        return;
    }

    while (getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (getline(iss, token, ',')) { // Split the line by commas
            tokens.push_back(token);
        }

        std::string courseNumber = tokens[0]; // First token is the course number
        std::string courseTitle = tokens[1]; // Second token is the course title
        std::vector<std::string> prerequisites(tokens.begin() + 2, tokens.end()); // Remaining tokens are prerequisites

        // Create a Course object and insert it into the course table
        Course course(courseNumber, courseTitle, prerequisites);
        courseTable[courseNumber] = course;
    }

    file.close();
    std::cout << "File loaded successfully." << std::endl; // Debug statement
}

// Function to print course information based on the course number
void PrintCourse(const std::unordered_map<std::string, Course>& courseTable, const std::string& courseNumber) {
    auto it = courseTable.find(courseNumber); // Find the course in the hash table
    if (it == courseTable.end()) { // Check if the course was found
        std::cout << "Course not found" << std::endl;
        return;
    }

    // Print course information
    const Course& course = it->second;
    std::cout << "Course Number: " << course.number << std::endl;
    std::cout << "Course Title: " << course.title << std::endl;
    if (!course.prerequisites.empty()) { // Print prerequisites if any
        std::cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            std::cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "Prerequisites: None" << std::endl;
    }
}

// Function to print all courses sorted alphanumerically by course number
void PrintAllCourses(const std::unordered_map<std::string, Course>& courseTable) {
    std::vector<std::string> courseNumbers; // Vector to store course numbers
    for (const auto& pair : courseTable) {
        courseNumbers.push_back(pair.first); // Add course numbers to the vector
    }
    std::sort(courseNumbers.begin(), courseNumbers.end()); // Sort course numbers alphanumerically

    // Print each course's information
    for (const auto& courseNumber : courseNumbers) {
        const Course& course = courseTable.at(courseNumber);
        std::cout << "Course Number: " << course.number << ", Title: " << course.title << std::endl;
    }
}

// Function to display the menu and handle user input
void ShowMenu(std::unordered_map<std::string, Course>& courseTable) {
    while (true) {
        // Display menu options
        std::cout << "1. Load Courses" << std::endl;
        std::cout << "2. Print All Courses" << std::endl;
        std::cout << "3. Print Course Information" << std::endl;
        std::cout << "9. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Ignore the newline character after choice input
        std::string courseNumber;

        switch (choice) {
        case 1: {
            // Load courses from a specified file
            std::cout << "Enter file name: ";
            std::string fileName;
            std::getline(std::cin, fileName); // Use getline to capture the entire file name
            std::cout << "Filename entered: " << fileName << std::endl; // Debug statement
            LoadCoursesFromFile(courseTable, fileName);
            break;
        }
        case 2:
            // Print all courses sorted alphanumerically
            PrintAllCourses(courseTable);
            break;
        case 3:
            // Print information for a specific course
            std::cout << "Enter course number: ";
            std::getline(std::cin, courseNumber); // Use getline to capture the entire course number
            PrintCourse(courseTable, courseNumber);
            break;
        case 9:
            // Exit the program
            return;
        default:
            // Handle invalid menu choice
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
    }
}

// Main function
int main() {
    std::unordered_map<std::string, Course> courseTable; // Hash table to store courses
    ShowMenu(courseTable); // Show menu and handle user input
    return 0;
}
