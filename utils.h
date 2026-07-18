#ifndef UTILS_H
#define UTILS_H

#include "data.h"

// 清屏、暂停
void clearScreen(void);
void pause(void);

// 通用的添加节点（头插法）
void addPatientNode(Patient p);
void addDoctorNode(Doctor d);

// 通用的删除节点（按id）
void deletePatientNode(int id);
void deleteDoctorNode(int id);

// 通用的查找节点（返回数据指针）
Patient* findPatientById(int id);
Doctor* findDoctorById(int id);

// 日期函数
Date getCurrentDate(void);
int dateDiff(Date start, Date end);

Department* findDeptById(int id);
Medicine* findMedicineById(int id);
Ward* findWardById(int id);
ExamItem* findExamItemById(int id);
TestItem* findTestItemById(int id);

// 更新函数
void updatePatientCostById(int patientId, float cost);
void updateDoctorWorkloadById(int doctorId, int delta);
void updateDeptVisitsById(int deptId);
int getIntInput(const char* prompt);
void addDepartmentNode(Department d);
void addWardNode(Ward w);
void addMedicineNode(Medicine m);
void addExamItemNode(ExamItem e);
void addTestItemNode(TestItem t);
void addRegistrationNode(Registration r);
void addFollowUpNode(FollowUpReminder f);
void addDailyStatNode(DailyStatistics ds);
void addMedicalRecordNode(MedicalRecord mr);
void addExamRecordNode(ExamRecord er);
void addTestRecordNode(TestRecord tr);

#endif
