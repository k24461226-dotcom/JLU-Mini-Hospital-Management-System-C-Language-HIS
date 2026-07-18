#pragma once
#ifndef PATIENT_H
#define PATIENT_H

#include "data.h"

// 患者登录入口
void patientLogin(void);

// 患者主菜单
void patientMenu(int patientId);

// 挂号（支持现场/预约）
void registerVisit(int patientId);

// 显示个人信息
void showPersonalInfo(int patientId);

// 显示医疗记录（挂号、看诊、检查、住院）
void showMedicalRecords(int patientId);

// 显示住院信息（病房、床位、主治医生）
void showHospitalInfo(int patientId);

// 显示费用明细并计算医保报销
void showCostDetail(int patientId);

// 医保报销计算（60岁以上50%）
double applyMedicare(int age, double totalCost);

// 查询排队叫号状态
void checkQueueStatus(int patientId);
//密码相关
void patientRegister(void);
void changePatientPassword(int patientId);
//退号
void cancelRegistration(int patientId);

#endif