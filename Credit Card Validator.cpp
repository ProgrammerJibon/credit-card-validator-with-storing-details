#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <windows.h>
using namespace std;


bool newFile(const string& fileName, const string& content) {
    string directoryPath = "./cards";



    string filePath = directoryPath + "/" + fileName;

    ofstream file(filePath);

    if (file.is_open()) {
        file << content;
        file.close();
        return true;
    } else {
        return false;
    }
}


bool isValidMasterCard(const string& cardNumber) {
    if (cardNumber.length() != 16) {
        return false;
    }

    if (cardNumber[0] != '5') {
        return false;
    }

    return true;
}

bool isValidVisaCard(const string& cardNumber) {
    if (cardNumber.length() != 16 && cardNumber.length() != 13) {
        return false;
    }

    if (cardNumber[0] != '4') {
        return false;
    }

    return true;
}

bool isValidExpiryDate(int inputYear, int inputMonth) {
    time_t now;
    time(&now);
    struct tm* currentTime = localtime(&now);

    int currentYear = currentTime->tm_year + 1900;
    int currentMonth = currentTime->tm_mon + 1;

    if (inputYear > currentYear) {
        return true;
    } else if (inputYear == currentYear) {
        return inputMonth >= currentMonth;
    }

    return false;
}

bool isValidCVC(const string& cvc) {
    return (cvc.length() == 3);
}

bool isValidFullName(const string& fullName) {
    return (fullName.length() >= 3);
}

bool cardValidator(){
    system("cls");
    system("color 0");
    system("title Validator");

    cout << "\n\n\t\t**************************************\n\n\t\t\t";
    cout << "Credit card validator";
    cout << "\n\n\t\t**************************************\n\n\n";

    string cardNumber, cvc, fullName, cardName;
    int expiryYear, expiryMonth;

    cout << "\n\tEnter the credit card number: ";
    cin >> cardNumber;

    if (!isValidMasterCard(cardNumber) && !isValidVisaCard(cardNumber)) {
        cout << "\n\n\tInvalid credit card number.\n\n";
        system("color c");
        return 1;
    }
    if(isValidMasterCard(cardNumber)){
        cardName = "Master Card";
    }else if(isValidVisaCard(cardNumber)){
        cardName = "Visa Card";
    }


    cout << "\n\tEnter the card expiry month (MM): ";
    cin >> expiryMonth;

    cout << "\n\tEnter the card expiry year (YY): ";
    cin >> expiryYear;

    expiryYear = 2000+expiryYear;

    if (!isValidExpiryDate(expiryYear, expiryMonth)) {
        cout << "\n\n\tInvalid expiry date.\n\n";
        system("color c");
        return 1;
    }

    cout << "\n\tEnter the CVC: ";
    cin >> cvc;

    if (!isValidCVC(cvc)) {
        cout << "\n\n\tInvalid CVC.\n\n";
        return 1;
    }

    cout << "\n\tEnter the cardholder's full name: ";
    cin.ignore();
    getline(cin, fullName);

    if (!isValidFullName(fullName)) {
        cout << "\n\n\tInvalid full name.\n\n";
        return 1;
    }

    system("color 0a");
    system("cls");
    system("title Valid Card Details");

    string content;
    content += "\n\t\tCredit Card Number: " + cardNumber + "\n";
    content += "\n\t\tFull Name: " + fullName + "\n";
    content += "\n\t\tCard Name: " + cardName + "\n";
    content += "\n\t\tExpiry Date: " + to_string(expiryMonth) + "/" + to_string(expiryYear) + "\n";
    content += "\n\t\tCVC: " + cvc + "\n";

    cout << endl << "\n\n\t ****************All Details Are Valid**************** ";
    cout << content;
    cout << endl << "\t ***************************************************** \n\n\n\n\n\tPress any key to go back to main menu...\n";
    newFile(cardNumber+".dat", content);
    getchar();

}


bool displayDatFilesContent() {

    string directoryPath = "./cards";
    system("cls");

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directoryPath + "\\*.dat").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
                string filePath = directoryPath + "\\" + findFileData.cFileName;

                string fileNameWithoutExtension = findFileData.cFileName;
                size_t extensionPos = fileNameWithoutExtension.find_last_of(".");
                if (extensionPos != string::npos) {
                    fileNameWithoutExtension = fileNameWithoutExtension.substr(0, extensionPos);
                }

                ifstream file(filePath);
                if (file.is_open()) {
                    cout << "\n\n\n\t\t\t" << fileNameWithoutExtension;
                    cout << "\n _____________________________________________________________________ ";
                    cout << "\n\n";
                    string line;
                    while (getline(file, line)) {
                        cout << line << endl;
                    }
                    file.close();
                    cout << "\n";
                    cout << "\n --------------------------------------------------------------------- \n";
                } else {
                    cerr << "Error opening file: " << filePath << endl;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    } else {
        cerr << "\n\n\t\t\tNo card found \n\n\t\t ";
    }
    cout << "\n  ";
    system("pause");
}


bool deleteFile(string cardNumber) {
    system("cls");
    if(cardNumber.length() > 0){
        cout << "No card found for "+cardNumber+"\n\n";
    }
    cout << "\n\n\t\t\tEnter 0 (zero) to go back to Main Menu\n\n\t\t\tOr ";
    cout << "enter the card number to delete: ";
    cin >> cardNumber;
    if(cardNumber == "0"){
        return false;
    }
    string filePath = "./cards/"+cardNumber+".dat";
    if (DeleteFile(filePath.c_str())) {
        cout << "File '" << cardNumber << "' deleted successfully." << endl;
    } else {
        cerr << "Error deleting '" << cardNumber << "'. Error code: " << GetLastError() << endl;
        return deleteFile(cardNumber);
    }
    system("pause");
    return true;
}



void setConsoleColumnSize(int columns) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    SMALL_RECT windowSize = {0, 0, static_cast<SHORT>(columns - 1), csbi.srWindow.Bottom};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}




int main() {

    setConsoleColumnSize(71);

    system("cls");
    system("color 0");
    system("title Credit card validator for PBL 2023");

    cout << "\n\n\t\t**************************************\n\n\t\t\t";
    cout << "Credit card validator";
    cout << "\n\n\t\t**************************************\n\n\n";


    int option;
    cout << "\n\n\t\t1. Validate and Add new card";
    cout << "\n\n\t\t2. View All cards";
    cout << "\n\n\t\t3. Delete a card";
    cout << "\n\n\t\t4. Exit";
    cout << "\n\n\t\tChoose an Option: ";
    cin >> option;
    if(option == 1){
        cardValidator();
    }else if(option == 2){
        displayDatFilesContent();
    }else if(option == 3){
        deleteFile("");
    }else if(option == 4){
        system("cls");
        return 0;
    }
    return main();
}
