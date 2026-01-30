#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

// Student class to represent individual student records
class Student {
private:
    int rollNumber;
    string name;
    string course;
    float cgpa;
    string email;

public:
    // Default constructor
    Student() : rollNumber(0), name(""), course(""), cgpa(0.0), email("") {}
    
    // Parameterized constructor
    Student(int roll, string n, string c, float gpa, string e) 
        : rollNumber(roll), name(n), course(c), cgpa(gpa), email(e) {}
    
    // Getters
    int getRollNumber() const { return rollNumber; }
    string getName() const { return name; }
    string getCourse() const { return course; }
    float getCGPA() const { return cgpa; }
    string getEmail() const { return email; }
    
    // Setters
    void setName(string n) { name = n; }
    void setCourse(string c) { course = c; }
    void setCGPA(float gpa) { cgpa = gpa; }
    void setEmail(string e) { email = e; }
    
    // Display student information
    void display() const {
        cout << left << setw(12) << rollNumber 
             << setw(25) << name 
             << setw(20) << course 
             << setw(8) << fixed << setprecision(2) << cgpa 
             << setw(30) << email << endl;
    }
    
    // Write student data to file
    void writeToFile(ofstream &file) const {
        file << rollNumber << endl;
        file << name << endl;
        file << course << endl;
        file << cgpa << endl;
        file << email << endl;
    }
    
    // Read student data from file
    void readFromFile(ifstream &file) {
        file >> rollNumber;
        file.ignore();
        getline(file, name);
        getline(file, course);
        file >> cgpa;
        file.ignore();
        getline(file, email);
    }
};

// StudentManagementSystem class to handle all operations
class StudentManagementSystem {
private:
    vector<Student> students;
    const string filename = "students.dat";
    
    // Load all students from file
    void loadFromFile() {
        ifstream file(filename);
        if (!file) {
            return; // File doesn't exist yet
        }
        
        students.clear();
        Student temp;
        while (file.peek() != EOF) {
            temp.readFromFile(file);
            if (file) {
                students.push_back(temp);
            }
        }
        file.close();
        cout << "Data loaded successfully!" << endl;
    }
    
    // Save all students to file
    void saveToFile() {
        ofstream file(filename);
        if (!file) {
            cout << "Error: Unable to open file for writing!" << endl;
            return;
        }
        
        for (const auto &student : students) {
            student.writeToFile(file);
        }
        file.close();
    }
    
    // Find student by roll number
    int findStudentIndex(int rollNumber) {
        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].getRollNumber() == rollNumber) {
                return i;
            }
        }
        return -1;
    }
    
    // Display table header
    void displayHeader() const {
        cout << "\n" << string(100, '=') << endl;
        cout << left << setw(12) << "Roll No" 
             << setw(25) << "Name" 
             << setw(20) << "Course" 
             << setw(8) << "CGPA" 
             << setw(30) << "Email" << endl;
        cout << string(100, '=') << endl;
    }
    
public:
    // Constructor - load data from file
    StudentManagementSystem() {
        loadFromFile();
    }
    
    // Destructor - save data to file
    ~StudentManagementSystem() {
        saveToFile();
    }
    
    // Add a new student
    void addStudent() {
        int rollNumber;
        string name, course, email;
        float cgpa;
        
        cout << "\n--- Add New Student ---" << endl;
        cout << "Enter Roll Number: ";
        cin >> rollNumber;
        
        // Check if roll number already exists
        if (findStudentIndex(rollNumber) != -1) {
            cout << "Error: Student with Roll Number " << rollNumber << " already exists!" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Name: ";
        getline(cin, name);
        
        cout << "Enter Course: ";
        getline(cin, course);
        
        cout << "Enter CGPA (0.0 - 10.0): ";
        cin >> cgpa;
        
        // Validate CGPA
        while (cgpa < 0.0 || cgpa > 10.0) {
            cout << "Invalid CGPA! Enter value between 0.0 and 10.0: ";
            cin >> cgpa;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Email: ";
        getline(cin, email);
        
        Student newStudent(rollNumber, name, course, cgpa, email);
        students.push_back(newStudent);
        saveToFile();
        
        cout << "\nStudent added successfully!" << endl;
    }
    
    // Display all students
    void displayAllStudents() {
        if (students.empty()) {
            cout << "\nNo student records found!" << endl;
            return;
        }
        
        displayHeader();
        for (const auto &student : students) {
            student.display();
        }
        cout << string(100, '=') << endl;
        cout << "Total Students: " << students.size() << endl;
    }
    
    // Search for a student by roll number
    void searchStudent() {
        int rollNumber;
        cout << "\nEnter Roll Number to search: ";
        cin >> rollNumber;
        
        int index = findStudentIndex(rollNumber);
        
        if (index == -1) {
            cout << "Student with Roll Number " << rollNumber << " not found!" << endl;
        } else {
            displayHeader();
            students[index].display();
            cout << string(100, '=') << endl;
        }
    }
    
    // Search students by name (partial match)
    void searchByName() {
        string searchName;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nEnter name to search: ";
        getline(cin, searchName);
        
        // Convert search term to lowercase for case-insensitive search
        transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
        
        vector<Student> foundStudents;
        for (const auto &student : students) {
            string studentName = student.getName();
            transform(studentName.begin(), studentName.end(), studentName.begin(), ::tolower);
            
            if (studentName.find(searchName) != string::npos) {
                foundStudents.push_back(student);
            }
        }
        
        if (foundStudents.empty()) {
            cout << "No students found with name containing '" << searchName << "'" << endl;
        } else {
            displayHeader();
            for (const auto &student : foundStudents) {
                student.display();
            }
            cout << string(100, '=') << endl;
            cout << "Found " << foundStudents.size() << " student(s)" << endl;
        }
    }
    
    // Update student information
    void updateStudent() {
        int rollNumber;
        cout << "\nEnter Roll Number of student to update: ";
        cin >> rollNumber;
        
        int index = findStudentIndex(rollNumber);
        
        if (index == -1) {
            cout << "Student with Roll Number " << rollNumber << " not found!" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        
        cout << "\nCurrent Details:" << endl;
        displayHeader();
        students[index].display();
        cout << string(100, '=') << endl;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "\nEnter new details (press Enter to keep current value):" << endl;
        
        string input;
        
        cout << "Name [" << students[index].getName() << "]: ";
        getline(cin, input);
        if (!input.empty()) {
            students[index].setName(input);
        }
        
        cout << "Course [" << students[index].getCourse() << "]: ";
        getline(cin, input);
        if (!input.empty()) {
            students[index].setCourse(input);
        }
        
        cout << "CGPA [" << students[index].getCGPA() << "]: ";
        getline(cin, input);
        if (!input.empty()) {
            float cgpa = stof(input);
            while (cgpa < 0.0 || cgpa > 10.0) {
                cout << "Invalid CGPA! Enter value between 0.0 and 10.0: ";
                getline(cin, input);
                cgpa = stof(input);
            }
            students[index].setCGPA(cgpa);
        }
        
        cout << "Email [" << students[index].getEmail() << "]: ";
        getline(cin, input);
        if (!input.empty()) {
            students[index].setEmail(input);
        }
        
        saveToFile();
        cout << "\nStudent record updated successfully!" << endl;
    }
    
    // Delete a student
    void deleteStudent() {
        int rollNumber;
        cout << "\nEnter Roll Number of student to delete: ";
        cin >> rollNumber;
        
        int index = findStudentIndex(rollNumber);
        
        if (index == -1) {
            cout << "Student with Roll Number " << rollNumber << " not found!" << endl;
            return;
        }
        
        cout << "\nStudent to be deleted:" << endl;
        displayHeader();
        students[index].display();
        cout << string(100, '=') << endl;
        
        char confirm;
        cout << "\nAre you sure you want to delete this record? (y/n): ";
        cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            students.erase(students.begin() + index);
            saveToFile();
            cout << "Student record deleted successfully!" << endl;
        } else {
            cout << "Deletion cancelled." << endl;
        }
    }
    
    // Display students by course
    void displayByCourse() {
        string course;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nEnter course name: ";
        getline(cin, course);
        
        vector<Student> courseStudents;
        for (const auto &student : students) {
            if (student.getCourse() == course) {
                courseStudents.push_back(student);
            }
        }
        
        if (courseStudents.empty()) {
            cout << "No students found in course: " << course << endl;
        } else {
            cout << "\nStudents in " << course << ":" << endl;
            displayHeader();
            for (const auto &student : courseStudents) {
                student.display();
            }
            cout << string(100, '=') << endl;
            cout << "Total: " << courseStudents.size() << " student(s)" << endl;
        }
    }
};

// Function to display menu
void displayMenu() {
    cout << "\n";
    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "║   STUDENT MANAGEMENT SYSTEM               ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    cout << "  1. Add New Student" << endl;
    cout << "  2. Display All Students" << endl;
    cout << "  3. Search Student by Roll Number" << endl;
    cout << "  4. Search Student by Name" << endl;
    cout << "  5. Update Student Information" << endl;
    cout << "  6. Delete Student" << endl;
    cout << "  7. Display Students by Course" << endl;
    cout << "  8. Exit" << endl;
    cout << "════════════════════════════════════════════" << endl;
    cout << "Enter your choice: ";
}

int main() {
    StudentManagementSystem sms;
    int choice;
    
    cout << "\n*** Welcome to Student Management System ***\n" << endl;
    
    do {
        displayMenu();
        cin >> choice;
        
        // Input validation
        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number: ";
            cin >> choice;
        }
        
        switch (choice) {
            case 1:
                sms.addStudent();
                break;
            case 2:
                sms.displayAllStudents();
                break;
            case 3:
                sms.searchStudent();
                break;
            case 4:
                sms.searchByName();
                break;
            case 5:
                sms.updateStudent();
                break;
            case 6:
                sms.deleteStudent();
                break;
            case 7:
                sms.displayByCourse();
                break;
            case 8:
                cout << "\nThank you for using Student Management System!" << endl;
                cout << "Goodbye!\n" << endl;
                break;
            default:
                cout << "Invalid choice! Please select a valid option (1-8)." << endl;
        }
        
        if (choice != 8) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while (choice != 8);
    
    return 0;
}