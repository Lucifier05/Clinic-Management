#include "clinic.h"
#include <iostream>
#include <string>

int main() {
    Clinic clinic;
    char userType, action;
    std::string name, specialty;
    int roomNum, age;
    bool emergency;
    
    while (true) {
        std::cout << "\nType D for Doctor or P for Patient (Q to quit): ";
        std::cin >> userType;
        
        if (toupper(userType) == 'Q') break;
        
        std::cout << "Type I for check-in or O for checkout: ";
        std::cin >> action;
        
        if (toupper(userType) == 'D') {
            if (toupper(action) == 'I') {
                std::cout << "Enter doctor's name: ";
                std::cin >> name;
                std::cout << "Enter room number (1-100): ";
                std::cin >> roomNum;
                std::cout << "Enter specialty code: ";
                std::cin >> specialty;
                clinic.doctorCheckIn(name, roomNum, specialty);
            } else if (toupper(action) == 'O') {
                std::cout << "Enter doctor's name: ";
                std::cin >> name;
                clinic.doctorCheckOut(name);
            }
        } else if (toupper(userType) == 'P') {
            if (toupper(action) == 'I') {
                std::cout << "Enter patient's name: ";
                std::cin >> name;
                std::cout << "Enter age: ";
                std::cin >> age;
                std::cout << "Enter specialty code needed: ";
                std::cin >> specialty;
                std::cout << "Is this an emergency? (1 for yes, 0 for no): ";
                std::cin >> emergency;
                clinic.patientCheckIn(name, age, specialty, emergency);
            } else if (toupper(action) == 'O') {
                std::cout << "Enter patient's name: ";
                std::cin >> name;
                clinic.patientCheckOut(name);
            }
        }
    }
    
    return 0;
}