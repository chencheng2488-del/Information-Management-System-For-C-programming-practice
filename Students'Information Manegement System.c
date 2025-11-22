/*
 * Students' Information Management System / 学生信息管理系统 / SIMS
 * Copyright (c) 2025 陈宸 (Chen Chen / MccoyFreyr / Cchen / Mccoy)
 * Licensed under the MIT License. See LICENSE file for details.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> 
#include <windows.h>

// 颜色定义
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_RESET   "\033[0m"

// 软件版本信息常量定义
#define SOFTWARE_NAME "Students' Information Management System"
#define SOFTWARE_VERSION "v0.1.7"
#define SOFTWARE_VERSION_TEXT "v0.1.7-dev"
#define SOFTWARE_AUTHOR "MccoyFreyr / 陈宸"
#define COPYRIGHT_YEAR "2023-2025"
#define LICENSE_TYPE "MIT License"

// 学生信息结构体
typedef struct {
    char name[20];       // 姓名
    char gender[5];      // 性别
    char id[20];         // 学号
    char className[20];  // 班级
    char department[30]; // 院系
    char major[30];      // 专业
    float *scores;       // 成绩数组
    int scoreCount;      // 成绩数量
    float totalScore;    // 成绩总和
} Student;

// 学生信息管理系统结构体
typedef struct {
    Student *students;      // 学生数组
    int capacity;           // 容量
    int count;              // 当前学生数量
    char **scoreNames;      // 成绩名预设数组
    int scoreNameCount;     // 成绩名预设数量
    int scoreNameCapacity;  // 成绩名预设容量
    char **departmentNames; // 院系预设数组
    int departmentCount;    // 院系预设数量
    int departmentCapacity; // 院系预设容量
    char **majorNames;      // 专业预设数组
    int majorCount;         // 专业预设数量
    int majorCapacity;      // 专业预设容量
} StudentManager;

// 函数声明
StudentManager *initManager(int capacity);
void freeManager(StudentManager *manager);
char getKey();
void clearScreen();
void setColor(const char *color);
void showMenu();
void showSoftwareInfo();
void showInstructions();
void showDeveloperMessage();
int addStudent(StudentManager *manager);
int findStudentByName(StudentManager *manager, const char *name);
int findStudentById(StudentManager *manager, const char *id);
void displayStudent(const Student *student);
void searchStudents(StudentManager *manager);
void modifyStudent(StudentManager *manager);
void deleteStudent(StudentManager *manager);
void displayAllStudents(StudentManager *manager);
int isValidScore(float score);
int isValidGender(const char *gender);
int isValidId(const char *id);
int isValidName(const char *name);
int isEmptyString(const char *str);
void clearInputBuffer();
// 成绩名预设相关函数
void manageScoreNames(StudentManager *manager);
void addScoreName(StudentManager *manager, const char *name);
void clearScoreNames(StudentManager *manager);
void displayScoreNames(StudentManager *manager);
// 院系和专业预设相关函数
void manageDepartments(StudentManager *manager);
void addDepartment(StudentManager *manager, const char *name);
void clearDepartments(StudentManager *manager);
void displayDepartments(StudentManager *manager);
void manageMajors(StudentManager *manager);
void addMajor(StudentManager *manager, const char *name);
void clearMajors(StudentManager *manager);
void displayMajors(StudentManager *manager);
int isValidDepartment(StudentManager *manager, const char *department);
int isValidMajor(StudentManager *manager, const char *major);

// 初始化学生管理器
StudentManager *initManager(int capacity) {
    StudentManager *manager = (StudentManager *)malloc(sizeof(StudentManager));
    if (manager == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    
    manager->students = (Student *)malloc(sizeof(Student) * capacity);
    if (manager->students == NULL) {
        free(manager);
        printf("内存分配失败！\n");
        return NULL;
    }
    
    manager->capacity = capacity;
    manager->count = 0;
    
    // 初始化成绩名预设
    int scoreNameInitCapacity = 5;
    manager->scoreNames = (char **)malloc(sizeof(char *) * scoreNameInitCapacity);
    if (manager->scoreNames == NULL) {
        free(manager->students);
        free(manager);
        printf("内存分配失败！\n");
        return NULL;
    }
    manager->scoreNameCapacity = scoreNameInitCapacity;
    manager->scoreNameCount = 0;
    
    // 初始化院系预设
    int departmentInitCapacity = 5;
    manager->departmentNames = (char **)malloc(sizeof(char *) * departmentInitCapacity);
    if (manager->departmentNames == NULL) {
        free(manager->scoreNames);
        free(manager->students);
        free(manager);
        printf("内存分配失败！\n");
        return NULL;
    }
    manager->departmentCapacity = departmentInitCapacity;
    manager->departmentCount = 0;
    
    // 初始化专业预设
    int majorInitCapacity = 5;
    manager->majorNames = (char **)malloc(sizeof(char *) * majorInitCapacity);
    if (manager->majorNames == NULL) {
        free(manager->departmentNames);
        free(manager->scoreNames);
        free(manager->students);
        free(manager);
        printf("内存分配失败！\n");
        return NULL;
    }
    manager->majorCapacity = majorInitCapacity;
    manager->majorCount = 0;
    
    return manager;
}

// 释放学生管理器
void freeManager(StudentManager *manager) {
    if (manager != NULL) {
        if (manager->students != NULL) {
            // 释放每个学生的成绩数组
            for (int i = 0; i < manager->count; i++) {
                if (manager->students[i].scores != NULL) {
                    free(manager->students[i].scores);
                }
            }
            free(manager->students);
        }
        
        // 释放成绩名预设数组
        if (manager->scoreNames != NULL) {
            for (int i = 0; i < manager->scoreNameCount; i++) {
                if (manager->scoreNames[i] != NULL) {
                    free(manager->scoreNames[i]);
                }
            }
            free(manager->scoreNames);
        }
        
        // 释放院系预设数组
        if (manager->departmentNames != NULL) {
            for (int i = 0; i < manager->departmentCount; i++) {
                if (manager->departmentNames[i] != NULL) {
                    free(manager->departmentNames[i]);
                }
            }
            free(manager->departmentNames);
        }
        
        // 释放专业预设数组
        if (manager->majorNames != NULL) {
            for (int i = 0; i < manager->majorCount; i++) {
                if (manager->majorNames[i] != NULL) {
                    free(manager->majorNames[i]);
                }
            }
            free(manager->majorNames);
        }
        
        free(manager);
    }
}

// 显示院系列表
void displayDepartments(StudentManager *manager) {
    setColor(COLOR_YELLOW);
    if (manager->departmentCount == 0) {
        printf("\t\t当前没有预设的院系！\n");
    } else {
        printf("\t\t当前预设的院系列表：\n");
        for (int i = 0; i < manager->departmentCount; i++) {
            printf("\t\t%d. %s\n", i + 1, manager->departmentNames[i]);
        }
    }
    setColor(COLOR_RESET);
}

// 添加院系
void addDepartment(StudentManager *manager, const char *name) {
    // 检查院系是否已存在
    for (int i = 0; i < manager->departmentCount; i++) {
        if (strcmp(manager->departmentNames[i], name) == 0) {
            setColor(COLOR_RED);
            printf("\t\t院系 '%s' 已存在！\n", name);
            setColor(COLOR_RESET);
            return;
        }
    }
    
    // 检查是否需要扩容
    if (manager->departmentCount >= manager->departmentCapacity) {
        int newCapacity = manager->departmentCapacity * 2;
        char **newDepartmentNames = (char **)realloc(manager->departmentNames, sizeof(char *) * newCapacity);
        if (newDepartmentNames == NULL) {
            setColor(COLOR_RED);
            printf("\t\t内存分配失败，无法添加新院系！\n");
            setColor(COLOR_RESET);
            return;
        }
        manager->departmentNames = newDepartmentNames;
        manager->departmentCapacity = newCapacity;
    }
    
    // 添加新院系
    manager->departmentNames[manager->departmentCount] = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    if (manager->departmentNames[manager->departmentCount] == NULL) {
        setColor(COLOR_RED);
        printf("\t\t内存分配失败，无法添加新院系！\n");
        setColor(COLOR_RESET);
        return;
    }
    
    strcpy(manager->departmentNames[manager->departmentCount], name);
    manager->departmentCount++;
    
    setColor(COLOR_GREEN);
    printf("\t\t院系 '%s' 添加成功！\n", name);
    setColor(COLOR_RESET);
}

// 清除所有院系
void clearDepartments(StudentManager *manager) {
    for (int i = 0; i < manager->departmentCount; i++) {
        free(manager->departmentNames[i]);
    }
    manager->departmentCount = 0;
    
    setColor(COLOR_GREEN);
    printf("\t\t所有院系已清除！\n");
    setColor(COLOR_RESET);
}

// 管理院系预设
void manageDepartments(StudentManager *manager) {
    char choice;
    while (1) {
        clearScreen();
        setColor(COLOR_CYAN);
        printf("\n\t\t=============================================\n");
        printf("\t\t               院系预设管理                  \n");
        printf("\t\t=============================================\n");
        setColor(COLOR_RESET);
        
        displayDepartments(manager);
        
        setColor(COLOR_YELLOW);
        printf("\t\t1. 添加院系\n");
        printf("\t\t2. 清除所有院系\n");
        printf("\t\t0. 返回主菜单\n");
        printf("\t\t请输入选择: ");
        setColor(COLOR_RESET);
        
        choice = getch();
        clearInputBuffer();
        
        switch (choice) {
            case '1': {
                char department[30];
                printf("\t\t请输入院系名称: ");
                fgets(department, sizeof(department), stdin);
                department[strcspn(department, "\n")] = '\0';
                
                if (isEmptyString(department)) {
                    setColor(COLOR_RED);
                    printf("\t\t院系名称不能为空！\n");
                    setColor(COLOR_RESET);
                } else {
                    addDepartment(manager, department);
                }
                break;
            }
            case '2': {
                char confirm;
                setColor(COLOR_RED);
                printf("\t\t确定要清除所有院系吗？(y/n): ");
                setColor(COLOR_RESET);
                confirm = getch();
                printf("%c\n", confirm);
                if (confirm == 'y' || confirm == 'Y') {
                    clearDepartments(manager);
                } else {
                    printf("\t\t已取消操作！\n");
                }
                break;
            }
            case '0':
                return;
            default:
                setColor(COLOR_RED);
                printf("\t\t无效的选择！\n");
                setColor(COLOR_RESET);
        }
        
        setColor(COLOR_BLUE);
        printf("\t\t按任意键继续...");
        setColor(COLOR_RESET);
        getch();
    }
}

// 显示专业列表
void displayMajors(StudentManager *manager) {
    setColor(COLOR_YELLOW);
    if (manager->majorCount == 0) {
        printf("\t\t当前没有预设的专业！\n");
    } else {
        printf("\t\t当前预设的专业列表：\n");
        for (int i = 0; i < manager->majorCount; i++) {
            printf("\t\t%d. %s\n", i + 1, manager->majorNames[i]);
        }
    }
    setColor(COLOR_RESET);
}

// 添加专业
void addMajor(StudentManager *manager, const char *name) {
    // 检查专业是否已存在
    for (int i = 0; i < manager->majorCount; i++) {
        if (strcmp(manager->majorNames[i], name) == 0) {
            setColor(COLOR_RED);
            printf("\t\t专业 '%s' 已存在！\n", name);
            setColor(COLOR_RESET);
            return;
        }
    }
    
    // 检查是否需要扩容
    if (manager->majorCount >= manager->majorCapacity) {
        int newCapacity = manager->majorCapacity * 2;
        char **newMajorNames = (char **)realloc(manager->majorNames, sizeof(char *) * newCapacity);
        if (newMajorNames == NULL) {
            setColor(COLOR_RED);
            printf("\t\t内存分配失败，无法添加新专业！\n");
            setColor(COLOR_RESET);
            return;
        }
        manager->majorNames = newMajorNames;
        manager->majorCapacity = newCapacity;
    }
    
    // 添加新专业
    manager->majorNames[manager->majorCount] = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    if (manager->majorNames[manager->majorCount] == NULL) {
        setColor(COLOR_RED);
        printf("\t\t内存分配失败，无法添加新专业！\n");
        setColor(COLOR_RESET);
        return;
    }
    
    strcpy(manager->majorNames[manager->majorCount], name);
    manager->majorCount++;
    
    setColor(COLOR_GREEN);
    printf("\t\t专业 '%s' 添加成功！\n", name);
    setColor(COLOR_RESET);
}

// 清除所有专业
void clearMajors(StudentManager *manager) {
    for (int i = 0; i < manager->majorCount; i++) {
        free(manager->majorNames[i]);
    }
    manager->majorCount = 0;
    
    setColor(COLOR_GREEN);
    printf("\t\t所有专业已清除！\n");
    setColor(COLOR_RESET);
}

// 管理专业预设
void manageMajors(StudentManager *manager) {
    char choice;
    while (1) {
        clearScreen();
        setColor(COLOR_CYAN);
        printf("\n\t\t=============================================\n");
        printf("\t\t               专业预设管理                  \n");
        printf("\t\t=============================================\n");
        setColor(COLOR_RESET);
        
        displayMajors(manager);
        
        setColor(COLOR_YELLOW);
        printf("\t\t1. 添加专业\n");
        printf("\t\t2. 清除所有专业\n");
        printf("\t\t0. 返回主菜单\n");
        printf("\t\t请输入选择: ");
        setColor(COLOR_RESET);
        
        choice = getch();
        clearInputBuffer();
        
        switch (choice) {
            case '1': {
                char major[30];
                printf("\t\t请输入专业名称: ");
                fgets(major, sizeof(major), stdin);
                major[strcspn(major, "\n")] = '\0';
                
                if (isEmptyString(major)) {
                    setColor(COLOR_RED);
                    printf("\t\t专业名称不能为空！\n");
                    setColor(COLOR_RESET);
                } else {
                    addMajor(manager, major);
                }
                break;
            }
            case '2': {
                char confirm;
                setColor(COLOR_RED);
                printf("\t\t确定要清除所有专业吗？(y/n): ");
                setColor(COLOR_RESET);
                confirm = getch();
                printf("%c\n", confirm);
                if (confirm == 'y' || confirm == 'Y') {
                    clearMajors(manager);
                } else {
                    printf("\t\t已取消操作！\n");
                }
                break;
            }
            case '0':
                return;
            default:
                setColor(COLOR_RED);
                printf("\t\t无效的选择！\n");
                setColor(COLOR_RESET);
        }
        
        setColor(COLOR_BLUE);
        printf("\t\t按任意键继续...");
        setColor(COLOR_RESET);
        getch();
    }
}

// 验证院系是否有效
int isValidDepartment(StudentManager *manager, const char *department) {
    if (manager->departmentCount == 0) {
        return 0;  // 没有预设院系
    }
    
    for (int i = 0; i < manager->departmentCount; i++) {
        if (strcmp(manager->departmentNames[i], department) == 0) {
            return 1;  // 院系有效
        }
    }
    
    return 0;  // 院系无效
}

// 验证专业是否有效
int isValidMajor(StudentManager *manager, const char *major) {
    if (manager->majorCount == 0) {
        return 0;  // 没有预设专业
    }
    
    for (int i = 0; i < manager->majorCount; i++) {
        if (strcmp(manager->majorNames[i], major) == 0) {
            return 1;  // 专业有效
        }
    }
    
    return 0;  // 专业无效
}

// 获取单个按键输入（无需回车）
char getKey() {
    return _getch();
}

// 清屏
void clearScreen() {
    system("cls");
}

// 设置颜色
void setColor(const char *color) {
    printf("%s", color);
}

// 显示菜单
void showMenu() {
    clearScreen();
    setColor(COLOR_YELLOW);
    printf("\n\n");
    printf("                ==============================================================\n");
    printf("                *                                                            *\n");
    printf("                *                      学生信息管理系统                      *\n");
    printf("                *                                                            *\n");
    printf("                ==============================================================\n");
    printf("                *                       系统功能菜单                         *\n");
    printf("                --------------------------------------------------------------\n");
    setColor(COLOR_RESET);
    
    setColor(COLOR_CYAN);
    printf("                *  1. %-15s          ** 2. %-15s                *\n", "使用说明", "录入学生信息");
    printf("                ==============================================================\n");         
    printf("                *  3. %-15s         ** 4. %-15s                *\n", "显示学生信息", "查找学生信息");
    printf("                ==============================================================\n");         
    printf("                *  5. %-15s         ** 6. %-15s                *\n", "修改学生信息", "删除学生信息");
    printf("                ==============================================================\n");         
    printf("                *  7. %-15s         ** 8. %-15s                *\n", "院系预设设置", "专业预设设置");
    printf("                ==============================================================\n");         
    printf("                *  9. %-15s           ** a. %-15s                 *\n", "成绩名预设", "软件信息");
    printf("                ==============================================================\n");
    printf("                *  b. %-15s           ** 0. %-15s                 *\n", "开发者的话", "退出系统");
    printf("                ==============================================================\n");
    setColor(COLOR_RESET);
    
    setColor(COLOR_BLUE);
    printf("\n\t\tStudents' Information Management System developed by MccoyFreyr\n");
    setColor(COLOR_RESET);
    
    setColor(COLOR_MAGENTA);
    printf("\n\t\t请输入选择 (0-9, a, b): ");
    setColor(COLOR_RESET);
}

// 验证字符串是否为空或只包含空格
int isEmptyString(const char *str) {
    if (str == NULL || *str == '\0') return 1;
    while (*str != '\0') {
        if (*str != ' ' && *str != '\t') {
            return 0;
        }
        str++;
    }
    return 1;
}

// 清理输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 验证成绩有效性
int isValidScore(float score) {
    return score >= 0 && score <= 100;
}

// 验证性别有效性
int isValidGender(const char *gender) {
    // 支持多种输入方式避免编码问题
    return (strcmp(gender, "男") == 0 || 
            strcmp(gender, "女") == 0 || 
            strcmp(gender, "1") == 0 || 
            strcmp(gender, "0") == 0 || 
            strcmp(gender, "male") == 0 || 
            strcmp(gender, "female") == 0 || 
            strcmp(gender, "M") == 0 || 
            strcmp(gender, "F") == 0);
}

// 验证学号有效性
int isValidId(const char *id) {
    if (isEmptyString(id)) return 0;
    
    // 验证长度（学号长度应在合理范围内）
    int len = strlen(id);
    if (len < 4 || len > 20) {
        return 0;
    }
    
    // 验证字符：只允许数字和字母
    for (int i = 0; id[i] != '\0'; i++) {
        if (!((id[i] >= '0' && id[i] <= '9') || 
              (id[i] >= 'a' && id[i] <= 'z') || 
              (id[i] >= 'A' && id[i] <= 'Z'))) {
            return 0;
        }
    }
    return 1;
}

// 验证姓名有效性
int isValidName(const char *name) {
    if (isEmptyString(name)) return 0;
    
    // 验证长度
    int len = strlen(name);
    if (len < 1 || len > 20) {
        return 0;
    }
    
    
    return 1;
}

int main() {
    // 设置控制台标题
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    char consoleTitle[100];
    sprintf(consoleTitle, "学生信息管理系统 %s", SOFTWARE_VERSION_TEXT);
    SetConsoleTitle(consoleTitle);
    
    clearScreen();
    setColor(COLOR_GREEN);
    printf("\n\n\t\t=======================================");
    printf("\n\n\n\t\t      学生信息管理系统 正在启动...      ");
    printf("\n\n\n\t\t=======================================");
    setColor(COLOR_RESET);
    
    // 初始化系统
    printf("\n\t\t正在初始化系统...");
    StudentManager *manager = initManager(100);  // 初始容量100
    
    if (manager == NULL) {
        setColor(COLOR_RED);
        printf("\n\t\t系统初始化失败！");
        setColor(COLOR_RESET);
        printf("\t\t按任意键退出...");
        getKey();
        return 1;
    }
    
    setColor(COLOR_GREEN);
    printf("\n\t\t系统初始化成功！\n");
    setColor(COLOR_RESET);
    
    printf("\t\t按任意键继续...");
    getKey();
    
    char choice;
    
    while (1) {
        showMenu();
        choice = getKey();
        
        // 转换为小写以支持不区分大小写
        if (choice >= 'A' && choice <= 'Z') {
            choice += 32;
        }
        
        // 验证输入是否有效
        if (!((choice >= '0' && choice <= '9') || choice == 'a' || choice == 'b')) {
            clearScreen();
            setColor(COLOR_RED);
            printf("\n\n\t\t无效的选择，请重新输入！\n");
            setColor(COLOR_RESET);
            printf("\t\t按任意键继续...");
            getKey();
            continue;
        }
        
        switch (choice) {
            case '1':
                showInstructions();
                break;
            case '2':
                addStudent(manager);
                break;
            case '3':
                displayAllStudents(manager);
                break;
            case '4':
                searchStudents(manager);
                break;
            case '5':
                modifyStudent(manager);
                break;
            case '6':
                deleteStudent(manager);
                break;
            case '7':
                manageDepartments(manager);
                break;
            case '8':
                manageMajors(manager);
                break;
            case '9':
                manageScoreNames(manager);
                break;
            case 'a':
                showSoftwareInfo();
                break;
            case 'b':
                showDeveloperMessage();
                break;
            case '0':
                clearScreen();
                setColor(COLOR_GREEN);
                printf("\n\n\t\t感谢使用学生信息管理系统！\n\n");
                setColor(COLOR_RESET);
                freeManager(manager);
                return 0;
            default:
                clearScreen();
                setColor(COLOR_RED);
                printf("\n\n\t\t无效的选择，请重新输入！\n");
                setColor(COLOR_RESET);
                printf("\t\t按任意键继续...");
                getKey();
        }
    }
    
    return 0;
}

// 录入学生信息
int addStudent(StudentManager *manager) {
    clearScreen();
    setColor(COLOR_YELLOW);
    printf("\n\n\t\t===== 录入学生信息 =====\n\n");
    setColor(COLOR_RESET);
    
    // 检查是否已经设置了院系和专业预设
    if (manager->departmentCount == 0 || manager->majorCount == 0) {
        setColor(COLOR_RED);
        printf("\t\t尚未输入预设！请先设置院系和专业预设。\n");
        setColor(COLOR_RESET);
        printf("\t\t按任意键返回...");
        getKey();
        return 0;
    }
    
    // 检查是否已满
    if (manager->count >= manager->capacity) {
        setColor(COLOR_RED);
        printf("\t\t系统容量已满，无法添加新学生！\n");
        setColor(COLOR_RESET);
        printf("\t\t按任意键返回...");
        getKey();
        return 0;
    }
    
    Student *student = &manager->students[manager->count];
    char temp[100];
    int result;
    
    // 录入姓名（带验证）
    while (1) {
        printf("\t\t姓名: ");
        result = scanf("%s", temp);
        clearInputBuffer();
        
        if (result == 1 && isValidName(temp)) {
            strcpy(student->name, temp);
            break;
        } else {
            setColor(COLOR_RED);
            printf("\t\t姓名无效，请输入1-20个字符！\n");
            setColor(COLOR_RESET);
        }
    }
    
    // 录入性别（带验证）
    while (1) {
        printf("\t\t性别 (男/女): ");
        fgets(temp, sizeof(temp), stdin);
        // 移除末尾的换行符
        size_t len = strlen(temp);
        if (len > 0 && temp[len - 1] == '\n') {
            temp[len - 1] = '\0';
        }
        
        if (isValidGender(temp)) {
            strcpy(student->gender, temp);
            break;
        } else {
            setColor(COLOR_RED);
            printf("\t\t性别无效，请输入'男'或'女'！\n");
            setColor(COLOR_RESET);
        }
    }
    
    // 录入学号（带验证）
    while (1) {
        printf("\t\t学号 (4-20位数字和字母): ");
        result = scanf("%s", temp);
        clearInputBuffer();
        
        if (result == 1 && isValidId(temp)) {
            // 检查学号是否已存在
            int exists = 0;
            for (int i = 0; i < manager->count; i++) {
                if (strcmp(manager->students[i].id, temp) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (exists) {
                setColor(COLOR_RED);
                printf("\t\t学号已存在！\n");
                setColor(COLOR_RESET);
            } else {
                strcpy(student->id, temp);
                break;
            }
        } else {
            setColor(COLOR_RED);
            printf("\t\t学号无效，请输入4-20位数字和字母！\n");
            setColor(COLOR_RESET);
        }
    }
    
    // 录入班级（带基本验证）
    while (1) {
        printf("\t\t班级: ");
        result = scanf("%s", temp);
        clearInputBuffer();
        
        if (result == 1 && !isEmptyString(temp) && strlen(temp) < 20) {
            strcpy(student->className, temp);
            break;
        } else {
            setColor(COLOR_RED);
            printf("\t\t班级名称无效，请输入有效名称！\n");
            setColor(COLOR_RESET);
        }
    }
    
    // 从预设列表中选择院系
    while (1) {
        setColor(COLOR_CYAN);
        printf("\t\t预设院系列表：\n");
        for (int i = 0; i < manager->departmentCount; i++) {
            printf("\t\t%d. %s\n", i + 1, manager->departmentNames[i]);
        }
        setColor(COLOR_RESET);
        
        printf("\t\t请选择院系 (1-%d): ", manager->departmentCount);
        int choice;
        result = scanf("%d", &choice);
        clearInputBuffer();
        
        if (result == 1 && choice >= 1 && choice <= manager->departmentCount) {
            strcpy(student->department, manager->departmentNames[choice - 1]);
            break;
        } else {
            setColor(COLOR_RED);
            printf("\t\t选择无效，请输入正确的序号！\n");
            setColor(COLOR_RESET);
        }
    }
    
    // 从预设列表中选择专业
    while (1) {
        setColor(COLOR_CYAN);
        printf("\t\t预设专业列表：\n");
        for (int i = 0; i < manager->majorCount; i++) {
            printf("\t\t%d. %s\n", i + 1, manager->majorNames[i]);
        }
        setColor(COLOR_RESET);
        
        printf("\t\t请选择专业 (1-%d): ", manager->majorCount);
        int choice;
        result = scanf("%d", &choice);
        clearInputBuffer();
        
        if (result == 1 && choice >= 1 && choice <= manager->majorCount) {
            strcpy(student->major, manager->majorNames[choice - 1]);
            break;
        } else {
            setColor(COLOR_RED);
            printf("\t\t选择无效，请输入正确的序号！\n");
            setColor(COLOR_RESET);
        }
    }
    
    // 初始化成绩数组
    student->scores = NULL;
    student->scoreCount = 0;
    student->totalScore = 0.0;
    
    // 录入成绩（支持多个成绩，输入end结束）
    setColor(COLOR_YELLOW);
    printf("\t\t请输入成绩 (0-100)，输入end结束输入：\n");
    
    // 如果有预设的成绩名，显示出来
    if (manager->scoreNameCount > 0) {
        setColor(COLOR_CYAN);
        printf("\t\t预设成绩名：");
        for (int i = 0; i < manager->scoreNameCount; i++) {
            printf("%s", manager->scoreNames[i]);
            if (i < manager->scoreNameCount - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
    setColor(COLOR_RESET);
    
    int maxScores = 10; // 初始最大成绩数量
    student->scores = (float *)malloc(maxScores * sizeof(float));
    
    while (1) {
        char input[20];
        // 如果有预设的成绩名且当前输入的成绩索引小于预设数量，显示预设的成绩名
        if (manager->scoreNameCount > 0 && student->scoreCount < manager->scoreNameCount) {
            printf("\t\t%s: ", manager->scoreNames[student->scoreCount]);
        } else {
            printf("\t\t成绩 %d: ", student->scoreCount + 1);
        }
        scanf("%s", input);
        
        // 检查是否输入end
        if (strcmp(input, "end") == 0) {
            break;
        }
        
        // 尝试转换为数字
        char *endptr;
        float scoreValue = strtof(input, &endptr);
        
        // 验证输入是否为有效数字且在0-100之间
        if (*endptr == '\0' && isValidScore(scoreValue)) {
            // 检查是否需要扩容
            if (student->scoreCount >= maxScores) {
                maxScores *= 2;
                student->scores = (float *)realloc(student->scores, maxScores * sizeof(float));
            }
            
            // 添加成绩
            student->scores[student->scoreCount] = scoreValue;
            student->totalScore += scoreValue;
            student->scoreCount++;
        } else {
            setColor(COLOR_RED);
            printf("\t\t成绩无效，请输入0-100之间的数字或输入end结束！\n");
            setColor(COLOR_RESET);
        }
    }
    
    // 如果没有输入任何成绩，设置一个默认值
    if (student->scoreCount == 0) {
        student->scores[0] = 0.0;
        student->scoreCount = 1;
        student->totalScore = 0.0;
        setColor(COLOR_YELLOW);
        printf("\t\t未输入成绩，已设置默认值0分\n");
        setColor(COLOR_RESET);
    } else {
        // 重新分配内存到实际需要的大小
        student->scores = (float *)realloc(student->scores, student->scoreCount * sizeof(float));
    }
    
    manager->count++;
    
    setColor(COLOR_GREEN);
    printf("\n\t\t学生信息录入成功！\n");
    setColor(COLOR_RESET);
    printf("\t\t按任意键返回...");
    getKey();
    return 1;
}

// 根据姓名查找学生
int findStudentByName(StudentManager *manager, const char *name) {
    if (manager == NULL || name == NULL || isEmptyString(name)) {
        return -1;
    }
    
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->students[i].name, name) == 0) {
            return i;
        }
    }
    
    // 查找包含该姓名的学生（模糊查找）
    for (int i = 0; i < manager->count; i++) {
        if (strstr(manager->students[i].name, name) != NULL) {
            return i;
        }
    }
    
    return -1;  // 未找到
}

// 根据学号查找学生
int findStudentById(StudentManager *manager, const char *id) {
    if (manager == NULL || id == NULL || isEmptyString(id)) {
        return -1;
    }
    
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->students[i].id, id) == 0) {
            return i;
        }
    }
    
    return -1;  // 未找到
}

// 显示单个学生信息
void displayStudent(const Student *student) {
    if (student == NULL) {
        return;
    }
    
    setColor(COLOR_CYAN);
    printf("\n============================================================\n");
    printf("\t             学生信息详情\n");
    printf("============================================================\n");
    setColor(COLOR_RESET);
    
    setColor(COLOR_YELLOW);
    printf("\t姓名: %s\n", student->name);
    printf("\t学号: %s\n", student->id);
    printf("\t性别: %s\n", student->gender);
    printf("\t班级: %s\n", student->className);
    printf("\t院系: %s\n", student->department);
    printf("\t专业: %s\n", student->major);
    printf("\t成绩列表: ");
    for (int i = 0; i < student->scoreCount; i++) {
        printf("%.2f", student->scores[i]);
        if (i < student->scoreCount - 1) {
            printf(", ");
        }
    }
    printf("\n");
    printf("\t成绩总和: %.2f\n", student->totalScore);
    setColor(COLOR_RESET);
    
    setColor(COLOR_CYAN);
    printf("============================================================\n");
    setColor(COLOR_RESET);
}

// 查找学生信息
void searchStudents(StudentManager *manager) {
    if (manager == NULL) {
        setColor(COLOR_RED);
        printf("\n系统错误！\n");
        setColor(COLOR_RESET);
        return;
    }
    
    if (manager->count == 0) {
        setColor(COLOR_YELLOW);
        printf("\n暂无学生信息！\n");
        setColor(COLOR_RESET);
        printf("\t\t按任意键返回...");
        getKey();
        return;
    }
    
    char searchChoice;
    char searchInput[100];
    int index;
    
    while (1) {
        clearScreen();
        setColor(COLOR_GREEN);
        printf("\n\n\t\t\t=======================================\n");
        printf("\t\t\t          查找学生信息          \n");
        printf("\t\t\t=======================================\n\n");
        setColor(COLOR_RESET);
        
        setColor(COLOR_YELLOW);
        printf("\t\t[1] 按姓名查找\n");
        printf("\t\t[2] 按学号查找\n");
        printf("\t\t[0] 返回主菜单\n\n");
        setColor(COLOR_RESET);
        
        setColor(COLOR_CYAN);
        printf("\t\t请选择查找方式: ");
        setColor(COLOR_RESET);
        
        searchChoice = getKey();
        
        if (searchChoice == '0') {
            return;
        }
        
        if (searchChoice == '1' || searchChoice == '2') {
            break;
        }
        
        setColor(COLOR_RED);
        printf("\n\t\t无效的选择，请重新输入！\n");
        setColor(COLOR_RESET);
        Sleep(1000);
    }
    
    clearInputBuffer();
    
    if (searchChoice == '1') {
        setColor(COLOR_CYAN);
        printf("\n\t\t请输入要查找的学生姓名: ");
        setColor(COLOR_RESET);
        
        if (fgets(searchInput, sizeof(searchInput), stdin) != NULL) {
            // 去除换行符
            searchInput[strcspn(searchInput, "\n")] = '\0';
            
            if (isEmptyString(searchInput)) {
                setColor(COLOR_RED);
                printf("\n\t\t姓名不能为空！\n");
                setColor(COLOR_RESET);
                printf("\t\t按任意键返回...");
                getKey();
                return;
            }
            
            index = findStudentByName(manager, searchInput);
            
            if (index != -1) {
                displayStudent(&manager->students[index]);
                setColor(COLOR_GREEN);
                printf("\t\t学生信息查找成功！\n");
                setColor(COLOR_RESET);
            } else {
                setColor(COLOR_RED);
                printf("\n\t\t未找到该学生信息！\n");
                setColor(COLOR_RESET);
            }
        }
    } else if (searchChoice == '2') {
        setColor(COLOR_CYAN);
        printf("\n\t\t请输入要查找的学生学号: ");
        setColor(COLOR_RESET);
        
        if (fgets(searchInput, sizeof(searchInput), stdin) != NULL) {
            // 去除换行符
            searchInput[strcspn(searchInput, "\n")] = '\0';
            
            if (isEmptyString(searchInput)) {
                setColor(COLOR_RED);
                printf("\n\t\t学号不能为空！\n");
                setColor(COLOR_RESET);
                printf("\t\t按任意键返回...");
                getKey();
                return;
            }
            
            index = findStudentById(manager, searchInput);
            
            if (index != -1) {
                displayStudent(&manager->students[index]);
                setColor(COLOR_GREEN);
                printf("\t\t学生信息查找成功！\n");
                setColor(COLOR_RESET);
            } else {
                setColor(COLOR_RED);
                printf("\n\t\t未找到该学生信息！\n");
                setColor(COLOR_RESET);
            }
        }
    }
    
    printf("\n\t\t按任意键返回...");
    getKey();
}

// 修改学生信息
void modifyStudent(StudentManager *manager) {
    if (manager == NULL) {
        setColor(COLOR_RED);
        printf("\n系统错误！\n");
        setColor(COLOR_RESET);
        return;
    }
    
    if (manager->count == 0) {
        setColor(COLOR_YELLOW);
        printf("\n暂无学生信息！\n");
        setColor(COLOR_RESET);
        printf("\n\t\t按任意键返回...");
        getKey();
        return;
    }
    
    char searchInput[100];
    int index = -1;
    
    clearScreen();
    setColor(COLOR_GREEN);
    printf("\n\n\t\t\t=======================================\n");
    printf("\t\t\t          修改学生信息          \n");
    printf("\t\t\t=======================================\n\n");
    setColor(COLOR_RESET);
    
    setColor(COLOR_CYAN);
    printf("\t\t请输入要修改的学生学号: ");
    setColor(COLOR_RESET);
    
    clearInputBuffer();
    if (fgets(searchInput, sizeof(searchInput), stdin) != NULL) {
        // 去除换行符
        searchInput[strcspn(searchInput, "\n")] = '\0';
        
        if (isEmptyString(searchInput)) {
            setColor(COLOR_RED);
            printf("\n\t\t学号不能为空！\n");
            setColor(COLOR_RESET);
            printf("\n\t\t按任意键返回...");
            getKey();
            return;
        }
        
        index = findStudentById(manager, searchInput);
        
        if (index == -1) {
            // 尝试按姓名查找
            setColor(COLOR_YELLOW);
            printf("\n\t\t未找到该学号的学生，是否尝试按姓名查找？(y/n): ");
            setColor(COLOR_RESET);
            
            char confirm = getKey();
            if (confirm == 'y' || confirm == 'Y') {
                clearInputBuffer();
                setColor(COLOR_CYAN);
                printf("\n\t\t请输入学生姓名: ");
                setColor(COLOR_RESET);
                
                if (fgets(searchInput, sizeof(searchInput), stdin) != NULL) {
                    searchInput[strcspn(searchInput, "\n")] = '\0';
                    
                    if (isEmptyString(searchInput)) {
                        setColor(COLOR_RED);
                        printf("\n\t\t姓名不能为空！\n");
                        setColor(COLOR_RESET);
                        printf("\n\t\t按任意键返回...");
                        getKey();
                        return;
                    }
                    
                    index = findStudentByName(manager, searchInput);
                }
            }
        }
    }
    
    if (index == -1) {
        setColor(COLOR_RED);
        printf("\n\t\t未找到该学生信息！\n");
        setColor(COLOR_RESET);
        printf("\n\t\t按任意键返回...");
        getKey();
        return;
    }
    
    // 显示找到的学生信息
    displayStudent(&manager->students[index]);
    
    char modifyChoice;
    char newData[100];
    
    while (1) {
        clearScreen();
        displayStudent(&manager->students[index]);
        
        setColor(COLOR_YELLOW);
        printf("\n\t\t请选择要修改的信息项：\n");
        printf("\t\t[1] 姓名\n");
        printf("\t\t[2] 性别\n");
        printf("\t\t[3] 班级\n");
        printf("\t\t[4] 院系\n");
        printf("\t\t[5] 专业\n");
        printf("\t\t[6] 成绩\n");
        printf("\t\t[0] 完成修改\n\n");
        setColor(COLOR_RESET);
        
        setColor(COLOR_CYAN);
        printf("\t\t请选择: ");
        setColor(COLOR_RESET);
        
        modifyChoice = getKey();
        
        if (modifyChoice == '0') {
            break;
        }
        
        clearInputBuffer();
        
        switch (modifyChoice) {
            case '1':
                do {
                    setColor(COLOR_CYAN);
                    printf("\n\t\t请输入新姓名: ");
                    setColor(COLOR_RESET);
                    
                    if (fgets(newData, sizeof(newData), stdin) != NULL) {
                        newData[strcspn(newData, "\n")] = '\0';
                        
                        if (isValidName(newData)) {
                            strcpy(manager->students[index].name, newData);
                            setColor(COLOR_GREEN);
                            printf("\t\t姓名修改成功！\n");
                            setColor(COLOR_RESET);
                            break;
                        } else {
                            setColor(COLOR_RED);
                            printf("\t\t姓名格式错误！请输入1-20个字符。\n");
                            setColor(COLOR_RESET);
                        }
                    }
                } while (1);
                break;
                
            case '2':
                do {
                    setColor(COLOR_CYAN);
                    printf("\n\t\t请输入新性别 (男/女): ");
                    setColor(COLOR_RESET);
                    
                    // 确保输入缓冲区干净
                    clearInputBuffer();
                    
                    if (fgets(newData, sizeof(newData), stdin) != NULL) {
                        // 移除末尾的换行符
                        size_t len = strlen(newData);
                        if (len > 0 && newData[len - 1] == '\n') {
                            newData[len - 1] = '\0';
                        }
                        
                        if (isValidGender(newData)) {
                            strcpy(manager->students[index].gender, newData);
                            setColor(COLOR_GREEN);
                            printf("\t\t性别修改成功！\n");
                            setColor(COLOR_RESET);
                            break;
                        } else {
                            setColor(COLOR_RED);
                            printf("\t\t性别无效，请输入'男'或'女'！\n");
                            setColor(COLOR_RESET);
                        }
                    }
                } while (1);
                break;
                
            case '3':
                do {
                    setColor(COLOR_CYAN);
                    printf("\n\t\t请输入新班级: ");
                    setColor(COLOR_RESET);
                    
                    if (fgets(newData, sizeof(newData), stdin) != NULL) {
                        newData[strcspn(newData, "\n")] = '\0';
                        
                        if (!isEmptyString(newData) && strlen(newData) <= 50) {
                            strcpy(manager->students[index].className, newData);
                            setColor(COLOR_GREEN);
                            printf("\t\t班级修改成功！\n");
                            setColor(COLOR_RESET);
                            break;
                        } else {
                            setColor(COLOR_RED);
                            printf("\t\t班级名称不能为空且不能超过50个字符！\n");
                            setColor(COLOR_RESET);
                        }
                    }
                } while (1);
                break;
                
            case '4':
                do {
                    setColor(COLOR_CYAN);
                    printf("\n\t\t请输入新院系: ");
                    setColor(COLOR_RESET);
                    
                    if (fgets(newData, sizeof(newData), stdin) != NULL) {
                        newData[strcspn(newData, "\n")] = '\0';
                        
                        if (!isEmptyString(newData) && strlen(newData) <= 50) {
                            strcpy(manager->students[index].department, newData);
                            setColor(COLOR_GREEN);
                            printf("\t\t院系修改成功！\n");
                            setColor(COLOR_RESET);
                            break;
                        } else {
                            setColor(COLOR_RED);
                            printf("\t\t院系名称不能为空且不能超过50个字符！\n");
                            setColor(COLOR_RESET);
                        }
                    }
                } while (1);
                break;
                
            case '5':
                do {
                    setColor(COLOR_CYAN);
                    printf("\n\t\t请输入新专业: ");
                    setColor(COLOR_RESET);
                    
                    if (fgets(newData, sizeof(newData), stdin) != NULL) {
                        newData[strcspn(newData, "\n")] = '\0';
                        
                        if (!isEmptyString(newData) && strlen(newData) <= 50) {
                            strcpy(manager->students[index].major, newData);
                            setColor(COLOR_GREEN);
                            printf("\t\t专业修改成功！\n");
                            setColor(COLOR_RESET);
                            break;
                        } else {
                            setColor(COLOR_RED);
                            printf("\t\t专业名称不能为空且不能超过50个字符！\n");
                            setColor(COLOR_RESET);
                        }
                    }
                } while (1);
                break;
                
            case '6':
                // 显示当前成绩
                setColor(COLOR_YELLOW);
                printf("\n\t\t当前成绩列表: ");
                for (int i = 0; i < manager->students[index].scoreCount; i++) {
                    // 如果有预设的成绩名，显示成绩名
                    if (manager->scoreNameCount > 0 && i < manager->scoreNameCount) {
                        printf("%s: %.2f", manager->scoreNames[i], manager->students[index].scores[i]);
                    } else {
                        printf("%.2f", manager->students[index].scores[i]);
                    }
                    if (i < manager->students[index].scoreCount - 1) {
                        printf(", ");
                    }
                }
                printf("\n\t\t请输入新的成绩列表 (0-100)，输入end结束输入：\n");
                
                // 如果有预设的成绩名，显示出来
                if (manager->scoreNameCount > 0) {
                    setColor(COLOR_CYAN);
                    printf("\t\t预设成绩名：");
                    for (int i = 0; i < manager->scoreNameCount; i++) {
                        printf("%s", manager->scoreNames[i]);
                        if (i < manager->scoreNameCount - 1) {
                            printf(", ");
                        }
                    }
                    printf("\n");
                }
                setColor(COLOR_RESET);
                
                // 释放旧的成绩数组
                if (manager->students[index].scores != NULL) {
                    free(manager->students[index].scores);
                }
                
                // 重新初始化成绩信息
                int maxScores = 10;
                manager->students[index].scores = (float *)malloc(maxScores * sizeof(float));
                manager->students[index].scoreCount = 0;
                manager->students[index].totalScore = 0.0;
                
                while (1) {
                    char input[20];
                    // 如果有预设的成绩名且当前输入的成绩索引小于预设数量，显示预设的成绩名
                    if (manager->scoreNameCount > 0 && manager->students[index].scoreCount < manager->scoreNameCount) {
                        printf("\t\t%s: ", manager->scoreNames[manager->students[index].scoreCount]);
                    } else {
                        printf("\t\t成绩 %d: ", manager->students[index].scoreCount + 1);
                    }
                    scanf("%s", input);
                    
                    // 检查是否输入end
                    if (strcmp(input, "end") == 0) {
                        break;
                    }
                    
                    // 尝试转换为数字
                    char *endptr;
                    float scoreValue = strtof(input, &endptr);
                    
                    // 验证输入是否为有效数字且在0-100之间
                    if (*endptr == '\0' && isValidScore(scoreValue)) {
                        // 检查是否需要扩容
                        if (manager->students[index].scoreCount >= maxScores) {
                            maxScores *= 2;
                            manager->students[index].scores = (float *)realloc(manager->students[index].scores, maxScores * sizeof(float));
                        }
                        
                        // 添加成绩
                        manager->students[index].scores[manager->students[index].scoreCount] = scoreValue;
                        manager->students[index].totalScore += scoreValue;
                        manager->students[index].scoreCount++;
                    } else {
                        setColor(COLOR_RED);
                        printf("\t\t成绩无效，请输入0-100之间的数字或输入end结束！\n");
                        setColor(COLOR_RESET);
                    }
                }
                
                // 如果没有输入任何成绩，设置一个默认值
                if (manager->students[index].scoreCount == 0) {
                    manager->students[index].scores[0] = 0.0;
                    manager->students[index].scoreCount = 1;
                    manager->students[index].totalScore = 0.0;
                    setColor(COLOR_YELLOW);
                    printf("\t\t未输入成绩，已设置默认值0分\n");
                    setColor(COLOR_RESET);
                } else {
                    // 重新分配内存到实际需要的大小
                    manager->students[index].scores = (float *)realloc(manager->students[index].scores, manager->students[index].scoreCount * sizeof(float));
                }
                
                setColor(COLOR_GREEN);
                printf("\t\t成绩修改成功！\n");
                setColor(COLOR_RESET);
                break;
                
            default:
                setColor(COLOR_RED);
                printf("\t\t无效的选择！\n");
                setColor(COLOR_RESET);
                Sleep(1000);
        }
        
        printf("\t\t按任意键继续...");
        getKey();
    }
    
    setColor(COLOR_GREEN);
    printf("\n\t\t学生信息修改完成！\n");
    setColor(COLOR_RESET);
    printf("\n\t\t按任意键返回...");
    getKey();
}

// 删除学生信息
void deleteStudent(StudentManager *manager) {
    if (manager == NULL) {
        setColor(COLOR_RED);
        printf("\n系统错误！\n");
        setColor(COLOR_RESET);
        return;
    }
    
    clearScreen();
    setColor(COLOR_GREEN);
    printf("\n\n\t\t\t=======================================\n");
    printf("\t\t\t          删除学生信息          \n");
    printf("\t\t\t=======================================\n\n");
    setColor(COLOR_RESET);
    
    if (manager->count == 0) {
        setColor(COLOR_YELLOW);
        printf("\n\t\t暂无学生信息！\n");
        setColor(COLOR_RESET);
        printf("\n\t\t按任意键返回...");
        getKey();
        return;
    }
    
    char searchInput[100];
    int index = -1;
    int searchType = 1; // 默认按学号搜索
    
    // 选择搜索类型
    setColor(COLOR_CYAN);
    printf("\t\t请选择搜索方式：\n");
    printf("\t\t[1] 按学号删除\n");
    printf("\t\t[2] 按姓名删除\n\n");
    setColor(COLOR_RESET);
    
    char choice = getKey();
    if (choice == '2') {
        searchType = 2;
    }
    
    clearInputBuffer();
    
    if (searchType == 1) {
        setColor(COLOR_CYAN);
        printf("\n\t\t请输入要删除的学生学号: ");
        setColor(COLOR_RESET);
        
        if (fgets(searchInput, sizeof(searchInput), stdin) != NULL) {
            searchInput[strcspn(searchInput, "\n")] = '\0';
            
            if (isEmptyString(searchInput)) {
                setColor(COLOR_RED);
                printf("\n\t\t学号不能为空！\n");
                setColor(COLOR_RESET);
                printf("\n\t\t按任意键返回...");
                getKey();
                return;
            }
            
            index = findStudentById(manager, searchInput);
            
            if (index == -1) {
                // 尝试按姓名查找
                setColor(COLOR_YELLOW);
                printf("\n\t\t未找到该学号的学生，是否尝试按姓名查找？(y/n): ");
                setColor(COLOR_RESET);
                
                char confirm = getKey();
                if (confirm == 'y' || confirm == 'Y') {
                    clearInputBuffer();
                    searchType = 2;
                }
            }
        }
    }
    
    if (searchType == 2 && index == -1) {
        setColor(COLOR_CYAN);
        printf("\n\t\t请输入要删除的学生姓名: ");
        setColor(COLOR_RESET);
        
        if (fgets(searchInput, sizeof(searchInput), stdin) != NULL) {
            searchInput[strcspn(searchInput, "\n")] = '\0';
            
            if (isEmptyString(searchInput)) {
                setColor(COLOR_RED);
                printf("\n\t\t姓名不能为空！\n");
                setColor(COLOR_RESET);
                printf("\n\t\t按任意键返回...");
                getKey();
                return;
            }
            
            index = findStudentByName(manager, searchInput);
        }
    }
    
    if (index == -1) {
        setColor(COLOR_RED);
        printf("\n\t\t未找到该学生信息！\n");
        setColor(COLOR_RESET);
        printf("\n\t\t按任意键返回...");
        getKey();
        return;
    }
    
    // 显示要删除的学生信息
    setColor(COLOR_CYAN);
    printf("\n\t\t要删除的学生信息:\n");
    setColor(COLOR_RESET);
    displayStudent(&manager->students[index]);
    
    setColor(COLOR_RED);
    printf("\n\t\t⚠️  警告：此操作将永久删除该学生信息！\n");
    printf("\t\t是否确认删除？(y/n): ");
    setColor(COLOR_RESET);
    
    char confirm = getKey();
    if (confirm == 'y' || confirm == 'Y') {
        // 删除学生（将后面的学生前移）
        for (int i = index; i < manager->count - 1; i++) {
            manager->students[i] = manager->students[i + 1];
        }
        manager->count--;
        
        setColor(COLOR_GREEN);
        printf("\n\t\t✅ 学生信息删除成功！\n");
        setColor(COLOR_RESET);
    } else {
        setColor(COLOR_YELLOW);
        printf("\n\t\t已取消删除操作！\n");
        setColor(COLOR_RESET);
    }
    
    printf("\n\t\t按任意键返回...");
    getKey();
}

// 显示所有学生信息
void displayAllStudents(StudentManager *manager) {
    clearScreen();
    setColor(COLOR_YELLOW);
    printf("\n\n\t\t===== 学生信息查看 =====\n\n");
    setColor(COLOR_RESET);
    
    // 显示筛选菜单
    setColor(COLOR_CYAN);
    printf("\t\t1. 查看所有学生\n");
    printf("\t\t2. 按专业筛选查看\n");
    setColor(COLOR_RESET);
    
    printf("\t\t请选择操作 (1-2): ");
    int choice;
    int result = scanf("%d", &choice);
    clearInputBuffer();
    
    if (result != 1 || (choice != 1 && choice != 2)) {
        setColor(COLOR_RED);
        printf("\t\t选择无效！\n");
        setColor(COLOR_RESET);
        printf("\t\t按任意键返回...");
        getKey();
        return;
    }
    
    clearScreen();
    
    if (manager->count == 0) {
        setColor(COLOR_YELLOW);
        printf("\n\n\t\t===== 学生信息查看 =====\n\n");
        setColor(COLOR_RESET);
        setColor(COLOR_RED);
        printf("\t\t当前没有学生信息！\n");
        setColor(COLOR_RESET);
        printf("\t\t按任意键返回...");
        getKey();
        return;
    }
    
    if (choice == 1) {
        // 查看所有学生
        setColor(COLOR_YELLOW);
        printf("\n\n\t\t===== 所有学生信息 =====\n\n");
        setColor(COLOR_RESET);
        
        setColor(COLOR_CYAN);
        printf("\t\t共有 %d 名学生\n\n", manager->count);
        setColor(COLOR_RESET);
        
        for (int i = 0; i < manager->count; i++) {
            printf("\t\t学生 %d:\n", i + 1);
            displayStudent(&manager->students[i]);
            printf("\t\t-----------------------------\n");
        }
    } else if (choice == 2) {
        // 按专业筛选查看
        if (manager->majorCount == 0) {
            setColor(COLOR_YELLOW);
            printf("\n\n\t\t===== 学生信息查看 =====\n\n");
            setColor(COLOR_RESET);
            setColor(COLOR_RED);
            printf("\t\t尚未设置专业预设！\n");
            setColor(COLOR_RESET);
            printf("\t\t按任意键返回...");
            getKey();
            return;
        }
        
        // 显示专业列表供选择
        setColor(COLOR_YELLOW);
        printf("\n\n\t\t===== 选择专业 =====\n\n");
        setColor(COLOR_RESET);
        
        setColor(COLOR_CYAN);
        for (int i = 0; i < manager->majorCount; i++) {
            printf("\t\t%d. %s\n", i + 1, manager->majorNames[i]);
        }
        setColor(COLOR_RESET);
        
        printf("\t\t请选择专业 (1-%d): ", manager->majorCount);
        int majorChoice;
        result = scanf("%d", &majorChoice);
        clearInputBuffer();
        
        if (result != 1 || majorChoice < 1 || majorChoice > manager->majorCount) {
            setColor(COLOR_RED);
            printf("\t\t选择无效！\n");
            setColor(COLOR_RESET);
            printf("\t\t按任意键返回...");
            getKey();
            return;
        }
        
        // 显示选中专业的学生信息
        const char *selectedMajor = manager->majorNames[majorChoice - 1];
        clearScreen();
        
        setColor(COLOR_YELLOW);
        printf("\n\n\t\t===== %s 专业学生信息 =====\n\n", selectedMajor);
        setColor(COLOR_RESET);
        
        int count = 0;
        for (int i = 0; i < manager->count; i++) {
            if (strcmp(manager->students[i].major, selectedMajor) == 0) {
                count++;
                printf("\t\t学生 %d:\n", count);
                displayStudent(&manager->students[i]);
                printf("\t\t-----------------------------\n");
            }
        }
        
        if (count == 0) {
            setColor(COLOR_RED);
            printf("\t\t该专业暂无学生信息！\n");
            setColor(COLOR_RESET);
        } else {
            setColor(COLOR_CYAN);
            printf("\t\t共有 %d 名学生\n", count);
            setColor(COLOR_RESET);
        }
    }
    
    printf("\t\t按任意键返回...");
    getKey();
}

// 显示使用说明
void showInstructions() {
    clearScreen();
    setColor(COLOR_GREEN);
    printf("\n\n\t\t\t=======================================\n");
    printf("\t\t\t            使用说明            \n");
    printf("\t\t\t=======================================\n\n");
    setColor(COLOR_RESET);
    
    setColor(COLOR_CYAN);
    printf("\t\t【操作提示】\n");
    setColor(COLOR_YELLOW);
    printf("\t\t请注意：在录入学生信息之前，\n");
    printf("\t\t必须先通过 '7. 院系预设' 和 '8. 专业预设' 添加\n");
    printf("\t\t所需的院系和专业信息！\n\n");
    setColor(COLOR_CYAN);
    printf("\t\t否则将无法正常录入学生的院系和专业信息。\n\n");
    setColor(COLOR_YELLOW);
    printf("\t\t【重要提示】\n");
    setColor(COLOR_RED);
    printf("\t\t该软件目前仍处于开发中，尚未实现长期的数据保存功能，慎用！\n");
    printf("\t\t建议仅在测试环境中使用，正式环境请备份数据！\n");
    setColor(COLOR_RESET);
    
    printf("\t\t按任意键返回...");
    getKey();
}

// 显示开发者的话
void showDeveloperMessage() {
    clearScreen();
    setColor(COLOR_GREEN);
    printf("\n\n\t\t\t=======================================\n");
    printf("\t\t\t            开发者的话            \n");
    printf("\t\t\t=======================================\n\n");
    setColor(COLOR_RESET);
    
    setColor(COLOR_CYAN);

    printf("\t\t这个程序只是我用来交程序设计课的大作业的半成品\n");
    printf("\t\t老实说，大学的课程的确水的没边了\n");
    printf("\t\t往往一节网课的知识量，非要23节课才能上完\n");
    printf("\t\t还不如翘课去干点更有意义的事\n");
    printf("\t\t虽然只是一个作业，但我的确很好奇这个纯C语言程序到底有多大潜力\n");
    printf("\t\t我计划在未来的时间里，继续完善这个程序，\n");
    printf("\t\t添加更多的功能和优化使用体验。\n\n");
    printf("\t\t顺便说一下，我在这个项目中，使用了一些我之前没有接触过的知识，\n");
    printf("\t\t比如文件操作、动态内存分配等。\n");
    printf("\t\t在之后我也会尝试学着使用github的功能，\n");
    printf("\t\t来管理和维护项目的代码。\n");
    printf("\t\t至于很久之后是否还会再继续完善这个程序？\n");
    printf("\t\t这个程序是不是会变成一个大项目，变成需要多个语言来达成目的的项目？\n");
    printf("\t\t这我就无从得知了。\n");
    printf("\t\t毕竟说到底我只是一个网络工程专业的大一学生，\n");
    printf("\t\t我也没有什么经验，以后的方向也应该不是软件开发\n");
    printf("\t\t所以这个项目可能会有很多问题，\n");
    printf("\t\t也可能会有很多不足，\n");
    printf("\t\t但是我会尽我最大的努力，\n");
    printf("\t\t来完善这个项目，\n");
    printf("\t\t让它变成一个更好的项目。\n");
    printf("\t\t                                          ——MccoyFreyr\n");
    setColor(COLOR_YELLOW);
    printf("\t\t\t---------------------------------------\n");
    printf("\t\t\t                 特别鸣谢                \n");
    printf("\t\t\t---------------------------------------\n\n");
    setColor(COLOR_CYAN);
    printf("\t\t啊？原来还要有这个部分的吗？\n");
    printf("\t\t好吧，其实是我玩游戏时看到会加入特别鸣谢名单\n");
    printf("\t\t以此感谢那些对项目开发有贡献的人\n");
    printf("\t\t不过说实话,这个项目是我一人开发的\n");
    printf("\t\t没有人协助过我完成这个项目\n");
    printf("\t\t硬要说感谢谁，我想也只有我的父亲和一位朋友了吧\n");
    printf("\t\t感谢我的父亲含辛茹苦把我送上大学，能够学到这些知识\n");
    printf("\t\t也感谢我的朋友的建议和反馈，让我能够不断完善这个项目\n");
    printf("\t\t                                          ——MccoyFreyr\n\n");
    setColor(COLOR_RESET);
    
    printf("\t\t按任意键返回...");
    getKey();
}
  
// 显示软件信息函数
void showSoftwareInfo() {
    clearScreen();
    setColor(COLOR_GREEN);
    printf("\n\n\t\t\t=======================================\n");
    printf("\t\t\t            软件信息            \n");
    printf("\t\t\t=======================================\n\n");
    setColor(COLOR_RESET);
    
    setColor(COLOR_CYAN);
    printf("\t\t软件名称: %s\n", SOFTWARE_NAME);
    printf("\t\t开发版本: %s\n", SOFTWARE_VERSION_TEXT);
    printf("\t\t开发作者: %s\n", SOFTWARE_AUTHOR);
    printf("\t\t软件功能: 学生信息的录入、查找、修改、删除和展示，\n");
    printf("\t\t          院系专业预设管理，成绩管理\n");
    printf("\t\t开发环境: Windows + C语言\n");
    printf("\t\t系统要求: Windows 7及以上版本\n");
    printf("\t\t编译环境: GCC/MinGW\n\n");
    printf("\t\t软件状态: 开发版 (Development)\n");
    printf("\t\t版权所有: © %s %s\n", COPYRIGHT_YEAR, SOFTWARE_AUTHOR);
    printf("\t\t开源协议: %s\n", LICENSE_TYPE);
    printf("\t\t协议说明: 允许自由使用、修改和分发，保留原作者版权声明\n");
    setColor(COLOR_RESET);
    
    setColor(COLOR_CYAN);
    printf("\t\t======================================\n");
    printf("\t\t           软件开发历程           \n");
    printf("\t\t======================================\n");
    printf("\t\t2023-09-15: 项目初始规划与需求分析 (v0.1.0-dev)\n");
    printf("\t\t2023-09-20: 系统架构设计完成 (v0.1.1-dev)\n");
    printf("\t\t2023-09-25: 基础数据结构实现 (v0.1.2-dev)\n");
    printf("\t\t2023-10-05: 学生信息增删改查功能开发 (v0.1.3-dev)\n");
    printf("\t\t2023-10-15: 院系和专业预设功能实现 (v0.1.4-dev)\n");
    printf("\t\t2023-10-20: 成绩管理模块完成 (v0.1.5-dev)\n");
    printf("\t\t2023-10-25: 界面美化与用户体验优化 (v0.1.6-dev)\n");
    printf("\t\t2023-10-30: 系统测试与Bug修复 (v0.1.6.1-dev)\n");
    printf("\t\t2023-11-05: v0.1.7-dev版本发布\n\n");
    printf("\t\t后续开发计划：（v0.1.8-dev ~ v0.2.x-dev）\n");
    printf("\t\t1. 完善成绩管理模块，添加更多成绩项和统计功能\n");
    printf("\t\t2. 优化用户界面，提供更多操作选项和快捷键\n");
    printf("\t\t3. 增加数据存储功能，防止数据丢失\n");
    printf("\t\t4. 完善帮助文档，方便用户使用\n");
    printf("\t\t5. 计划在v0.2.0版本中提供完整的数据持久化支持\n");
    printf("\t\t======================================\n");
    setColor(COLOR_YELLOW);
    printf("\t\t感谢您使用本系统！\n");
    printf("\t\t如有问题或建议，欢迎反馈！\n\n");
    setColor(COLOR_RESET);
    
    printf("\t\t按任意键返回...");
    getKey();
}

// 显示成绩名预设列表
void displayScoreNames(StudentManager *manager) {
    int i;
    
    if (manager->scoreNameCount == 0) {
        setColor(COLOR_YELLOW);
        printf("\n当前没有预设的成绩名！\n");
        setColor(COLOR_RESET);
        return;
    }
    
    setColor(COLOR_CYAN);
    printf("\n当前预设的成绩名列表：\n");
    printf("------------------------------------------------------------\n");
    for (i = 0; i < manager->scoreNameCount; i++) {
        printf("\t%2d. %s\n", i + 1, manager->scoreNames[i]);
    }
    printf("------------------------------------------------------------\n");
    setColor(COLOR_RESET);
}

// 添加成绩名
void addScoreName(StudentManager *manager, const char *name) {
    if (manager->scoreNameCount >= manager->scoreNameCapacity) {
        // 扩容
        int newCapacity = manager->scoreNameCapacity * 2;
        char **newScoreNames = (char **)realloc(manager->scoreNames, newCapacity * sizeof(char *));
        if (newScoreNames == NULL) {
            setColor(COLOR_RED);
            printf("\n内存分配失败，无法添加成绩名！\n");
            setColor(COLOR_RESET);
            return;
        }
        manager->scoreNames = newScoreNames;
        manager->scoreNameCapacity = newCapacity;
    }
    
    // 分配空间并复制成绩名
    manager->scoreNames[manager->scoreNameCount] = (char *)malloc(strlen(name) + 1);
    if (manager->scoreNames[manager->scoreNameCount] == NULL) {
        setColor(COLOR_RED);
        printf("\n内存分配失败，无法添加成绩名！\n");
        setColor(COLOR_RESET);
        return;
    }
    
    strcpy(manager->scoreNames[manager->scoreNameCount], name);
    manager->scoreNameCount++;
    
    setColor(COLOR_GREEN);
    printf("\n成功添加成绩名：%s\n", name);
    setColor(COLOR_RESET);
}

// 清除所有成绩名预设
void clearScoreNames(StudentManager *manager) {
    int i;
    
    if (manager->scoreNames != NULL) {
        for (i = 0; i < manager->scoreNameCount; i++) {
            free(manager->scoreNames[i]);
        }
        manager->scoreNameCount = 0;
    }
    
    setColor(COLOR_GREEN);
    printf("\n所有成绩名预设已清除！\n");
    setColor(COLOR_RESET);
}

// 管理成绩名预设
void manageScoreNames(StudentManager *manager) {
    char choice;
    char scoreName[50];
    
    while (1) {
        clearScreen();
        setColor(COLOR_YELLOW);
        printf("============================================================\n");
        printf("*                     成绩名预设管理                        *\n");
        printf("============================================================\n");
        setColor(COLOR_RESET);
        
        // 显示当前预设的成绩名
        displayScoreNames(manager);
        
        setColor(COLOR_BLUE);
        printf("\n预设成绩名操作：\n");
        printf("1. 添加成绩名\n");
        printf("2. 清除所有成绩名预设\n");
        printf("0. 返回主菜单\n");
        setColor(COLOR_RESET);
        
        printf("\n请输入选择: ");
        choice = getKey();
        clearInputBuffer(); // 清除输入缓冲区
        
        switch (choice) {
            case '1':
                clearScreen();
                setColor(COLOR_YELLOW);
                printf("============================================================\n");
                printf("*                     添加成绩名预设                        *\n");
                printf("============================================================\n");
                setColor(COLOR_RESET);
                
                printf("请输入成绩名 (输入 'end' 结束添加): ");
                while (1) {
                    fgets(scoreName, sizeof(scoreName), stdin);
                    scoreName[strcspn(scoreName, "\n")] = '\0';
                    
                    if (strcmp(scoreName, "end") == 0) {
                        break;
                    }
                    
                    if (isEmptyString(scoreName)) {
                        setColor(COLOR_RED);
                        printf("成绩名不能为空，请重新输入: ");
                        setColor(COLOR_RESET);
                    } else {
                        addScoreName(manager, scoreName);
                        printf("请继续输入成绩名 (输入 'end' 结束添加): ");
                    }
                }
                break;
                
            case '2':
                setColor(COLOR_YELLOW);
                printf("\n确定要清除所有成绩名预设吗？(y/n): ");
                setColor(COLOR_RESET);
                choice = getKey();
                clearInputBuffer(); // 清除输入缓冲区
                if (choice == 'y' || choice == 'Y') {
                    clearScoreNames(manager);
                }
                break;
                
            case '0':
                return;
                
            default:
                setColor(COLOR_RED);
                printf("\n无效的选择，请重新输入！\n");
                setColor(COLOR_RESET);
                printf("按任意键继续...");
                getKey();
                clearInputBuffer(); // 清除输入缓冲区
                break;
        }
    }
}
