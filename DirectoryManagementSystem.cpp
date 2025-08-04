#include <iostream>
#include <string>
#include <direct.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Function to display the main menu
void mainMenu() {
    cout << "[1] List Files\n";
    cout << "[2] Create Directory\n";
    cout << "[3] Change Directory\n";
    cout << "[4] Exit\n";
    cout << "Enter option: ";
}

// Function to display the list files submenu
void listFilesMenu() {
    cout << "[1] List All Files\n";
    cout << "[2] List Files by Extension (e.g., .txt)\n";
    cout << "[3] List Files by Pattern (e.g., moha*.*)\n";
    cout << "Enter choice: ";
}

// Function to list files based on user selection
void listFiles() {
    int fileListOption;
    listFilesMenu();
    while (!(cin >> fileListOption)) {
        cout << "Invalid input. Enter a number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(); // Clear input buffer

    try {
        if (fileListOption == 1) {
            cout << "\nFiles in current directory:\n";
            for (const auto& fileItem : fs::directory_iterator(fs::current_path())) {
                cout << "- " << fileItem.path().filename().string() << endl;
            }
        }
        else if (fileListOption == 2) {
            string userExtension;
            cout << "Enter file extension (include the dot, e.g., .txt): ";
            getline(cin, userExtension);
            cout << "\nListing files with extension " << userExtension << ":\n";
            bool isFileFound = false;
            for (const auto& fileItem : fs::directory_iterator(fs::current_path())) {
                if (fileItem.path().extension().string() == userExtension) {
                    cout << fileItem.path().filename().string() << endl;
                    isFileFound = true;
                }
            }
            if (!isFileFound) {
                cout << "No files found with extension " << userExtension << "!\n";
            }
        }
        else if (fileListOption == 3) {
            string userPattern;
            cout << "Enter file pattern (e.g., moha*.*): ";
            getline(cin, userPattern);
            cout << "\nListing files matching pattern " << userPattern << ":\n";
            bool isPatternMatched = false;

            size_t starIndex = userPattern.find('*');
            string patternPrefix = (starIndex != string::npos) ? userPattern.substr(0, starIndex) : userPattern;

            for (const auto& fileItem : fs::directory_iterator(fs::current_path())) {
                string fileNameOnly = fileItem.path().filename().string();
                if (fileNameOnly.find(patternPrefix) == 0) {
                    cout << fileNameOnly << endl;
                    isPatternMatched = true;
                }
            }
            if (!isPatternMatched) {
                cout << "No files found matching pattern " << userPattern << "!\n";
            }
        }
        else {
            cout << "Invalid choice!\n";
        }
    }
    catch (const fs::filesystem_error& errorMsg) {
        cout << "Filesystem error: " << errorMsg.what() << endl;
    }
}

// Function to create a new directory
void createDirectory() {
    string newFolderName;
    cout << "\nEnter new directory name: ";
    getline(cin, newFolderName);

    if (_access(newFolderName.c_str(), 0) == 0) {
        cout << "Error: Directory \"" << newFolderName << "\" already exists!\n";
    }
    else {
        try {
            if (fs::create_directory(newFolderName)) {
                cout << "Directory \"" << newFolderName << "\" created successfully.\n";
            }
            else {
                cout << "Error: Failed to create directory '" << newFolderName << "'!\n";
            }
        }
        catch (const fs::filesystem_error& errorMsg) {
            cout << "Filesystem error: " << errorMsg.what() << endl;
        }
    }
}

// Function to display the change directory submenu
void changeDirectoryMenu() {
    cout << "\nChange Directory Menu\n";
    cout << "[1] Move to Parent Directory\n";
    cout << "[2] Move to Root Directory\n";
    cout << "[3] Enter Custom Path (e.g., C:\\Users\\Documents)\n";
    cout << "Enter choice: ";
}

// Function to change the current working directory
void changeDirectory() {
    int directoryOption;
    changeDirectoryMenu();
    while (!(cin >> directoryOption)) {
        cout << "Invalid input. Enter a number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(); // Clear input buffer

    try {
        if (directoryOption == 1) {
            fs::path currentPath = fs::current_path();
            fs::path parentPath = currentPath.parent_path();
            if (parentPath != currentPath) {
                fs::current_path(parentPath);
                cout << "Changed to parent directory: " << fs::current_path().string() << endl;
            }
            else {
                cout << "Error: Already at root or no parent directory exists!\n";
            }
        }
        else if (directoryOption == 2) {
            fs::path rootPath = fs::current_path().root_path();
            fs::current_path(rootPath);
            cout << "Changed to root directory: " << fs::current_path().string() << endl;
        }
        else if (directoryOption == 3) {
            string customDirectoryPath;
            cout << "Enter new directory path (e.g., C:\\Users\\Documents): ";
            getline(cin, customDirectoryPath);
            if (_chdir(customDirectoryPath.c_str()) == 0) {
                cout << "Changed to directory: " << fs::current_path().string() << endl;
            }
            else {
                cout << "Error: Directory \"InvalidPath\" not found!\n";
            }
        }
        else {
            cout << "Invalid choice!\n";
        }
    }
    catch (const fs::filesystem_error& errorMsg) {
        cout << "Filesystem error: " << errorMsg.what() << endl;
    }
}

// Function to handle the main menu and user interaction
void mainMenuLoop() {
    int selectedMenuOption;
    do {
        mainMenu();
        while (!(cin >> selectedMenuOption)) {
            cout << "Invalid input. Enter a number: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(); // Clear input buffer

        switch (selectedMenuOption) {
        case 1:
            listFiles();
            break;
        case 2:
            createDirectory();
            break;
        case 3:
            changeDirectory();
            break;
        case 4:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (selectedMenuOption != 4);
}

int main() {
    mainMenuLoop();
    return 0;
}
