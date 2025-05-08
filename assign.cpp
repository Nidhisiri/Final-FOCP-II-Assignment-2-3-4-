#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
using namespace std;

// Forward declarations
class Course;
class Department;
class Professor;

// Exception Hierarchy
class UniversitySystemException:public exception{
protected:
    string message;
    time_t timestamp;
public:
    UniversitySystemException(const string& msg):message(msg){
        timestamp=time(nullptr);
    }
    const char* what() const noexcept override{
         return message.c_str(); 
    }
    string getTimestamp() const{
        return string(ctime(&timestamp));
    }
};

class EnrollmentException:public UniversitySystemException{
public:
    EnrollmentException(const string& msg):UniversitySystemException(msg){}
};

class GradeException:public UniversitySystemException{
public:
    GradeException(const string& msg):UniversitySystemException(msg){}
};

class PaymentException:public UniversitySystemException{
public:
    PaymentException(const string& msg):UniversitySystemException(msg){}
};

// Base Person class
class Person{
private:
    string name;
    int age;
    string id;
    string contactInfo;
protected:
    void validateName(const string& n){
        if (n.empty()) throw UniversitySystemException("Name cannot be empty");
    }
    void validateAge(int a){
        if (a<=0 || a>120) throw UniversitySystemException("Invalid age");
    }
    void validateId(const string& i){
        if (i.empty()) throw UniversitySystemException("ID cannot be empty");
    }
public:
    Person(const string& n, int a, const string& i, const string& c)
        : name(n), age(a), id(i), contactInfo(c) {
        validateName(n);
        validateAge(a);
        validateId(i);
    }
    virtual ~Person(){}
    
    // Getters
    string getName() const{
        return name;
    }
    int getAge() const{ 
        return age; 
    }
    string getId() const{ 
        return id; 
    }
    string getContactInfo() const{ 
        return contactInfo; 
    }
    
    // Setters
    void setName(const string& n){ 
        validateName(n); name=n;
    }
    void setAge(int a){ 
        validateAge(a); age=a; 
    }
    void setContactInfo(const string& c){ 
        contactInfo=c; 
    }
    
    virtual void displayDetails() const{
        cout<<"Person: "<<name<<", Age: "<<age<<", ID: "<<id<<endl;
    }
    virtual double calculatePayment() const{ 
        return 0.0; 
    }
};

// Student class
class Student:public Person{
private:
    string enrollmentDate;
    string program;
    double gpa;
protected:
    void validateGpa(double g){
        if (g<0.0 || g>4.0) throw GradeException("GPA must be between 0.0 and 4.0");
    }
public:
    Student(const string& n, int a, const string& i, const string& c,
            const string& e, const string& p, double g)
        : Person(n, a, i, c), enrollmentDate(e), program(p), gpa(g){
        validateGpa(g);
    }
    
    // Getters
    string getEnrollmentDate() const{ 
        return enrollmentDate; 
    }
    string getProgram() const{ 
        return program; 
    }
    double getGpa() const{ 
        return gpa; 
    }
    
    // Setters
    void setGpa(double g){ 
        validateGpa(g); gpa = g; 
    }
    
    void displayDetails() const override{
        Person::displayDetails();
        cout<<"Student - Program: "<<program<<", GPA: "<<gpa<<endl;
    }
    
    double calculatePayment() const override{
        return 5000.0; // Base tuition per semester
    }
};

// Derived Student classes
class UndergraduateStudent : public Student{
private:
    string major;
    string minor;
    string expectedGradDate;
public:
    UndergraduateStudent(const string& n, int a, const string& i, const string& c,
                        const string& e, const string& p, double g,
                        const string& maj, const string& min, const string& grad)
        : Student(n, a, i, c, e, p, g), major(maj), minor(min), expectedGradDate(grad){}
    
    void displayDetails() const override{
        Student::displayDetails();
        cout<<"Undergraduate - Major: "<<major<<", Minor: "<<minor<< ", Expected Graduation: "<<expectedGradDate<<endl;
    }
    
    double calculatePayment() const override{
        return Student::calculatePayment()+1000.0; // Additional fees
    }
};

class GraduateStudent:public Student{
private:
    string researchTopic;
    string advisor;
    string thesisTitle;
public:
    GraduateStudent(const string& n, int a, const string& i, const string& c,
                    const string& e, const string& p, double g,
                    const string& rt, const string& adv, const string& tt)
        : Student(n, a, i, c, e, p, g), researchTopic(rt), advisor(adv), thesisTitle(tt){}
    
    void displayDetails() const override {
        Student::displayDetails();
        cout << "Graduate - Research: " << researchTopic
             << ", Advisor: " << advisor << endl;
    }
    
    double calculatePayment() const override{
        return Student::calculatePayment()+2000.0; // Research fees
    }
};

// Professor class
class Professor : public Person{
private:
    string department;
    string specialization;
    string hireDate;
protected:
    int yearsOfService;
public:
    Professor(const string& n, int a, const string& i, const string& c,
              const string& d, const string& s, const string& h, int y)
        : Person(n, a, i, c), department(d), specialization(s), hireDate(h), yearsOfService(y){}
    
    // Getters
    string getDepartment() const{ 
        return department; 
    }
    string getSpecialization() const{ 
        return specialization; 
    }
    
    void displayDetails() const override{
        Person::displayDetails();
        cout<<"Professor - Department: "<<department<< ", Specialization: "<<specialization<<endl;
    }
    
    virtual double calculatePayment() const override{
        return 50000.0+(yearsOfService*1000.0);
    }
};

// Derived Professor classes
class AssistantProfessor : public Professor{
private:
    int publications;
public:
    AssistantProfessor(const string& n, int a, const string& i, const string& c,
                      const string& d, const string& s, const string& h, int y, int p)
        : Professor(n, a, i, c, d, s, h, y), publications(p){}
    
    void displayDetails() const override{
        Professor::displayDetails();
        cout<<"Assistant Professor - Publications: "<<publications<<endl;
    }
    
    double calculatePayment() const override{
        return Professor::calculatePayment()+(publications*500.0);
    }
};

class AssociateProfessor : public Professor{
private:
    int grants;
public:
    AssociateProfessor(const string& n, int a, const string& i, const string& c,
                      const string& d, const string& s, const string& h, int y, int g)
        : Professor(n, a, i, c, d, s, h, y), grants(g){}
    
    void displayDetails() const override{
        Professor::displayDetails();
        cout<<"Associate Professor - Grants: "<<grants<<endl;
    }
    
    double calculatePayment() const override{
        return Professor::calculatePayment()+(grants*1000.0);
    }
};

class FullProfessor : public Professor{
private:
    bool tenured;
public:
    FullProfessor(const string& n, int a, const string& i, const string& c,
                  const string& d, const string& s, const string& h, int y, bool t)
        : Professor(n, a, i, c, d, s, h, y), tenured(t){}
    
    void displayDetails() const override{
        Professor::displayDetails();
        cout<<"Full Professor - Tenured: "<<(tenured ? "Yes" : "No")<<endl;
    }
    
    double calculatePayment() const override{
        return Professor::calculatePayment()+(tenured ? 20000.0 : 0.0);
    }
};

// Course class
class Course{
private:
    string code;
    string title;
    int credits;
    string description;
    Professor* instructor;
protected:
    void validateCredits(int c){
        if(c<=0)throw UniversitySystemException("Credits must be positive");
    }
public:
    Course(const string& c, const string& t, int cr, const string& d)
        : code(c), title(t), credits(cr), description(d), instructor(nullptr){
        validateCredits(cr);
    }
    ~Course(){}
    
    // Getters
    string getCode() const{ 
        return code; 
    }
    string getTitle() const{ 
        return title; 
    }
    int getCredits() const{
        return credits; 
    }
    string getDescription() const{
         return description; 
        }
    
    void setInstructor(Professor* p){ 
        instructor = p; 
    }
    Professor* getInstructor() const {
        return instructor; 
    }
};

// Department class
class Department{
private:
    string name;
    string location;
    double budget;
    vector<Professor*> professors;
public:
    Department(const string& n, const string& l, double b)
        : name(n), location(l), budget(b){}
    ~Department() {}
    
    void addProfessor(Professor* p) { professors.push_back(p); }
    const vector<Professor*>& getProfessors() const{
         return professors;
        }
    
    string getName() const{ 
        return name; 
    }
};

// GradeBook class
class GradeBook{
private:
    Course* course;
    vector<pair<string, double>> grades; // student ID, grade
protected:
    void validateGrade(double g) {
        if(g<0.0 || g>100.0) throw GradeException("Grade must be between 0.0 and 100.0");
    }
public:
    GradeBook(Course* c):course(c){}
    
    void addGrade(const string& studentId, double grade){
        validateGrade(grade);
        grades.emplace_back(studentId, grade);
    }
    
    double calculateAverageGrade() const{
        if(grades.empty()) 
        return 0.0;
        double sum=0.0;
        for(const auto& g:grades) sum+=g.second;
        return sum/grades.size();
    }
    
    double getHighestGrade() const{
        if(grades.empty()) return 0.0;
        return max_element(grades.begin(), grades.end(),
            [](const auto& a, const auto& b){ 
                return a.second < b.second;})->second;
    }
    
    vector<string> getFailingStudents() const{
        vector<string> failing;
        for(const auto& g:grades){
            if(g.second < 60.0) failing.push_back(g.first);
        }
        return failing;
    }
};

// EnrollmentManager class
class EnrollmentManager{
private:
    vector<pair<string, Course*>> enrollments; // student ID, course
    const int MAX_ENROLLMENT=30;
public:
    void enrollStudent(const string& studentId, Course* course){
        int count = count_if(enrollments.begin(), enrollments.end(),
            [&course](const auto& e) {
                return e.second==course; });
        if(count >= MAX_ENROLLMENT)
            throw EnrollmentException("Course is full");
        enrollments.emplace_back(studentId, course);
    }
    
    void dropStudent(const string& studentId, Course* course){
        auto it = find_if(enrollments.begin(), enrollments.end(),
            [&studentId, &course](const auto& e){
                return e.first == studentId && e.second == course;
            });
        if(it != enrollments.end()) enrollments.erase(it);
    }
    
    int getEnrollmentCount(Course* course) const {
        return count_if(enrollments.begin(), enrollments.end(),
            [&course](const auto& e){ 
                return e.second==course; });
    }
};

// Classroom class
class Classroom{
private:
    string roomNumber;
    int capacity;
public:
    Classroom(const string& rn, int c) : roomNumber(rn), capacity(c) {}
    string getRoomNumber() const { return roomNumber; }
};

// Schedule class
class Schedule{
private:
    vector<tuple<Course*, Classroom*, string>> schedule; // Course, Classroom, Time slot
public:
    void addCourseSchedule(Course* c, Classroom* r, const string& time) {
        schedule.emplace_back(c, r, time);
    }
};

// University class
class University{
private:
    vector<Department*> departments;
    vector<Course*> courses;
    vector<Person*> people;
public:
    ~University() {
        for (auto d : departments) delete d;
        for (auto c : courses) delete c;
        for (auto p : people) delete p;
    }
    
    void addDepartment(Department* d) { departments.push_back(d); }
    void addCourse(Course* c) { courses.push_back(c); }
    void addPerson(Person* p) { people.push_back(p); }
    
    vector<Professor*> getAllProfessors() const{
        vector<Professor*> profs;
        for(const auto& d : departments){
            const auto& deptProfs = d->getProfessors();
            profs.insert(profs.end(), deptProfs.begin(), deptProfs.end());
        }
        return profs;
    }
};

// UniversitySystem class
class UniversitySystem{
private:
    University* university;
    EnrollmentManager* enrollmentManager;
    ofstream logFile;
    
    void logException(const UniversitySystemException& e){
        logFile<<"Exception at "<<e.getTimestamp()<<": "<<e.what()<<endl;
    }
public:
    UniversitySystem() : university(new University), enrollmentManager(new EnrollmentManager){
        logFile.open("university_errors.log", ios::app);
    }
    ~UniversitySystem(){
        delete university;
        delete enrollmentManager;
        logFile.close();
    }
    
    void addDepartment(const string& name, const string& location, double budget){
        university->addDepartment(new Department(name, location, budget));
    }
    
    void addCourse(const string& code, const string& title, int credits, const string& desc){
        university->addCourse(new Course(code, title, credits, desc));
    }
    
    void addPerson(Person* p){ 
        university->addPerson(p); 
    }
    
    void enrollStudent(const string& studentId, Course* course){
        try{
            enrollmentManager->enrollStudent(studentId, course);
        } catch (const EnrollmentException& e){
            logException(e);
            throw;
        }
    }
};

// Test program
int main(){
    try {
        UniversitySystem system;
        
        // Create departments
        system.addDepartment("Computer Science", "Building A", 1000000.0);
        
        // Create courses
        Course* cs101 = new Course("CS101", "Intro to Programming", 3, "Basic programming");
        Course* cs201 = new Course("CS201", "Data Structures", 3, "Advanced programming");
        system.addCourse(cs101->getCode(), cs101->getTitle(), cs101->getCredits(), cs101->getDescription());
        system.addCourse(cs201->getCode(), cs201->getTitle(), cs201->getCredits(), cs201->getDescription());
        
        // Create people
        UndergraduateStudent* undergrad = new UndergraduateStudent(
            "Raj Grover", 20, "S001", "raj@gmail.com",
            "2022-01-01", "Computer Science", 3.8,
            "CS", "Math", "2027-01-01"
        );
        
        GraduateStudent* grad = new GraduateStudent(
            "Kajal Sharma", 25, "S002", "kajal@gmail.com",
            "2024-01-01", "Computer Science", 3.6,
            "AI", "Dr. Jack", "ML Optimization"
        );
        
        FullProfessor* prof = new FullProfessor(
            "Dr.Jack ", 45, "P001", "jack@gmail.com",
            "Computer Science", "AI", "2018-01-01", 15, true
        );
        
        system.addPerson(undergrad);
        system.addPerson(grad);
        system.addPerson(prof);
        
        // Demonstrate polymorphism
        vector<Person*> people = {undergrad, grad, prof};
        cout << "\nPolymorphic Display:\n";
        for(const auto& p : people){
            p->displayDetails();
            cout<<"Payment: $"<<fixed<<setprecision(2)<<p->calculatePayment()<<endl;
        }
        
        // Enrollment
        system.enrollStudent("S001", cs101);
        system.enrollStudent("S002", cs101);
        
        // GradeBook
        GradeBook gb(cs101);
        gb.addGrade("S001", 96.0);
        gb.addGrade("S002", 90.0);
        
        cout<<"\nGrade Statistics:\n";
        cout<<"Average Grade: "<<gb.calculateAverageGrade()<<endl;
        cout<<"Highest Grade: "<<gb.getHighestGrade()<<endl;
        
        // Clean up
        delete cs101;
        delete cs201;
        
    } catch (const UniversitySystemException& e){
        cerr<<"Error: "<<e.what()<<endl;
    }
    return 0;
}