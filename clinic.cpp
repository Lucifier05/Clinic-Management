#include "clinic.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <limits.h>


// Patient Implementation
Patient::Patient(std::string n, int a, std::string spec, bool emergency)
    : name(n), age(a), specialtyNeeded(spec), isEmergency(emergency) {}

std::string Patient::getName() const { return name; }
int Patient::getAge() const { return age; }
std::string Patient::getSpecialtyNeeded() const { return specialtyNeeded; }
bool Patient::getIsEmergency() const { return isEmergency; }

// Doctor Implementation
Doctor::Doctor(std::string n, std::string spec, int room)
    : name(n), specialty(spec), roomNumber(room) {}

std::string Doctor::getName() const { return name; }
std::string Doctor::getSpecialty() const { return specialty; }
int Doctor::getRoomNumber() const { return roomNumber; }

void Doctor::addPatient(Patient* patient) {
    if (patient->getIsEmergency()) {
        std::queue<Patient*> tempQueue;
        tempQueue.push(patient);
        while (!waitingList.empty()) {
            tempQueue.push(waitingList.front());
            waitingList.pop();
        }
        waitingList = tempQueue;
    } else {
        waitingList.push(patient);
    }
}

Patient* Doctor::getNextPatient() {
    if (waitingList.empty()) return nullptr;
    Patient* next = waitingList.front();
    waitingList.pop();
    return next;
}

int Doctor::getWaitingListSize() const {
    return waitingList.size();
}

const std::queue<Patient*>& Doctor::getWaitingList() const {
    return waitingList;
}

// Room Implementation
Room::Room() : roomNumber(0), assignedDoctor(nullptr) {}

bool Room::isAvailable() const { return assignedDoctor == nullptr; }
void Room::assignDoctor(Doctor* doc) { assignedDoctor = doc; }
void Room::removeDoctor() { assignedDoctor = nullptr; }
Doctor* Room::getDoctor() const { return assignedDoctor; }
int Room::getRoomNumber() const { return roomNumber; }

// Clinic Implementation
Clinic::Clinic() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        rooms.push(new Room());
    }
}

Clinic::~Clinic() {
    while (!rooms.empty()) {
        delete rooms.front();
        rooms.pop();
    }
    while (!doctors.empty()) {
        delete doctors.front();
        doctors.pop();
    }
    while (!unassignedPatients.empty()) {
        delete unassignedPatients.front();
        unassignedPatients.pop();
    }
}

void Clinic::logTransaction(const std::string& message) {
    static std::ofstream logFile("clinic_transactions.log", std::ios::app);
    logFile << message << std::endl;
    std::cout << message << std::endl;
}

Doctor* Clinic::findBestDoctor(const std::string& specialty, int age) {
    Doctor* bestDoctor = nullptr;
    int shortestWaitList = INT_MAX;
    std::queue<Doctor*> tempDoctors = doctors;
    
    while (!tempDoctors.empty()) {
        Doctor* doc = tempDoctors.front();
        tempDoctors.pop();
        
        if (age < 16 && doc->getSpecialty() == "PED") {
            if (doc->getWaitingListSize() < shortestWaitList) {
                bestDoctor = doc;
                shortestWaitList = doc->getWaitingListSize();
            }
        } else if (doc->getSpecialty() == specialty) {
            if (doc->getWaitingListSize() < shortestWaitList) {
                bestDoctor = doc;
                shortestWaitList = doc->getWaitingListSize();
            }
        }
    }
    
    if (!bestDoctor) {
        tempDoctors = doctors;
        while (!tempDoctors.empty()) {
            Doctor* doc = tempDoctors.front();
            tempDoctors.pop();
            if (doc->getSpecialty() == "GEN" || doc->getSpecialty() == "GP") {
                if (doc->getWaitingListSize() < shortestWaitList) {
                    bestDoctor = doc;
                    shortestWaitList = doc->getWaitingListSize();
                }
            }
        }
    }
    
    return bestDoctor;
}

bool Clinic::doctorCheckIn(const std::string& name, int roomNum, const std::string& specialty) {
    if (roomNum < 1 || roomNum > MAX_ROOMS) {
        logTransaction("Error: Invalid room number " + std::to_string(roomNum));
        return false;
    }

    std::queue<Room*> tempRooms = rooms;
    while (!tempRooms.empty()) {
        Room* room = tempRooms.front();
        tempRooms.pop();
        if (!room->isAvailable() && room->getRoomNumber() == roomNum) {
            logTransaction("Error: Room " + std::to_string(roomNum) + " is already occupied");
            return false;
        }
    }

    Doctor* newDoctor = new Doctor(name, specialty, roomNum);
    doctors.push(newDoctor);
    logTransaction("Doctor " + name + " checked in to room " + std::to_string(roomNum));
    return true;
}

void Clinic::doctorCheckOut(const std::string& name) {
    std::queue<Doctor*> tempDoctors;
    Doctor* departingDoctor = nullptr;
    
    while (!doctors.empty()) {
        Doctor* doc = doctors.front();
        doctors.pop();
        if (doc->getName() == name) {
            departingDoctor = doc;
        } else {
            tempDoctors.push(doc);
        }
    }
    
    doctors = tempDoctors;
    
    if (departingDoctor) {
        reassignPatients(departingDoctor);
        delete departingDoctor;
        logTransaction("Doctor " + name + " checked out. Goodbye!");
    } else {
        logTransaction("Error: Doctor " + name + " not found");
    }
}

void Clinic::reassignPatients(Doctor* departingDoctor) {
    std::queue<Patient*>& waitingList = const_cast<std::queue<Patient*>&>(departingDoctor->getWaitingList());
    while (!waitingList.empty()) {
        Patient* patient = waitingList.front();
        waitingList.pop();
        Doctor* newDoctor = findBestDoctor(patient->getSpecialtyNeeded(), patient->getAge());
        if (newDoctor) {
            newDoctor->addPatient(patient);
            logTransaction("Patient " + patient->getName() + " reassigned to Dr. " + newDoctor->getName());
        } else {
            unassignedPatients.push(patient);
            logTransaction("Patient " + patient->getName() + " added to unassigned queue");
        }
    }
}

void Clinic::patientCheckIn(const std::string& name, int age, const std::string& specialty, bool isEmergency) {
    Patient* newPatient = new Patient(name, age, specialty, isEmergency);
    Doctor* assignedDoctor = findBestDoctor(specialty, age);
    
    if (assignedDoctor) {
        assignedDoctor->addPatient(newPatient);
        logTransaction("Patient " + name + " assigned to Dr. " + assignedDoctor->getName() + 
                      " in room " + std::to_string(assignedDoctor->getRoomNumber()));
    } else {
        unassignedPatients.push(newPatient);
        logTransaction("No doctor available. Patient " + name + " added to waiting list");
    }
}

void Clinic::patientCheckOut(const std::string& name) {
    logTransaction("Patient " + name + " checked out. Goodbye!");
}