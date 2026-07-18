#ifndef DOCTOR_H
#define DOCTOR_H

#include "data.h"   // 使用 Doctor、Patient 等结构体

// 医生登录和菜单
void doctorLogin(void);
void doctorMenu(Doctor* d);

// 排班和队列
void viewTodaySchedule(Doctor* d);
void viewWaitingQueue(Doctor* d);

// 叫号和接诊核心功能
void startDiagnosis(Doctor* d);
void callNextPatient(Doctor* d);
void performDiagnosis(Doctor* d, int patientId);

// 接诊操作
void prescribeMedicine(int patientId, int doctorId);
void requestExam(int patientId, int doctorId);
void requestTest(int patientId, int doctorId);
void admitPatient(int patientId, int doctorId);
void completeDiagnosis(Doctor* d, Patient* p);

// 患者管理
void viewMyPatients(Doctor* d);
void viewMedicalHistory(Doctor* d);
void setMaxPatients(Doctor* d);
// 检查/检验结果录入
void enterExamResult(Doctor* d);
void enterTestResult(Doctor* d);
void setFollowUpReminder(Doctor* d, int patientId);
//出院
void dischargePatient(Doctor* d);
//修改密码
void changeDoctorPassword(Doctor* d);
#endif
