#include "utils.h"
#include <windows.h>
#include <stdlib.h>

void clearScreen(void) {
    system("cls");
}

void pause(void) {
    printf("\n按任意键继续...");
    getchar();
    getchar();
}

int getIntInput(const char* prompt) {
    int value;
    char term;
    while (1) {
        printf("%s", prompt);   // 显示提示信息
        if (scanf("%d", &value) == 1) {
            // 成功读取整数，检查后面是否为换行符（防止输入如 "123abc"）
            if ((term = getchar()) == '\n') {
                break;
            }
            else {
                // 有多余字符，清空到行尾
                while (getchar() != '\n');
                printf("输入无效，请输入纯数字。\n");
            }
        }
        else {
            // 输入非数字，清空缓冲区并提示
            while (getchar() != '\n');
            printf("输入无效，请输入数字。\n");
        }
    }
    return value;
}
void addMedicalRecordNode(MedicalRecord mr) {
    RecordNode* newNode = (RecordNode*)malloc(sizeof(RecordNode));
    newNode->data = mr;
    newNode->next = recordHead;
    recordHead = newNode;
    recordCount++;
}

Date getCurrentDate(void) {
    Date today;
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    today.year = tm->tm_year + 1900;
    today.month = tm->tm_mon + 1;
    today.day = tm->tm_mday;
    return today;
}

int dateDiff(Date start, Date end) {
    return (end.year - start.year) * 365 +
        (end.month - start.month) * 30 +
        (end.day - start.day);
}

void addPatientNode(Patient p) {    // 添加节点函数
    PatientNode* newNode = (PatientNode*)malloc(sizeof(PatientNode));
    newNode->data = p;
    newNode->next = patientHead;
    patientHead = newNode;
    patientCount++;
}

void addDoctorNode(Doctor d) {
    DoctorNode* newNode = (DoctorNode*)malloc(sizeof(DoctorNode));
    newNode->data = d;
    newNode->next = NULL;
    if (doctorHead == NULL) {
        doctorHead = newNode;
    }
    else {
        DoctorNode* cur = doctorHead;
        while (cur->next) cur = cur->next;
        cur->next = newNode;
    }
    doctorCount++;
}

// 删除节点函数
void deletePatientNode(int id) {
    PatientNode* prev = NULL;
    PatientNode* curr = patientHead;
    while (curr) {
        if (curr->data.id == id) {
            if (prev) prev->next = curr->next;
            else patientHead = curr->next;
            free(curr);
            patientCount--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void deleteDoctorNode(int id) {
    DoctorNode* prev = NULL;
    DoctorNode* curr = doctorHead;
    while (curr) {
        if (curr->data.id == id) {
            if (prev) prev->next = curr->next;
            else doctorHead = curr->next;
            free(curr);
            doctorCount--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
Patient* findPatientById(int id) {    // 查找函数（返回指针）
    PatientNode* p = patientHead;
    while (p) {
        if (p->data.id == id) return &p->data;
        p = p->next;
    }
    return NULL;
}

Doctor* findDoctorById(int id) {
    DoctorNode* d = doctorHead;
    while (d) {
        if (d->data.id == id) return &d->data;
        d = d->next;
    }
    return NULL;
}

Department* findDeptById(int id) {
    DeptNode* d = deptHead;
    while (d) {
        if (d->data.id == id) return &d->data;
        d = d->next;
    }
    return NULL;
}
void updatePatientCostById(int patientId, float cost) {    // 更新患者费用
    Patient* p = findPatientById(patientId);
    if (p) {
        p->totalCost += cost;
        if (p->totalCost > 1000) p->totalCost = 1000;
    }
}

void updateDoctorWorkloadById(int doctorId, int delta) {
    Doctor* d = findDoctorById(doctorId);
    if (d) {
        d->currentPatients += delta;
        if (d->currentPatients < 0) d->currentPatients = 0;
    }
}

void updateDeptVisitsById(int deptId) {
    Department* dept = findDeptById(deptId);
    if (dept) {
        dept->todayVisits++;
    }
}

void addExamRecordNode(ExamRecord er) {
    ExamRecordNode* newNode = (ExamRecordNode*)malloc(sizeof(ExamRecordNode));
    newNode->data = er;
    newNode->next = examRecordHead;
    examRecordHead = newNode;
    examRecordCount++;
}

void addTestRecordNode(TestRecord tr) {
    TestRecordNode* newNode = (TestRecordNode*)malloc(sizeof(TestRecordNode));
    newNode->data = tr;
    newNode->next = testRecordHead;
    testRecordHead = newNode;
    testRecordCount++;
}
Ward* findWardById(int id) {
    WardNode* w = wardHead; while (w) {
        if (w->data.id == id) return &w->data; w = w->next;
    }
    return NULL;
}
// 查找检查项目
ExamItem* findExamItemById(int id) {
    ExamItemNode* e = examItemHead;
    while (e) {
        if (e->data.id == id) return &e->data;
        e = e->next;
    }
    return NULL;
}

// 查找检验项目
TestItem* findTestItemById(int id) {
    TestItemNode* t = testItemHead;
    while (t) {
        if (t->data.id == id) return &t->data;
        t = t->next;
    }
    return NULL;
}
Medicine* findMedicineById(int id) {
    MedicineNode* m = medicineHead;
    while (m) {
        if (m->data.id == id) return &m->data;
        m = m->next;
    }
    return NULL;
}
void addDepartmentNode(Department d) {
    DeptNode* newNode = (DeptNode*)malloc(sizeof(DeptNode));
    newNode->data = d;
    newNode->next = NULL;
    if (deptHead == NULL) {
        deptHead = newNode;
    }
    else {
        DeptNode* cur = deptHead;
        while (cur->next) cur = cur->next;
        cur->next = newNode;
    }
    deptCount++;
}

void addWardNode(Ward w) {
    WardNode* newNode = (WardNode*)malloc(sizeof(WardNode));
    newNode->data = w;
    newNode->next = wardHead;
    wardHead = newNode;
    wardCount++;
}

void addMedicineNode(Medicine m) {
    MedicineNode* newNode = (MedicineNode*)malloc(sizeof(MedicineNode));
    newNode->data = m;
    newNode->next = medicineHead;
    medicineHead = newNode;
    medicineCount++;
}

void addExamItemNode(ExamItem e) {
    ExamItemNode* newNode = (ExamItemNode*)malloc(sizeof(ExamItemNode));
    newNode->data = e;
    newNode->next = examItemHead;
    examItemHead = newNode;
    examItemCount++;
}

void addTestItemNode(TestItem t) {
    TestItemNode* newNode = (TestItemNode*)malloc(sizeof(TestItemNode));
    newNode->data = t;
    newNode->next = testItemHead;
    testItemHead = newNode;
    testItemCount++;
}

void addRegistrationNode(Registration r) {
    RegNode* newNode = (RegNode*)malloc(sizeof(RegNode));
    newNode->data = r;
    newNode->next = regHead;
    regHead = newNode;
    regCount++;
}

void addFollowUpNode(FollowUpReminder f) {
    FollowUpNode* newNode = (FollowUpNode*)malloc(sizeof(FollowUpNode));
    newNode->data = f;
    newNode->next = followUpHead;
    followUpHead = newNode;
    followUpCount++;
}

void addDailyStatNode(DailyStatistics ds) {
    DailyStatNode* newNode = (DailyStatNode*)malloc(sizeof(DailyStatNode));
    newNode->data = ds;
    newNode->next = dailyStatHead;
    dailyStatHead = newNode;
    dailyStatCount++;
}
