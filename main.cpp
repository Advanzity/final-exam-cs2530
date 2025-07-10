// Full Name: David Zakri
// Class: ITCS 2530
// GitHub Repo URL: https://github.com/Advanzity/final-exam-cs2530
// One-Sentence Summary: A console-based project management system that allows users to add, view, filter, sort, complete, save, and load projects with priority and due date tracking.
// Your posted hobby or interest: PROJECT MANAGEMENT
// AI Tool(s) Used: ChatGPT

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

const int MAX_PROJECTS = 100;

enum class Status { NotStarted = 0, InProgress = 1, Completed = 2 };

int dateToInt(const std::string& d) {
  return std::stoi(d.substr(0, 4) + d.substr(5, 2) + d.substr(8, 2));
}

struct Date {
  std::string iso;
};

class Project {
 private:
  std::string name;
  std::string type;
  Date due;
  int priority;
  Status status;
  std::string description;

 public:
  Project() : priority(1), status(Status::NotStarted) {}

  void input() {
    std::cin.ignore();
    std::cout << "Name: ";
    std::getline(std::cin, name);
    std::cout << "Type: ";
    std::getline(std::cin, type);
    std::cout << "Due date (YYYY-MM-DD): ";
    std::getline(std::cin, due.iso);
    std::cout << "Priority (1-5): ";
    std::cin >> priority;
    std::cin.ignore();
    std::cout << "Description: ";
    std::getline(std::cin, description);
    status = Status::NotStarted;
  }

  void display(int idx) const {
    std::cout << std::left << std::setw(3) << idx << std::setw(20) << name
              << std::setw(12) << type << std::setw(12) << due.iso
              << std::setw(3) << priority << std::setw(12);
    switch (status) {
      case Status::NotStarted:
        std::cout << "Not Started";
        break;
      case Status::InProgress:
        std::cout << "In Progress";
        break;
      case Status::Completed:
        std::cout << "Completed";
        break;
    }
    std::cout << "  " << description << '\n';
  }

  std::string getType() const { return type; }
  int getDate() const { return dateToInt(due.iso); }
  int getPrio() const { return priority; }
  Status getStatus() const { return status; }

  void markCompleted() { status = Status::Completed; }

  void write(std::ofstream& out) const {
    out << name << '\n'
        << type << '\n'
        << due.iso << '\n'
        << priority << '\n'
        << static_cast<int>(status) << '\n'
        << description << '\n';
  }

  void read(std::ifstream& in) {
    std::getline(in, name);
    std::getline(in, type);
    std::getline(in, due.iso);
    in >> priority;
    int s;
    in >> s;
    status = static_cast<Status>(s);
    in.ignore();
    std::getline(in, description);
  }
};

class ProjectManager {
 private:
  Project projects[MAX_PROJECTS];
  int count;

  void sortByDate() {
    for (int i = 0; i < count - 1; ++i)
      for (int j = i + 1; j < count; ++j)
        if (projects[j].getDate() < projects[i].getDate())
          std::swap(projects[i], projects[j]);
  }

  void sortByPriority() {
    for (int i = 0; i < count - 1; ++i)
      for (int j = i + 1; j < count; ++j)
        if (projects[j].getPrio() < projects[i].getPrio())
          std::swap(projects[i], projects[j]);
  }

 public:
  ProjectManager() : count(0) {}

  void addProject() {
    if (count >= MAX_PROJECTS) {
      std::cout << "Array full.\n";
      return;
    }
    projects[count++].input();
  }

  void displayAll() const {
    std::cout << "\nIdx Name                 Type        Due Date    P  Status "
                 "      Description\n";
    std::cout << "-------------------------------------------------------------"
                 "-------------------\n";
    for (int i = 0; i < count; ++i) projects[i].display(i);
    std::cout << '\n';
  }

  void filterByType() const {
    std::cin.ignore();
    std::string t;
    std::cout << "Enter type to filter: ";
    std::getline(std::cin, t);
    std::cout << "\nProjects of type \"" << t << "\":\n";
    for (int i = 0; i < count; ++i)
      if (projects[i].getType() == t) projects[i].display(i);
    std::cout << '\n';
  }

  void sortMenu() {
    char c;
    std::cout << "Sort by (D)ue date or (P)riority? ";
    std::cin >> c;
    if (c == 'D' || c == 'd')
      sortByDate();
    else
      sortByPriority();
    displayAll();
  }

  void markCompleted() {
    int idx;
    std::cout << "Enter index to mark completed: ";
    std::cin >> idx;
    if (idx >= 0 && idx < count)
      projects[idx].markCompleted();
    else
      std::cout << "Invalid index.\n";
  }

  void saveToFile() const {
    std::cin.ignore();
    std::string fname;
    std::cout << "Save to file: ";
    std::getline(std::cin, fname);
    try {
      std::ofstream out(fname);
      if (!out) throw std::ios_base::failure("open");
      out << count << '\n';
      for (int i = 0; i < count; ++i) projects[i].write(out);
      std::cout << "Saved.\n";
    } catch (...) {
      std::cout << "File write error.\n";
    }
  }

  void loadFromFile() {
    std::cin.ignore();
    std::string fname;
    std::cout << "Load from file: ";
    std::getline(std::cin, fname);
    try {
      std::ifstream in(fname);
      if (!in) throw std::ios_base::failure("open");
      in >> count;
      in.ignore();
      for (int i = 0; i < count && i < MAX_PROJECTS; ++i) projects[i].read(in);
      std::cout << "Loaded.\n";
    } catch (...) {
      std::cout << "File read error.\n";
      count = 0;
    }
  }
};

int main() {
  ProjectManager pm;
  bool running = true;
  while (running) {
    std::cout << "\n==== Project Dashboard ====\n"
              << "1. Add new project\n"
              << "2. View all projects\n"
              << "3. View projects by type\n"
              << "4. Sort projects\n"
              << "5. Mark project as completed\n"
              << "6. Save projects to file\n"
              << "7. Load projects from file\n"
              << "8. Exit\n"
              << "Choose: ";
    int choice;
    std::cin >> choice;
    switch (choice) {
      case 1:
        pm.addProject();
        break;
      case 2:
        pm.displayAll();
        break;
      case 3:
        pm.filterByType();
        break;
      case 4:
        pm.sortMenu();
        break;
      case 5:
        pm.markCompleted();
        break;
      case 6:
        pm.saveToFile();
        break;
      case 7:
        pm.loadFromFile();
        break;
      case 8:
        running = false;
        break;
      default:
        std::cout << "Invalid.\n";
    }
  }
  return 0;
}