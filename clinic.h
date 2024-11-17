#ifndef CLINIC_H
#define CLINIC_H

#include <string>
#include <queue>
#include <limits.h>


class Patient {
private:
    std::string name;
    int age;
    std::string specialtyNeeded;
    bool isEmergency;

public:
    Patient(std::string n, int a, std::string spec, bool emergency = false);
    std::string getName() const;
    int getAge() const;
    std::string getSpecialtyNeeded() const;
    bool getIsEmergency() const;
};

class Doctor {
private:
    std::string name;
    std::string specialty;
    int roomNumber;
    std::queue<Patient*> waitingList;

public:
    Doctor(std::string n, std::string spec, int room);
    std::string getName() const;
    std::string getSpecialty() const;
    int getRoomNumber() const;
    void addPatient(Patient* patient);
    Patient* getNextPatient();
    int getWaitingListSize() const;
    const std::queue<Patient*>& getWaitingList() const;
};

class Room {
private:
    int roomNumber;
    Doctor* assignedDoctor;

public:
    Room();
    bool isAvailable() const;
    void assignDoctor(Doctor* doc);
    void removeDoctor();
    Doctor* getDoctor() const;
    int getRoomNumber() const;
};

class Clinic {
private:
    std::queue<Room*> rooms;
    std::queue<Doctor*> doctors;
    std::queue<Patient*> unassignedPatients;
    static const int MAX_ROOMS = 100;

    void logTransaction(const std::string& message);
    Doctor* findBestDoctor(const std::string& specialty, int age);
    void reassignPatients(Doctor* departingDoctor);

public:
    Clinic();
    ~Clinic();
    bool doctorCheckIn(const std::string& name, int roomNum, const std::string& specialty);
    void doctorCheckOut(const std::string& name);
    void patientCheckIn(const std::string& name, int age, const std::string& specialty, bool isEmergency = false);
    void patientCheckOut(const std::string& name);
};

#endif