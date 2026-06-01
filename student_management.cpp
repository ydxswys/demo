#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

struct Student {
    string id;
    string name;
    int age;
    double score;
};

const string DATA_FILE = "students.txt";

void pauseScreen()
{
    cout << "\n按回车键继续...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt)
{
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        }
        cout << "输入无效，请输入整数。\n";
        clearInput();
    }
}

double readDouble(const string& prompt)
{
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        }
        cout << "输入无效，请输入数字。\n";
        clearInput();
    }
}

string readLine(const string& prompt)
{
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

int findStudentIndex(const vector<Student>& students, const string& id)
{
    for (int i = 0; i < static_cast<int>(students.size()); ++i) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

void loadStudents(vector<Student>& students)
{
    ifstream file(DATA_FILE);
    if (!file.is_open()) {
        return;
    }

    Student student;
    while (file >> student.id >> student.name >> student.age >> student.score) {
        students.push_back(student);
    }
}

void saveStudents(const vector<Student>& students)
{
    ofstream file(DATA_FILE);
    if (!file.is_open()) {
        cout << "保存文件失败。\n";
        return;
    }

    for (const Student& student : students) {
        file << student.id << ' '
             << student.name << ' '
             << student.age << ' '
             << student.score << '\n';
    }
}

void printStudent(const Student& student)
{
    cout << left << setw(12) << student.id
         << setw(12) << student.name
         << setw(8) << student.age
         << fixed << setprecision(2) << student.score << '\n';
}

void showAllStudents(const vector<Student>& students)
{
    if (students.empty()) {
        cout << "暂无学生信息。\n";
        return;
    }

    cout << left << setw(12) << "学号"
         << setw(12) << "姓名"
         << setw(8) << "年龄"
         << "成绩\n";
    cout << "----------------------------------------\n";

    for (const Student& student : students) {
        printStudent(student);
    }
}

void addStudent(vector<Student>& students)
{
    Student student;

    student.id = readLine("请输入学号: ");
    if (student.id.empty()) {
        cout << "学号不能为空。\n";
        return;
    }

    if (findStudentIndex(students, student.id) != -1) {
        cout << "该学号已存在。\n";
        return;
    }

    student.name = readLine("请输入姓名: ");
    student.age = readInt("请输入年龄: ");
    student.score = readDouble("请输入成绩: ");

    students.push_back(student);
    saveStudents(students);
    cout << "添加成功。\n";
}

void deleteStudent(vector<Student>& students)
{
    string id = readLine("请输入要删除的学生学号: ");
    int index = findStudentIndex(students, id);

    if (index == -1) {
        cout << "未找到该学生。\n";
        return;
    }

    students.erase(students.begin() + index);
    saveStudents(students);
    cout << "删除成功。\n";
}

void updateStudent(vector<Student>& students)
{
    string id = readLine("请输入要修改的学生学号: ");
    int index = findStudentIndex(students, id);

    if (index == -1) {
        cout << "未找到该学生。\n";
        return;
    }

    cout << "当前信息:\n";
    printStudent(students[index]);

    students[index].name = readLine("请输入新姓名: ");
    students[index].age = readInt("请输入新年龄: ");
    students[index].score = readDouble("请输入新成绩: ");

    saveStudents(students);
    cout << "修改成功。\n";
}

void searchStudent(const vector<Student>& students)
{
    string id = readLine("请输入要查询的学生学号: ");
    int index = findStudentIndex(students, id);

    if (index == -1) {
        cout << "未找到该学生。\n";
        return;
    }

    cout << left << setw(12) << "学号"
         << setw(12) << "姓名"
         << setw(8) << "年龄"
         << "成绩\n";
    cout << "----------------------------------------\n";
    printStudent(students[index]);
}

void sortStudentsByScore(vector<Student>& students)
{
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.score > b.score;
    });

    saveStudents(students);
    cout << "已按成绩从高到低排序。\n";
    showAllStudents(students);
}

void showStatistics(const vector<Student>& students)
{
    if (students.empty()) {
        cout << "暂无学生信息。\n";
        return;
    }

    double total = 0.0;
    const Student* highest = &students[0];
    const Student* lowest = &students[0];

    for (const Student& student : students) {
        total += student.score;
        if (student.score > highest->score) {
            highest = &student;
        }
        if (student.score < lowest->score) {
            lowest = &student;
        }
    }

    cout << "学生人数: " << students.size() << '\n';
    cout << "平均成绩: " << fixed << setprecision(2) << total / students.size() << '\n';
    cout << "最高成绩: " << highest->name << " (" << highest->score << ")\n";
    cout << "最低成绩: " << lowest->name << " (" << lowest->score << ")\n";
}

void showMenu()
{
    cout << "\n========== 学生管理系统 ==========\n";
    cout << "1. 添加学生\n";
    cout << "2. 删除学生\n";
    cout << "3. 修改学生\n";
    cout << "4. 查询学生\n";
    cout << "5. 显示全部学生\n";
    cout << "6. 按成绩排序\n";
    cout << "7. 成绩统计\n";
    cout << "0. 退出系统\n";
    cout << "请选择: ";
}

int main()
{
    vector<Student> students;
    loadStudents(students);

    while (true) {
        showMenu();

        int choice;
        if (!(cin >> choice)) {
            cout << "输入无效，请重新选择。\n";
            clearInput();
            continue;
        }
        clearInput();

        switch (choice) {
        case 1:
            addStudent(students);
            pauseScreen();
            break;
        case 2:
            deleteStudent(students);
            pauseScreen();
            break;
        case 3:
            updateStudent(students);
            pauseScreen();
            break;
        case 4:
            searchStudent(students);
            pauseScreen();
            break;
        case 5:
            showAllStudents(students);
            pauseScreen();
            break;
        case 6:
            sortStudentsByScore(students);
            pauseScreen();
            break;
        case 7:
            showStatistics(students);
            pauseScreen();
            break;
        case 0:
            saveStudents(students);
            cout << "已退出系统。\n";
            return 0;
        default:
            cout << "没有这个选项，请重新选择。\n";
            pauseScreen();
            break;
        }
    }
}
