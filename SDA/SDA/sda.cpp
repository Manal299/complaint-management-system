#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include<sstream>
using namespace std;

class Person;
class Teacher;
class Department;
class Complaint;
class State;

class Assignment
{
    string date;
    Complaint* complaint;
public:
};


class Person
{
    string name;
public:
    Person(string name) { this->name = name; }
    virtual ~Person() {}
    string getName() const { return name; }
};

class Teacher : public Person
{
    vector<Complaint*> complaints;
public:
    Teacher(string name) :Person(name) {}
    
    void addComplaint(Complaint* complaint) {
        complaints.push_back(complaint);
    }
    const vector<Complaint*>& getcomplaints()
    {

        return complaints;
    }

};

class Employee : public Person
{
    Department* dept;
public:
    Employee(string name, Department* d) :Person(name) { dept = d; }
    Department* getDepartment() const { return dept; }
};

class Manager :public Person
{
    Department* dept;
public:
    Manager(string name, Department* d) :Person(name) { dept = d; }
};



class Department
{
    vector<Employee*> emp;
    Manager* mgr;
    vector<Complaint*> complaints;
public:
    template<typename T>
    void setdept(string name)
    {
        this->emp.push_back(new T(name, this));
    }
    void addComplaint(Complaint* complaint)
    {
        complaints.push_back(complaint);
    }
    void setManager(string name)
    {
        mgr = new Manager(name, this);
    }
    template<typename T>
    void setcomplaint(State* stateInstance, string date, string desc, string filedby)
    {
        this->complaints.push_back(new T(stateInstance, date, desc, filedby, this));
    }
    const vector<Employee*>& getEmployees() const
    {
        return emp;
    }

    Manager* getManager() const
    {
        return mgr;
    }
    const vector<Complaint*>& getComplaints() const
    {

        return complaints;
    }
    virtual void printEmployees() const {}
    virtual void printManager() const {}
    virtual void printComplaints()const {}
    virtual string getdeptname() =0;
    Department* getdept()
    {
        return this;
    }
    virtual ~Department()
    {
        delete mgr;
        for (auto& employee : emp) {
            delete employee;
        }
    }
};

class IT : public Department {
public:
    void printEmployees() const
    {
        cout << "IT Department Employees:" << endl;
        for (const auto& employee : getEmployees())
        {
            cout << employee->getName() << endl;
        }
    }

    void printManager() const
    {
        if (getManager()) {
            cout << "IT Department Manager: " << getManager()->getName() << endl;
        }
        else {
            cout << "IT Department has no manager." << endl;
        }
    }
    void printComplaints() const;
    string getdeptname() 
    {
        return "IT";
    }
};

class Admin : public Department
{
public:
    void printEmployees() const
    {
        cout << "Admin Department Complaints:" << endl;
        for (const auto& employee : getEmployees()) {
            cout << employee->getName() << endl;
        }
    }

    void printManager() const
    {
        if (getManager()) {
            cout << "Admin Department Manager: " << getManager()->getName() << endl;
        }
        else {
            cout << "Admin Department has no manager." << endl;
        }
    }
    void printComplaints() const;
    string getdeptname()
    {
        return "Admin";
    }
};

class Accounts : public Department {
public:
    void printEmployees() const
    {
        cout << "Accounts Department Employees:" << endl;
        for (const auto& employee : getEmployees()) {
            cout << employee->getName() << endl;
        }
    }

    void printManager() const
    {
        if (getManager()) {
            cout << "Accounts Department Manager: " << getManager()->getName() << endl;
        }
        else {
            cout << "Accounts Department has no manager." << endl;
        }
    }

    void printComplaints() const;
    string getdeptname()
    {
        return "Accounts";
    }
};


class State {
public:
    virtual void handle() = 0;
    virtual ~State() {}
};

// Singleton New class
class New : public State {

    static New* instance;
    New() {} // Private constructor

public:
    static New* getInstance() {
        if (!instance)
            instance = new New();
        return instance;
    }

    void handle() {
        cout << "New State" << endl;
    }
};

New* New::instance = nullptr;

// Singleton Assigned class
class Assigned : public State {

    static Assigned* instance;
    Assigned() {}

public:
    static Assigned* getInstance() {
        if (!instance)
            instance = new Assigned();
        return instance;
    }

    void handle() {
        cout << "Assigned State" << endl;
    }
};

Assigned* Assigned::instance = nullptr;

// Singleton Resolved class
class Resolved : public State {

    static Resolved* instance;
    Resolved() {}

public:
    static Resolved* getInstance() {
        if (!instance)
            instance = new Resolved();
        return instance;
    }

    void handle() {
        cout << "Resolved State" << endl;
    }
};

Resolved* Resolved::instance = nullptr;

// Singleton Closed class
class Closed : public State {

    static Closed* instance;
    Closed() {}

public:
    static Closed* getInstance() {
        if (!instance)
            instance = new Closed();
        return instance;
    }

    void handle() {
        cout << "Closed State" << endl;
    }
};

Closed* Closed::instance = nullptr;
class Complaint
{
    State* currentState;
    Department* dept;
    Teacher* teacher;
    string date;
    string description;
public:
    Complaint(State* state, string d, string desc, Department* dept,Teacher*teach) 
    {
        currentState=state;
        date=d;
        description=desc;
        this->dept = dept;
        teacher = teach;
        this->dept->addComplaint(this);
        this->teacher->addComplaint(this);
    }

    void setState(State* state) {
        currentState = state;
    }

    void handleState() {
        if (currentState) {
            currentState->handle();
        }
    }
    string getdate()
    {
        return date;
    }
    State* getstatus()
    {
        return currentState;
    }
    string getdesc()
    {
        return description;
    }
    void printComplaint() const
    {
        cout << endl<<"Complaint Details:" << endl;
        cout << "Date: " << date << endl;
        cout << "Description: " << description << endl;
        cout << dept->getdeptname() << endl;
        cout << teacher->getName();
    }


    //string getfiledby()
    //{
    //    return filedby;
    //}
    /*Department* getdept()
    {
        return dept;
    }*/
    ~Complaint() {
        // currentState does not need to be deleted since it's managed by singleton classes
    }
};



class Main {
    vector<Teacher*> teachers;
    vector<Department*> dept;
public:
    Main() {
        // Initialize vectors
        dept.resize(3);  // 3 categories: IT,Admin,Accounts
        dept[0] = new IT();
        dept[1] = new Admin();
        dept[2] = new Accounts();
    }

    void loadData() {
        loadFromFile_t("teachers.txt");
        loadFromFile_e("IT.txt", 0);
        loadFromFile_e("Admin.txt", 1);
        loadFromFile_e("Accounts.txt", 2);
        loadFromFile_m("managers.txt");
        loadFromFile_c("ITcomplaints.txt", 0);
        loadFromFile_c("Admincomplaints.txt", 1);
        loadFromFile_c("Accountscomplaints.txt", 2);
    }

    void loadFromFile_t(const string& filename) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            teachers.push_back(new Teacher(line));
        }

        file.close();
    }

    void loadFromFile_e(const string& filename, int i) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            dept[i]->setdept<Employee>(line);
        }

        file.close();
    }
    void loadFromFile_m(const string& filename) {
        ifstream file(filename);
        string line;
        int i = 0;
        while (getline(file, line) && i < 3) {
            dept[i]->setManager(line);
            i++;
        }

        file.close();
    }
    void loadFromFile_c(string filename, int i)
    {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            
            addcomplaint(line,i);

        }

        file.close();
    }
    void addcomplaint(string &line,int i)
    {
        string date, description, filedBy, status;
        stringstream ss(line);
        if (getline(ss, date, ';') &&
            getline(ss, status, ';') &&
            getline(ss, description, ';') &&
            getline(ss, filedBy, ':')) {
        }
        State* stateInstance = nullptr;
        // Determine the correct State singleton instance based on stateStr
        if (status == "New") {
            stateInstance = New::getInstance();
        }
        else if (status == "Assigned") {
            stateInstance = Assigned::getInstance();
        }
        else if (status == "Resolved") {
            stateInstance = Resolved::getInstance();
        }
        else if (status == "Closed") {
            stateInstance = Closed::getInstance();
        }

        for (const auto& teacher : teachers) {
            if (teacher->getName() == filedBy) {
                /*Complaint* newComplaint = new Complaint(stateInstance, date, description, filedBy, dept[i]);
                teacher->addComplaint<Complaint>(newComplaint);*/
                if (stateInstance != nullptr)
                {
                    Complaint* newc = new Complaint(stateInstance, date, description, dept[i], teacher);
                    break;
                }
            }
        }
    }
    void displayTeachers() {
        for (const auto& teacher : teachers) {
            cout << teacher->getName() << endl;
        }
    }
    void printteachercomplaints() {
        for (const auto& teacher : teachers) {
            cout << "Teacher: " << teacher->getName() << endl;
            for (const auto& complaint : teacher->getcomplaints()) {
                cout << "Complaint Date: " << complaint->getdate() << ", Description: " << complaint->getdesc() << endl;
            }
        }
    }
    void displayEmployees(int i) {
        dept[i]->printEmployees();
    }

    void displayManager(int i) {
        dept[i]->printManager();
    }
    void displaydeptComplaints(int i) {
        //if (i == 0)
        //{
        //    cout << "complaints of IT department are : " << endl;
        //    for (const auto& complaint : dept[i]->getComplaints())
        //    {
        //        cout << "Complaint Date: " << complaint->getdate() << endl;
        //        cout << "Description: " << complaint->getdesc() << endl;
        //        cout << "Status: ";
        //        complaint->getstatus()->handle();
        //        //cout << "filed by : " << complaint->getfiledby() << endl;
        //    }
        //}
        //else if (i == 1)
        //{
        //    cout << "complaints of admin department are : " << endl;
        //    for (const auto& complaint : dept[i]->getComplaints())
        //    {
        //        cout << "Complaint Date: " << complaint->getdate() << endl;
        //        cout << "Description: " << complaint->getdesc() << endl;
        //        cout << "Status: ";
        //        complaint->getstatus()->handle();
        //        //cout << "filed by : " << complaint->getfiledby() << endl;
        //    }
        //}
        //else
        //{
        //    cout << "complaints of accounts department are : " << endl;
        //    for (const auto& complaint : dept[i]->getComplaints())
        //    {
        //        cout << "Complaint Date: " << complaint->getdate() << endl;
        //        cout << "Description: " << complaint->getdesc() << endl;
        //        cout << "Status: ";
        //        complaint->getstatus()->handle();
        //        //cout << "filed by : " << complaint->getfiledby() << endl;
        //    }
        //}
        dept[i]->printComplaints();
        cout << endl<<"--------------------------------------------------------------------------------" << endl;

    }

    ~Main()
    {
        for (auto& teacher : teachers)
        {
            delete teacher;
        }

        for (auto& d : dept)
        {
            delete d;
        }
    }
};

int main()
{
    Main app;

    app.loadData();
    cout << "Teachers" << endl;
    app.displayTeachers();
    app.displayManager(0);
    app.displayEmployees(0);
    app.displayManager(1);
    app.displayEmployees(1);
    app.displayManager(2);
    app.displayEmployees(2);
    app.displaydeptComplaints(0);
    app.displaydeptComplaints(1);
    app.displaydeptComplaints(2);
    app.printteachercomplaints();
    /*cout << "Employees" << endl;
    app.displayPersons(1);
    cout << "Managers" << endl;
    app.displayPersons(2);*/
    //Complaint complaint(New::getInstance()); // Initial state set to New

    //complaint.handleState(); // Handle New state

    //Complaint complaint1(New::getInstance()); // Initial state set to New

    //complaint1.handleState(); // Handle New state

    //complaint.setState(Assigned::getInstance()); // Change state to Assigned
    //complaint.handleState(); // Handle Assigned state
    return 0;
}

void IT::printComplaints() const
{
    cout << "IT Department Complaints:" << endl;
    for (const auto& complaint : getComplaints())
    {
        complaint->printComplaint();
    }
}

void Admin::printComplaints() const
{
    cout << "Admin Department Complaints:" << endl;
    for (const auto& complaint : getComplaints())
    {
        complaint->printComplaint();
    }
}
void Accounts::printComplaints() const
{
    cout << "Accounts Department Complaints:" << endl;
    for (const auto& complaint : getComplaints())
    {
        complaint->printComplaint();
    }
}
