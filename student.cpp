#include <bits/stdc++.h>
using namespace std;
enum class Role
{
    ADMIN,
    STUDENT,
    STAFF
};
struct AuthUser
{
    string username;
    string password;
    Role role;
    string sid;
};

// Forward declaration
string simpleHash(const string&pass);

string simpleHash(const string&pass)
{
    int hash=0;
    for(char c:pass)
    {
        hash+=c;
    }
    return to_string(hash);
    
};
bool isValidAge(const string &s)
{
    if (s.empty())
        return false;
    for (char c : s)
        if (!isdigit(c))
            return false;

    int age = stoi(s);
    return age > 15 && age <= 40;
};
bool isValidName(const string &name)
{
    if (name.empty())
        return false;
    for (char c : name)
        if (!isalpha(c) && c != ' ')
            return false;
    return true;
};

class User
{
public:
    string username;
    Role role;
    
    string linkedStudentId; 
    User(string u, Role r, string sid = "")
        : username(u), role(r), linkedStudentId(sid) {}
};
class AuthManager
{
public:
    static User login()
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        ifstream file("users.txt");
        if (!file)
        {
            cout << "users.txt not found!\n";
            exit(0);
        }

        string uname, pass;
        cout << "Username: ";
        getline(cin, uname);
        cout << "Password: ";
        getline(cin, pass);

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string fUser, fPass, fRole, fSid;

            getline(ss, fUser, ',');
            getline(ss, fPass, ',');
            getline(ss, fRole, ',');
            getline(ss, fSid, ',');

            if (uname == fUser && simpleHash(pass)==fPass)
            {
                Role role;
                if (fRole == "ADMIN")
                    role = Role::ADMIN;
                else if (fRole == "STUDENT")
                    role = Role::STUDENT;
                else
                    role = Role::STAFF;

                cout << "Login successful!\n";
                return User(fUser, role, fSid);
            }
        }

        cout << "Invalid username or password\n";
        exit(0);
    }
};

class Student
{
private:
    string student_id; // immutable

public:
    string name;
    int age;
    string course;

    Student(string id, string n, int a, string c)
        : student_id(id), name(n), age(a), course(c) {}

    string getId() const
    {
        return student_id;
    }

    void display() const
    {
        cout << student_id << " | "
             << name << " | "
             << age << " | "
             << course << endl;
    }
};

/* ===================== FileManager (ONLY FILE I/O) ===================== */
class FileManager
{
public:
    static void save(const vector<Student> &students)
    {
        ofstream file("students.txt");
        if (!file)
            return;

        for (const auto &s : students)
        {
            file << s.getId() << ","
                 << s.name << ","
                 << s.age << ","
                 << s.course << "\n";
        }
    }

    static vector<Student> load()
    {
        vector<Student> students;
        ifstream file("students.txt");
        if (!file)
            return students;

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string id, name, agestr, course;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, agestr, ',');
            getline(ss, course, ',');

            students.emplace_back(id, name, stoi(agestr), course);
        }
        return students;
    }
};

/* ===================== StudentManager (ONLY LOGIC) ===================== */
class StudentManager
{
private:
    vector<Student> students;

    int findIndexById(const string &id) const
    {
        for (size_t i = 0; i < students.size(); i++)
        {
            if (students[i].getId() == id)
                return i;
        }
        return -1;
    }

public:
    StudentManager(const vector<Student> &data) : students(data) {}

    StudentManager()
    {
        students = FileManager::load();
    }

    void addStudent(const User &user, string id, string name, int age, string course)
    {

        if (user.role != Role::ADMIN)
        {
            cout << "Access denied";
            return;
        }
        if (findIndexById(id) != -1)
        {
            cout << "Student already exists\n";
            return;
        }

        students.emplace_back(id, name, age, course);
        sortById();
        FileManager::save(students);
    }

    void displayAll(const User &user) const
    {
        if (user.role == Role::ADMIN || user.role == Role::STAFF)
        {
            // Admin sees all
            for (const auto &s : students)
                s.display();
        }
        else if (user.role == Role::STUDENT)
        {
            // Student sees ONLY own data
            int idx = findIndexById(user.linkedStudentId);
            if (idx != -1)
                students[idx].display();
            else
                cout << "No record found\n";
        }
        else
        {
            cout << "Access restricted\n";
        }
    }
    void searchStudent(const User &user, const string &id) const
    {
        if (user.role == Role::ADMIN || user.role == Role::STAFF)
        {
            int idx = findIndexById(id);
            if (idx != -1)
                students[idx].display();
            else
                cout << "Student not found\n";
        }
        else if (user.role == Role::STUDENT)
        {
            if (user.linkedStudentId == id)
            {
                int idx = findIndexById(id);
                if (idx != -1)
                    students[idx].display();
                else
                    cout << "No record found\n";
            }
            else
            {
                cout << "Access denied (privacy protected)\n";
            }
        }
    }

    void updateName(const User &user, const string &id, const string &newName)
    {
        if (user.role != Role::ADMIN)
        {
            cout << "Access denied\n";
            return;
        }
        int idx = findIndexById(id);
        if (idx == -1)
            return;

        students[idx].name = newName;
        sortById();
        FileManager::save(students);
    }

    void updateAge(const User &user, string id, int newAge)
    {
        if (user.role != Role::ADMIN)
        {
            cout << "Access denied\n";
            return;
        }
        int idx = findIndexById(id);
        if (idx == -1)
            return;

        students[idx].age = newAge;
        FileManager::save(students);
    }

    void updateCourse(const User &user, string id, string newCourse)
    {
        if (user.role != Role::ADMIN)
        {
            cout << "Access denied\n";
            return;
        }
        int idx = findIndexById(id);
        if (idx == -1)
            return;

        students[idx].course = newCourse;
        FileManager::save(students);
    }

    void deleteStudent(const User &user, const string &id)
    {
        if (user.role != Role::ADMIN)
        {
            cout << "Access denied\n";
            return;
        }
        int idx = findIndexById(id);
        if (idx == -1)
            return;

        students.erase(students.begin() + idx);
        FileManager::save(students);
    }

    void sortById()
    {
        sort(students.begin(), students.end(),
             [](const Student &a, const Student &b)
             {
                 return a.getId() < b.getId();
             });
    }
};
void adminMenu(const User &user, StudentManager &sm)
{
    int choice;

    cout << "\n1. Add Student"
         << "\n2. Display All"
         << "\n3. Search"
         << "\n4. Update"
         << "\n5. Delete"
         << "\n6. Exit\n";
    cout << "Enter your choice\n";
    cin >> choice;
    cin.ignore(); // Ignore the newline after choice

    string id, name, course, ageStr;
    int age;

    switch (choice)
    {
    case 1:
        cout << "ID: ";
        getline(cin, id);
        if (id.empty())
        {
            cout << "ID cannot be empty\n";
            return;
        }

        cout << "Name: ";
        getline(cin, name);

        if (!isValidName(name))
        {
            cout << "Invalid name!\n";
            return;
        }

        cout << "Age: ";
        getline(cin, ageStr);

        if (!isValidAge(ageStr))
        {
            cout << "Invalid age!\n";
            return;
        }
        age = stoi(ageStr);

        age = stoi(ageStr);
        cout << "Course: ";
        getline(cin, course);
        sm.addStudent(user, id, name, age, course);
        break;

    case 2:
        sm.sortById();
        sm.displayAll(user);
        break;

    case 3:
        cout << "Enter ID: ";
        getline(cin, id);
        sm.searchStudent(user, id);
        break;

    case 4:
    {
        cout << "Enter ID: ";
        getline(cin, id);
        cout << "1.Name  2.Age  3.Course\n";
        cout << "Enter your choice: ";
        int c;
        cin >> c;
        cin.ignore(); // Ignore newline after c

        if (c == 1)
        {
            cout << "New Name: ";
            getline(cin, name);
            sm.updateName(user, id, name);
        }
        else if (c == 2)
        {
            cout << "New Age: ";
            getline(cin, ageStr);
            age = stoi(ageStr);
            sm.updateAge(user, id, age);
        }
        else if (c == 3)
        {
            cout << "New Course: ";
            getline(cin, course);
            sm.updateCourse(user, id, course);
        }
        else
            cout << "Invalid choice\n";
        break;
    }

    case 5:
        cout << "Enter ID: ";
        getline(cin, id);
        sm.deleteStudent(user, id);
        break;

    case 6:
        cout << "Exiting...\n";
        break;

    default:
        cout << "Invalid choice\n";
        break;
    }
};
void StudentMenu(const User &user, StudentManager &sm)
{
    int choice;
    cout << "\n1. View my Profile"
         << "\n2. Search my profile"
         << "\n3. Exit\n";
    cout << "Enter your choice\n";
    cin >> choice;
    cin.ignore();

    string id;

    switch (choice)
    {
    case 1:
        sm.displayAll(user);

        break;

    case 2:
        cout << "Enter ID: ";
        getline(cin, id);
        sm.searchStudent(user, id);
        break;

    case 3:
        cout << "Exiting...\n";
        break;

    default:
        cout << "Invalid choice\n";
        break;
    }
};

void staffMenu(const User &user, StudentManager &sm)
{
    int choice;
    cout << "\n1. View all Profile"
         << "\n2. Search student profile"
         << "\n3. Exit\n";
    cout << "Enter your choice\n";
    cin >> choice;
    cin.ignore(); // Ignore the newline after choice

    string id;
    switch (choice)
    {
    case 1:
        sm.displayAll(user);
        break;

    case 2:
        cout << "Enter ID: ";
        getline(cin, id);
        sm.searchStudent(user, id);

        break;

    case 3:
        cout << "Exiting...\n";
        break;

    default:
        cout << "Invalid choice\n";
        break;
    }
};
int main()
{
    StudentManager sm;

    User currentUser = AuthManager::login(); // login ONCE

    if (currentUser.role == Role::ADMIN)
        adminMenu(currentUser, sm);
    else if (currentUser.role == Role::STUDENT)
        StudentMenu(currentUser, sm);
    else if (currentUser.role == Role::STAFF)
        staffMenu(currentUser, sm);

    return 0;
}
