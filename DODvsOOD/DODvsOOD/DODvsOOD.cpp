// DODvsOOD.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <map>
#include <chrono>

struct Employee {
    int id;
    std::string name;
    int age;
    std::string department;
    double salary;
};

struct Employees {
    std::vector<int> id;
    std::vector<std::string> name;
    std::vector<int> age;
    std::vector<std::string> department;
    std::vector<double> salary;

    void Init(int size) {
        id.reserve(size);
        name.reserve(size);
        age.reserve(size);
        department.reserve(size);
        salary.reserve(size);
    }
};


std::vector<Employee> OODTaskA(std::vector<Employee>& employees);
void OODTaskB(std::vector<Employee>& employees);

std::vector<int>& DODTaskA(std::vector<int>& employeesId, std::vector<double>& employeesSalary);
void DODTaskB(std::vector<double>& employeesSalary);

std::vector<Employee> OODTaskA(std::vector<Employee>& employees)
{
    std::vector<Employee> taskResult;
    for (Employee employee : employees) {
        if (employee.salary > 50000.00) {
            taskResult.push_back(employee);
        }
    }
    return taskResult;
}

void OODTaskB(std::vector<Employee>& employees)
{
    std::mt19937 rng(std::time(nullptr));
    std::normal_distribution<> salary_rise_dist(1.05, 1.15);
    for (Employee employee : employees) {
        employee.salary *= salary_rise_dist(rng);
    }
}

std::vector<int>& DODTaskA(std::vector<int>& employeesId, std::vector<double>& employeesSalary) {
    std::vector<int> taskResult;
    for (int i = 0; i < employeesSalary.size(); ++i) {
        if (employeesSalary[i] > 50000.00) {
            taskResult.push_back(employeesId[i]);
        }
    }
    return taskResult;
}

void DODTaskB(std::vector<double>& employeesSalary)
{
    std::mt19937 rng(std::time(nullptr));
    std::normal_distribution<> salary_rise_dist(1.05, 1.15);
    for (int i = 0; employeesSalary.size(); ++i) {
        employeesSalary[i] *= salary_rise_dist(rng);
    }
}

// Générateur de données aléatoires pour 1000 employés
std::vector<Employee> createEmployeeDataOOD() {
    std::vector<Employee> employees;
    employees.reserve(1000);

    // Générateur de nombres aléatoires
    std::mt19937 rng(std::time(nullptr));

    // Listes de noms et prénoms pour la génération aléatoire
    std::vector<std::string> firstNames = {
        "Jean", "Marie", "Pierre", "Sophie", "Thomas", "Julie", "Nicolas", "Emma",
        "Lucas", "Léa", "Antoine", "Clara", "Hugo", "Chloé", "Louis", "Camille",
        "Gabriel", "Sarah", "Alexandre", "Laura", "Maxime", "Julia", "Paul", "Louise",
        "Arthur", "Alice", "Victor", "Manon", "Jules", "Eva", "Nathan", "Inès",
        "Adam", "Charlotte", "Raphaël", "Zoé", "Théo", "Lina", "Samuel", "Anna"
    };

    std::vector<std::string> lastNames = {
        "Martin", "Bernard", "Dubois", "Thomas", "Robert", "Richard", "Petit", "Durand",
        "Leroy", "Moreau", "Simon", "Laurent", "Lefebvre", "Michel", "Garcia", "David",
        "Bertrand", "Roux", "Vincent", "Fournier", "Morel", "Girard", "Andre", "Lefevre",
        "Mercier", "Dupont", "Lambert", "Bonnet", "Francois", "Martinez", "Legrand", "Garnier",
        "Faure", "Rousseau", "Blanc", "Guerin", "Muller", "Henry", "Roussel", "Nicolas"
    };

    std::vector<std::string> departments = {
        "Ressources Humaines", "Informatique", "Marketing", "Ventes", "Finance",
        "Direction", "Recherche et Développement", "Production", "Qualité",
        "Service Client", "Communication", "Juridique", "Comptabilité", "Logistique"
    };

    // Distributions pour les données aléatoires
    std::uniform_int_distribution<> age_dist(22, 65);
    std::uniform_int_distribution<> firstname_dist(0, firstNames.size() - 1);
    std::uniform_int_distribution<> lastname_dist(0, lastNames.size() - 1);
    std::uniform_int_distribution<> dept_dist(0, departments.size() - 1);
    std::normal_distribution<> salary_dist(50000, 15000); // Moyenne de 50000, écart-type de 15000

    // Génération des 1000 employés
    for (int i = 0; i < 1000; ++i) {
        Employee emp;
        emp.id = 1001 + i;
        emp.name = firstNames[firstname_dist(rng)] + " " + lastNames[lastname_dist(rng)];
        emp.age = age_dist(rng);
        emp.department = departments[dept_dist(rng)];

        // Assurer un salaire minimum de 35000 et maximum de 150000
        double salary = salary_dist(rng);
        emp.salary = std::max(35000.0, std::min(150000.0, salary));

        employees.push_back(emp);
    }

    return employees;
}

// Générateur de données aléatoires pour 1000 employés
Employees createEmployeeDataDOD() {
    Employees employees;
    employees.Init(1000);

    // Générateur de nombres aléatoires
    std::mt19937 rng(std::time(nullptr));

    // Listes de noms et prénoms pour la génération aléatoire
    std::vector<std::string> firstNames = {
        "Jean", "Marie", "Pierre", "Sophie", "Thomas", "Julie", "Nicolas", "Emma",
        "Lucas", "Léa", "Antoine", "Clara", "Hugo", "Chloé", "Louis", "Camille",
        "Gabriel", "Sarah", "Alexandre", "Laura", "Maxime", "Julia", "Paul", "Louise",
        "Arthur", "Alice", "Victor", "Manon", "Jules", "Eva", "Nathan", "Inès",
        "Adam", "Charlotte", "Raphaël", "Zoé", "Théo", "Lina", "Samuel", "Anna"
    };

    std::vector<std::string> lastNames = {
        "Martin", "Bernard", "Dubois", "Thomas", "Robert", "Richard", "Petit", "Durand",
        "Leroy", "Moreau", "Simon", "Laurent", "Lefebvre", "Michel", "Garcia", "David",
        "Bertrand", "Roux", "Vincent", "Fournier", "Morel", "Girard", "Andre", "Lefevre",
        "Mercier", "Dupont", "Lambert", "Bonnet", "Francois", "Martinez", "Legrand", "Garnier",
        "Faure", "Rousseau", "Blanc", "Guerin", "Muller", "Henry", "Roussel", "Nicolas"
    };

    std::vector<std::string> departments = {
        "Ressources Humaines", "Informatique", "Marketing", "Ventes", "Finance",
        "Direction", "Recherche et Développement", "Production", "Qualité",
        "Service Client", "Communication", "Juridique", "Comptabilité", "Logistique"
    };

    // Distributions pour les données aléatoires
    std::uniform_int_distribution<> age_dist(22, 65);
    std::uniform_int_distribution<> firstname_dist(0, firstNames.size() - 1);
    std::uniform_int_distribution<> lastname_dist(0, lastNames.size() - 1);
    std::uniform_int_distribution<> dept_dist(0, departments.size() - 1);
    std::normal_distribution<> salary_dist(50000, 15000); // Moyenne de 50000, écart-type de 15000

    // Génération des 1000 employés
    for (int i = 0; i < 1000; ++i) {
        employees.id.push_back(i);
        employees.name.push_back(firstNames[firstname_dist(rng)] + " " + lastNames[lastname_dist(rng)]);
        employees.age.push_back(age_dist(rng));
        employees.department.push_back(departments[dept_dist(rng)]);

        // Assurer un salaire minimum de 35000 et maximum de 150000
        double salary = salary_dist(rng);
        employees.salary.push_back(std::max(35000.0, std::min(150000.0, salary)));
    }

    return employees;
}

int main()
{
    //OOD
    std::vector<Employee> employeesStoreOOD = createEmployeeDataOOD();

    const auto startOOD = std::chrono::high_resolution_clock::now();

    OODTaskA(employeesStoreOOD);
    OODTaskB(employeesStoreOOD);
    OODTaskA(employeesStoreOOD);

    const auto endOOD = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsedOOD = endOOD - startOOD;
    std::cout << "Waited " << elapsedOOD << '\n';

    //DOD
    Employees employeesStoreDOD = createEmployeeDataDOD();

    const auto startDOD = std::chrono::high_resolution_clock::now();

    DODTaskA(employeesStoreDOD.id, employeesStoreDOD.salary);
    DODTaskB(employeesStoreDOD.salary);
    DODTaskA(employeesStoreDOD.id, employeesStoreDOD.salary);

    const auto endDOD = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsedDOD = endDOD - startDOD;
    std::cout << "Waited " << elapsedDOD << '\n';
}
