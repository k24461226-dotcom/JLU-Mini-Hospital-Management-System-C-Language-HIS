#ifndef ADMIN_H
#define ADMIN_H

#include "data.h"
#include<stdbool.h>

// 管理员登录（固定密码，可自行修改）
void adminLogin(void);
//菜单
void adminMenu(void);
// ========== 科室管理 ==========
void query_all_departments(void);   // 查看所有科室及对应医生

// ========== 医生管理 ==========
bool admin_add_doctor(void);        // 新增医生
bool modify_doctor_dept(int doctor_id, int new_deptId); // 修改医生科室
void query_all_doctors(void);       // 查看所有医生信息

// ========== 床位管理 ==========
void query_all_beds(void);          // 查看全院床位状态
bool modify_bed_status(int wardId, int bedNum, bool occupy, int patientId, int doctorId);

// ========== 药品管理 ==========
void query_all_medicines(void);     // 查看所有药品库存
bool medicine_stock_in(int med_id, int count, Date in_date, const char* batch);
void stat_medicine_consume(void);   // 药品消耗统计（可选实现）

// ========== 全院统计 ==========
void stat_patients(void);           // 患者统计
void stat_finance(void);            // 财务统计
void print_all_stats(void);         // 打印全部统计报表
// 删除医生
bool admin_delete_doctor(int doctor_id);
//password
void changeAdminPassword(void);

#endif
