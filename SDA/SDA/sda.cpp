#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

void displayMainMenu() {
    cout << "\nComplaint Management System\n";
    cout << "1. Teacher\n";
    cout << "2. Manager\n";
    cout << "3. Employee\n";
    cout << "4. Administrator\n";
    cout << "5. Generate Reports\n";
    cout << "6. Exit\n";
}


class Main;
class Person;
class Teacher;
class Department;
class Complaint;
class State;
class Employee;
bool compareChronologicalOrder(const Complaint* a, const Complaint* b);
void teacherMenu(Main& main);
void employeeMenu(Main& main);

class Date
{
    int dd;
    int mm;
    int yy;
public:
    Date(int d, int m, int y)
    {
        dd = d;
        mm = m;
        yy = y;
    }
    void get_date(int& d, int& m, int& y)const
    {
        d = dd;
        m = mm;
        y = yy;
    }
    bool operator<(const Date& other) const 
    {
        // Compare years first
        if (yy <other.yy) {
            return true;
        }
        else if (yy > other.yy) {
            return false;
        }

        // If years are equal, compare months
        if (mm < other.mm) {
            return true;
        }
        else if (mm > other.mm) {
			return false;
		}

        // If months are equal, compare days
        return dd < other.dd;
    }
};
class Assignment
{
    Date date;
    Complaint* complaint;
    Employee* employee;
public:
    Assignment(int d, int m, int y, Complaint* c, Employee* e);
    string employeename();
    string getdate() const
    {
        int d, m, y;
        date.get_date(d, m, y);
        string date = to_string(y) + "-" + to_string(m) + "-" + to_string(d);
        return date;
    }
    string getstatus();
    void viewcomplaints();
};


class Person
{
    int id;
    string name;
public:
    Person(string name, int id) { this->name = name; this->id = id; }
    virtual ~Person() {}
    string getName() const { return name; }
    int getid() const { return id; }
};

class Teacher : public Person
{
    vector<Complaint*> complaints;
public:
    Teacher(string name,int id) :Person(name,id) {}

    void addComplaint(Complaint* complaint) {

        complaints.push_back(complaint);
        sort(complaints.begin(), complaints.end(), compareChronologicalOrder);
    }
    const vector<Complaint*>& getcomplaints()
    {
        return complaints;
    }
    void printComplaints() const;

};

class Employee : public Person
{
    Department* dept;
    vector<Assignment*> assignments;
public:
    Employee(string name, Department* d, int id) :Person(name, id) { dept = d; }
    Department* getDepartment() const { return dept; }
    void addassignment(Assignment* a)
    {
        assignments.push_back(a);
    }
    void viewcomplaints()
    {
        for (const auto& assignment : assignments)
        {
			assignment->viewcomplaints();
		}
	}
    void printComplaints(const string& status) const;

};

class Manager :public Person
{
    Department* dept;
public:
    Manager(string name, Department* d, int id) :Person(name, id) { dept = d; }
};



class Department
{
    vector<Employee*> emp;
    Manager* mgr;
    vector<Complaint*> complaints;
public:
    void setdept(int id,string name)
    {
        this->emp.push_back(new Employee(name, this,id));
    }
    void addComplaint(Complaint* complaint)
    {
        //yaad rakhna baad mai date ka comparison bhi dalna hai chronological order mai kerne ke liye 

        complaints.push_back(complaint);
        sort(complaints.begin(), complaints.end(), compareChronologicalOrder);
    }
    void sortcomplaints()
    {
		sort(complaints.begin(), complaints.end(), compareChronologicalOrder);
	}
    void setManager(int id,string name)
    {
        mgr = new Manager(name, this,id);
    }
    template<typename T>
    void setcomplaint(State* stateInstance, string date, string desc, string filedby)
    {
        this->complaints.push_back(new T(stateInstance, date, desc, filedby, this));
        sortcomplaints();
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
    virtual string getdeptname() const = 0;
    Department* getdept()
    {
        return this;
    }
    void printComplaints(const string& status) const;
    void addassignment(int id, vector<int>e, int d, int m, int y);
    virtual ~Department()
    {
        delete mgr;
        for (auto& employee : emp) {
            delete employee;
        }
    }
    bool checkid(int id);
    void setState(int id, State* state);

 
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
    string getdeptname() const
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
    string getdeptname() const
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
    string getdeptname() const
    {
        return "Accounts";
    }
};


class State {
public:
    virtual string handle(Complaint& context) = 0;
    virtual string getStatus() const = 0;
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

    string handle(Complaint& context) override {
        return "Complaint is open";
    }

    string getStatus() const override {
        return "New";
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

    string handle(Complaint& context) override {
        return "Complaint is in progress";
    }

    string getStatus() const override {
        return "Assigned";
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

    string handle(Complaint& context) override {
        return "Complaint is resolved";
    }

    string getStatus() const override {
        return "Resolved";
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

    string handle(Complaint& context) override {
        return "Complaint is closed";
    }

    string getStatus() const override {
        return "Closed";
    }
};

Closed* Closed::instance = nullptr;
class Complaint
{
    vector<Assignment*>assignments;
    State* currentState;
    Department* dept;
    Teacher* teacher;
    Date date;
    string description;
    int id;
public:
    Complaint(int id,int d, int m, int y, State* state, string desc, Department* dept, Teacher* teach) :date(d, m, y)
    {
        this->id = id;
        currentState = state;
        description = desc;
        this->dept = dept;
        teacher = teach;
        this->dept->addComplaint(this);
        this->teacher->addComplaint(this);
    }

    void setid(int id)
    {
		this->id = id;
	}
    void setState(State* state) {
        delete currentState;
        currentState = state;
    }

    string processComplaint() const {
        return currentState->handle(const_cast<Complaint&>(*this));
    }
    string getdate() const
    {
        int d, m, y;
        date.get_date(d, m, y);
        string date = to_string(y) + "-" + to_string(m) + "-" + to_string(d);
        return date;
    }
    const Date& getDate() const
    {
		return date;
	}

    const State* getState() const {
        return currentState;
    }

    string getStatus() const {
        return currentState->getStatus();
    }

    string getdesc()
    {
        return description;
    }
    void printComplaint() const
    {
        int d, m, y;
        cout << endl << "\n\nComplaint Details:" << endl;
        cout << "Date: " << getdate() << endl;
        cout << "Description: " << description << endl;
        cout<< "Status: " << currentState->getStatus() << endl;
        cout << "Department: " << dept->getdeptname() << endl;
        cout << "Teacher: " << teacher->getName()<<endl;
        if (!assignments.empty())
        {
            Assignment* firstAssignment = assignments.front();
			cout << "Assigned to:" << endl;
            for (const auto& assignment : assignments) {

				cout << assignment->employeename() << endl;
			}
			cout << "assigned Date :" << firstAssignment->getdate();
		}
        else
        {
			cout << "Not assigned to anyone yet" << endl;
		}

    }
    void addassignment(Assignment* a)
    {
        assignments.push_back(a);
    }
    int getid()
    {
		return id;
	}
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
            addteacher(line);
        }

        file.close();
    }
    void addteacher(string& line)
    {
        stringstream ss(line);
        string id;
        string name;
        getline(ss, id, ',');
        getline(ss, name);
        int id1 = stoi(id);
        teachers.push_back(new Teacher(name,id1));
    }

    void loadFromFile_e(const string& filename, int i) {
        ifstream file(filename);
        string line;

        while (getline(file, line))
        {
            addemployee(line, i);
        }

        file.close();
    }
    void addemployee(string& line, int i)
    {

        stringstream ss(line);
        string id;
        string name;
        getline(ss, id, ',');
        getline(ss, name);
        int id1 = stoi(id);
        dept[i]->setdept(id1,name);
    }
    void loadFromFile_m(const string& filename) {
        ifstream file(filename);
        string line;
        int i = 0;
        while (getline(file, line) && i < 3) {
            addmanager(line, i);
            i++;
        }

        file.close();
    }
    void addmanager(string& line, int i)
    {
        stringstream ss(line);
        string id;
        string name;
        getline(ss, id, ',');
        getline(ss, name);

        dept[i]->setManager(stoi(id),name);
    }
    void loadFromFile_c(string filename, int i)
    {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            string date, month, year, description, filedBy, status, id;
            string employeeList;
            string ayear, amonth, adate;
            stringstream ss(line);
            if (getline(ss, id, ';') &&
                getline(ss, year, '-') &&
                getline(ss, month, '-') &&
                getline(ss, date, ';') &&
                getline(ss, status, ';')) { }
            int d, m, y;
            d = stoi(date);
            m = stoi(month);
            y = stoi(year);
            if(status=="New")
            {
                if (getline(ss, description, ';') &&
                    getline(ss, filedBy)) {
				}
				
				addcomplaint(stoi(id), d, m, y, status, description, filedBy, i);
            }
            else
            {
                if (getline(ss, description, ';') &&
                    getline(ss, filedBy, ':') &&
                    getline(ss, employeeList, ';') &&
                    getline(ss, ayear, '-') &&
                    getline(ss, amonth, '-') &&
                    getline(ss, adate, ';')) { }
                addcomplaint(stoi(id), d, m, y, status, description, filedBy, i);
                addassignment(stoi(id), employeeList, stoi(adate), stoi(amonth), stoi(ayear), i);
            }

        }

        file.close();
    }
    void addcomplaint(int id,int d,int m,int y,string status,string description,string filedBy,int i)
    {
        
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
        else {
			cout << "Invalid state: " << status << endl;
		}

        for (const auto& teacher : teachers) {
            if (teacher->getid() == stoi(filedBy))
            {
                if (stateInstance != nullptr)
                {
                    new Complaint(id,d, m, y, stateInstance, description, dept[i], teacher);
                    break;
                }
            }
        }
    }
    void addassignment( int id,string employeeList, int d, int m, int y, int i)
    {
		vector<int> employeeIds = split(employeeList, ',');
		dept[i]->addassignment(id,employeeIds, d, m, y);
	}
    void displayTeachers() {
        for (const auto& teacher : teachers) {
            cout << teacher->getName() << endl;
        }
    }
    bool teacher(int id,string &name)
    {
        for (const auto& teacher : teachers) {
            if (teacher->getid() == id)
            {
                name=teacher->getName();
				return true;
			}
		}
        
		return false;
	}
    void printteachercomplaints() {
        for (const auto& teacher : teachers) {
            teacher->printComplaints();
        }
    }
    void teachercomplaints(int id)
    {
        for (const auto& teacher : teachers) {
            if (teacher->getid() == id)
            {
                teacher->printComplaints();
            }
        }
    }
    void displayEmployees(int i) {
        dept[i]->printEmployees();
    }
    bool checkemployee(int id, string& name, int i)
    {
        for (const auto& employee : dept[i]->getEmployees()) {
            if (employee->getid() == id)
            {
				name = employee->getName();
                return true;
            }
        }
        return false;
    }
    void displayManager(int i) {
        dept[i]->printManager();
    }
    void displaycomplaints(int i,string status) {
		dept[i]->printComplaints(status);
	}
    void displaydeptComplaints(int i) {
        dept[i]->printComplaints();
        cout << endl << "--------------------------------------------------------------------------------" << endl;

    }
    vector<int> split(const string& str, char delimiter) {
        vector<int> tokens;
        string token;
        stringstream ss(str);

        while (getline(ss, token, delimiter)) {
            tokens.push_back(stoi(token));
        }
        return tokens;
    }

    int generateid(int i)
    {
        int id;
        do{
                // Seed the random number generator with the current time
                std::srand(static_cast<unsigned int>(std::time(0)));

                // Generate and return a random 4-digit ID
                return 1000 + std::rand() % 9000; // Generates a random number between 1000 and 9999
            
		} while (dept[i]->checkid(id));
        return id;
    }
    void viewemployeecomplaints(int id,int i)
    {
        for (const auto& employee : dept[i]->getEmployees()) {
			if (employee->getid() == id)
			{ 
                employee->viewcomplaints();
            }
        }
    }
    void printemployeestatus(int id, int i, string status)
    {
        for (const auto& employee : dept[i]->getEmployees()) {
            if (employee->getid() == id)
            {
				employee->printComplaints(status);
			}
		}
	}
    bool setState(int id, State* state,int i)
    {
        for (const auto& complaint : dept[i]->getComplaints())
        {
			if (complaint->getid() == id)
			{ 
                complaint->setState(state);
				return true;
            }
        }
        return false;
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

    int choice;

    do {
        displayMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            teacherMenu(app);
        }
        else if (choice == 2) {
            //managerMenu();
        }
        else if (choice == 3) {
            employeeMenu(app);
        }
        else if (choice == 4) {
            // adminMenu();
        }
        else if (choice == 5) {
            // Generate reports
        }
        else if (choice==6)
        {
            cout << "Exiting the system. Goodbye!\n";
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 6);
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

void Teacher::printComplaints() const
{
	cout << "\n\n\nTeacher: " << getName() << endl;
    for (const auto& complaint : complaints)
    {
		complaint->printComplaint();
	}
}

Assignment::Assignment(int d, int m, int y, Complaint* c, Employee* e) :date(d, m, y)
{
    complaint = c;
    employee = e;
    complaint->addassignment(this);
    employee->addassignment(this);
}

string Assignment::employeename()
{
    return employee->getName();
}

bool compareChronologicalOrder(const Complaint* a, const Complaint* b) {
    return a->getDate() < b->getDate();
}

void Department::printComplaints(const string& status) const {
	cout << getdeptname() << " Department Complaints:" << endl;
    for (const auto& complaint : getComplaints()) {
        if (complaint->getStatus() == status) {
			complaint->printComplaint();
		}
	}
}

void teacherMenu(Main& main) {
    int choice;
        string id,name;
        cout << "Enter ID: ";
        cin.ignore();
        getline(cin, id);
        if (main.teacher(stoi(id),name))
        {
            cout << "\n\nWelcome " << name << endl;
          do {  
              // Display teacher menu options
            cout << "\nTeacher Menu\n";
            cout << "1. File a Complaint\n";
            cout << "2. View Complaints\n";
            cout << "3. Record Feedback\n";
            cout << "4. Go Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1) {
                int dept;
                cout << "Enter Department:\n1.IT\n2.Admin\n3.Accounts\n";
                cin >> dept;
                int year, month, date;
                string description;
                cout << "Enter date in the format yyyy-mm-dd: ";
                cin >> year >> month >> date;
                cout << "Enter Description: ";
                cin.ignore();
                getline(cin, description);
                int complaint_id=main.generateid(dept-1);
                cout << "Your complaint id is: " << complaint_id << endl;
                main.addcomplaint(complaint_id,date, month, year, "New", description, id, dept - 1);

            }
            else if (choice == 2) {
                main.teachercomplaints(stoi(id));
            }
            else if (choice == 3) {
                // Record feedback
            }
            else if (choice == 4) {
                cout << "Returning to the main menu.\n";
            }
            else {
                cout << "Invalid choice. Please try again.\n";
            }
          } while (choice != 4);
        }
        else
        {
            cout << "Invalid id, Please enter again\n";
        }

    
}

void Department::addassignment(int id, vector<int>e, int d, int m, int y)
{

    // Ensure there is at least one complaint to assign to
    if (!complaints.empty()) 
    {
        for (const auto& complaint : getComplaints())
        {
            if (complaint->getid() == id)
            {
                // Filter the provided employee names list to include only those who are in the specific department
                for (const auto& employeeid : e) {
                    for (const auto& emp : getEmployees()) {
                        if (emp->getid() == employeeid) {
                            new Assignment(d, m, y, complaint, emp);
                            break; // Break the inner loop once the employee is found
                        }
                    }
                }

                break; // Break the outer loop once the complaint is found
            }
        }
    }
}
bool Department::checkid(int id)
{
    for (const auto& complaint : getComplaints())
    {
        if (complaint->getid() == id)
        {
			return true;
		}
	}
	return false;
}
void Assignment::viewcomplaints()
{
	complaint->printComplaint();
}
void Employee::printComplaints(const string& status) const {
	//cout << "Employee: " << getName() << endl;
    for (const auto& assignment : assignments) {
        if (assignment->getstatus() == status) {
			assignment->viewcomplaints();
		}
	}
}
string Assignment::getstatus()
{
	return complaint->getStatus();
}

void Department::setState(int id, State* state)
{
    for (const auto& complaint : getComplaints())
    {
        if (complaint->getid() == id)
        {
			complaint->setState(state);
			break;
		}
	}
}
void employeeMenu(Main& main) {
    int choice;
    int dept;
    cout << "Enter Department:\n1.IT\n2.Admin\n3.Accounts\n";
    cin >> dept;
    int cid,in;
    string id, name;
    cout << "Enter ID: ";
    cin.ignore();
    getline(cin, id);
    if(main.checkemployee(stoi(id), name, dept - 1))
    {
        
        cout << "\n\nWelcome " << name << endl;
        do {
            // Display employee menu options
            std::cout << "\nEmployee Menu\n";
            std::cout << "1. View Assigned Jobs\n";
            std::cout << "2. Update Job Completion\n";
            std::cout << "3. Go Back to Main Menu\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (choice == 1) {
                cout<<"Assigned Jobs:\n";
                main.printemployeestatus(stoi(id), dept - 1,"Assigned");
                cout<<"\nResolved Jobs:\n";
                main.printemployeestatus(stoi(id), dept - 1, "Resolved");
            }
            else if (choice == 2) {
                // Update job completion
                main.printemployeestatus(stoi(id), dept - 1, "Assigned");
                cout<< "Enter complaint id: ";
                cin >> cid;
                cout<<"Do you want to mark this job as done?\n1.Yes\n2.No\n";
                cin>>in;
                cout<<"Are you sure?\n1.Yes\n2.No\n";
                cin >> in;
                if (in == 1)
                {
					if(main.setState(cid, Resolved::getInstance(),dept-1))
                        cout << "Job marked as done\n";
                    else
                        cout<< "An error occured\n";
                }
            }
            else if (choice == 3) {
                cout << "Returning to the main menu.\n";
            }
            else {
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 3);

    } 
    else
    {
		cout << "Invalid id, Please enter again\n";
	}
}