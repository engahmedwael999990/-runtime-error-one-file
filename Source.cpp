#include<iostream>
#include"LinkedQueue.h"
#include"UpdatedQueue.h"
#include"priQueue.h"
#include<fstream>
#include<string>
#include<random>

using namespace std;

// Enums for Patient and Car types
enum patientType { Emergency, Special, Normal };
enum CarStatus { Ready, Assigned, Loaded };
enum cartype { Scars, Ncars };

// Patient Class
class Patient {
private:
    int patientID;
    int requestTime;
    int nearestHospitalID;
    int distanceToHospital;
    int severity;
    patientType type;

public:
    Patient(int id = 0, int RequestTime = 0, int NearestHospitalID = 0, int DistanceToHospital = 0, patientType Type = Normal)
        : patientID(id), requestTime(RequestTime), nearestHospitalID(NearestHospitalID), distanceToHospital(DistanceToHospital), type(Type), severity(0) {}

    patientType getType() { return type; }
    int getID() { return patientID; }
    int getRequestTime() { return requestTime; }
    int getNearestHospitalID() { return nearestHospitalID; }
    int getDistanceToHospital() { return distanceToHospital; }
    int getSeverity() { return severity; }
    void setSeverity(int s) { severity = s; }

    // Overload << operator
    friend ostream& operator<<(ostream& os, const Patient& patient) {
        os << "Patient ID: " << patient.patientID << ", Type: " << patient.type << ", Severity: " << patient.severity;
        return os;
    }
};

// Car Class
class Car {
private:
    int busyTime;
    int speed;
    Patient* assignedPatient;
    CarStatus status;
    cartype Type;

public:
    Car(cartype carType = Ncars, int carSpeed = 0)
        : Type(carType), speed(carSpeed), status(Ready), busyTime(0), assignedPatient(nullptr) {}

    cartype getType() { return Type; }
    int getSpeed() { return speed; }
    CarStatus getStatus() { return status; }
    int getBusyTime() { return busyTime; }
    Patient* getAssignedPatient() { return assignedPatient; }
    void setStatus(CarStatus newStatus) { status = newStatus; }

};

// Hospital Class
class Hospital {
private:
    int hospitalID;
    int numSCars, numNCars;
    int SCspeed, NCspeed;
    LinkedQueue<Patient> SPlist;
    priQueue<Patient> EPlist;
    UpdatedQueue<Patient> NPlist;
    LinkedQueue<Car> SCarsFree;
    LinkedQueue<Car> NCarsFree;

public:
    Hospital(int id = 0, int numOfSC = 0, int numOfNC = 0, int SCsp = 0, int NCsp = 0)
        : hospitalID(id), numSCars(numOfSC), numNCars(numOfNC), SCspeed(SCsp), NCspeed(NCsp) {
        for (int i = 0; i < numOfSC; i++) SCarsFree.enqueue(Car(Scars, SCsp));
        for (int i = 0; i < numOfNC; i++) NCarsFree.enqueue(Car(Ncars, NCsp));
    }

    bool enqueueSP(Patient p) { return SPlist.enqueue(p); }
    void enqueueEP(Patient p, int severity) { EPlist.enqueue(p, severity); }
    bool enqueueNP(Patient p) { return NPlist.enqueue(p); }

    void print() {
        cout << "Hospital #" << hospitalID << " Data:" << endl;
        cout << "Special Patients: " << SPlist.getcount() << endl;
        cout << "Emergency Patients: " << EPlist.getcount() << endl;
        cout << "Normal Patients: " << NPlist.getcount() << endl;
        cout << "Free SCars: " << SCarsFree.getcount() << ", Free NCars: " << NCarsFree.getcount() << endl;
        cout << "---------------------------------------------" << endl;
    }
};

// CancelRequest Class
class CancelRequest {
private:
    int pID;
    int cancelTime;

public:
    CancelRequest(int pid = 0, int ct = 0) : pID(pid), cancelTime(ct) {}
    int getPID() { return pID; }
    int getCancelTime() { return cancelTime; }
};

// Organizer Class
class Organizer {
private:
    int H; // Number of hospitals
    Hospital* HospList;
    int** DistanceMatrix;
    LinkedQueue<Patient> AllRequests;
    LinkedQueue<CancelRequest> CancelledRequests;

public:
    Organizer(int numHospitals)
        : H(numHospitals) {
        HospList = new Hospital[H];
        DistanceMatrix = new int* [H];
        for (int i = 0; i < H; i++) DistanceMatrix[i] = new int[H]();
    }

    ~Organizer() {
        for (int i = 0; i < H; i++) delete[] DistanceMatrix[i];
        delete[] DistanceMatrix;
        delete[] HospList;
    }

    void LoadData(string filename) {
        fstream file(filename, ios::in);
        if (!file.is_open()) {
            cerr << "Error: Cannot open file!" << endl;
            return;
        }

        file >> H;
        HospList = new Hospital[H];
        DistanceMatrix = new int* [H];
        for (int i = 0; i < H; i++) DistanceMatrix[i] = new int[H]();

        for (int i = 0; i < H; i++)
            for (int j = 0; j < H; j++)
                file >> DistanceMatrix[i][j];

        int SCnum, NCnum, SCsp, NCsp;
        for (int i = 0; i < H; i++) {
            file >> SCnum >> NCnum >> SCsp >> NCsp;
            HospList[i] = Hospital(i + 1, SCnum, NCnum, SCsp, NCsp);
        }

        int reqNum;
        file >> reqNum;
        for (int i = 0; i < reqNum; i++) {
            string type;
            int pid, rt, hid, dist, severity;
            file >> type >> rt >> pid >> hid >> dist;

            if (type == "NP")
                AllRequests.enqueue(Patient(pid, rt, hid, dist, Normal));
            else if (type == "SP")
                AllRequests.enqueue(Patient(pid, rt, hid, dist, Special));
            else if (type == "EP") {
                file >> severity;
                Patient p(pid, rt, hid, dist, Emergency);
                p.setSeverity(severity);
                AllRequests.enqueue(p);
            }
        }
        file.close();
    }

    void print() {
        for (int i = 0; i < H; i++) HospList[i].print();
    }
};

// UI Class
class UI {
private:
    Organizer organizer;

public:
    UI(Organizer org) : organizer(org) {}

    void Interactive_mode(int timestep) {
        cout << "Current Timestep: " << timestep << endl;
        organizer.print();
    }
};

// Main Function
int main() {
    Organizer organizer(3); // Replace 3 with the actual number of hospitals
    organizer.LoadData("InputFile.txt");

    cout << "Testing Hospital Print Function:" << endl;
    organizer.print();

    return 0;
}
