 #include "admin.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


// 科室名称映射（固定5个，与初始化一致）
static const char* dept_names[] = { "内科", "外科", "儿科", "妇产科", "男科" };
#define MAX_DEPT 5
char adminPassword[20] = "admin123";
// ------------------------- 管理员登录 -------------------------
void adminLogin(void) {
    char pwd[20];
    printf("\n========== 管理员登录 ==========\n");
    printf("请输入密码: ");
    scanf("%s", pwd);
    if (strcmp(pwd, adminPassword) != 0) {
        printf("密码错误！登录失败。\n");
        pause();
        return;          // 直接返回，无返回值
    }
    printf("登录成功！\n");
    pause();
    // 登录成功后调用管理员菜单
    adminMenu();
}
//管理员菜单
void adminMenu(void) {
    int op;
    do {
        clearScreen();
        printf("\n========== 管理员菜单 ==========\n");
        printf("1. 查看科室医生列表\n");
        printf("2. 删除医生\n");
        printf("3. 新增医生\n");
        printf("4. 修改医生科室\n");
        printf("5. 查看所有医生\n");
        printf("6. 查看全院床位状态\n");
        printf("7. 查看药品库存\n");
        printf("8. 药品入库\n");
        printf("9. 药品消耗统计\n");
        printf("10. 患者统计\n");
        printf("11. 财务统计\n");
        printf("12. 打印全部统计报表\n");
        printf("13. 修改密码\n");  
        printf("0. 退出登录\n");
        printf("请选择: ");
        scanf("%d", &op);
        switch (op) {
        case 1: query_all_departments(); break;
        case 2: {
            int id;
            printf("请输入要删除的医生ID: ");
            scanf("%d", &id);
            admin_delete_doctor(id);
            pause();
            break;
        }
        case 3: admin_add_doctor(); break;
        case 4: {
            int id, newDept;
            printf("请输入医生ID: "); scanf("%d", &id);
            printf("请输入新科室ID(1-5): "); scanf("%d", &newDept);
            modify_doctor_dept(id, newDept);
            pause();
            break;
        }
        case 5: query_all_doctors(); break;
        case 6: query_all_beds(); break;
        case 7: query_all_medicines(); break;
        case 8: {
            int medId, count;
            Date inDate;
            char batch[20];
            printf("请输入药品ID: "); scanf("%d", &medId);
            printf("请输入数量: "); scanf("%d", &count);
            printf("请输入入库日期(年 月 日): "); scanf("%d %d %d", &inDate.year, &inDate.month, &inDate.day);
            printf("请输入批号: "); scanf("%s", batch);
            medicine_stock_in(medId, count, inDate, batch);
            pause();
            break;
        }
        case 9: stat_medicine_consume(); break;
        case 10: stat_patients(); pause(); break;
        case 11: stat_finance(); pause(); break;
        case 12: print_all_stats(); break;
        case 13: changeAdminPassword(); break;
        case 0: break;
        default: printf("无效选择！\n"); pause();
        }
    } while (op != 0);
}
void changeAdminPassword(void) {
    char old[20], new1[20], new2[20];
    printf("\n========== 修改管理员密码 ==========\n");
    printf("请输入原密码: ");
    scanf("%s", old);
    if (strcmp(old, adminPassword) != 0) {
        printf("原密码错误！\n");
        pause();
        return;
    }
    printf("请输入新密码: ");
    scanf("%s", new1);
    printf("请再次输入新密码: ");
    scanf("%s", new2);
    if (strcmp(new1, new2) != 0) {
        printf("两次密码不一致！\n");
        pause();
        return;
    }
    strcpy(adminPassword, new1);
    printf("密码修改成功！\n");
    pause();
}
// ------------------------- 科室管理 -------------------------
void query_all_departments(void) {
    printf("\n========== 全院科室及医生列表 ==========\n");
    for (int i = 0; i < MAX_DEPT; i++) {
        printf("【%s】（ID:%d）\n", dept_names[i], i + 1);
        printf("  医生列表: ");
        int has = 0;
        DoctorNode* cur = doctorHead;
        while (cur) {
            if (cur->data.deptId == i + 1) {
                printf("%s(ID:%d) ", cur->data.name, cur->data.id);
                has = 1;
            }
            cur = cur->next;
        }
        if (!has) printf("无");
        printf("\n");
    }
    pause();
}

// ------------------------- 医生管理 -------------------------
bool admin_add_doctor(void) {
    if (doctorCount >= MAX_DOCTORS) {
        printf("医生数量已达上限（%d）！\n", MAX_DOCTORS);
        return false;
    }
    Doctor d;
    printf("\n========== 新增医生 ==========\n");
    printf("医生ID: ");
    scanf("%d", &d.id);
    // 检查ID是否重复
    if (findDoctorById(d.id) != NULL) {
        printf("ID已存在！\n");
        return false;
    }
    printf("姓名: ");
    scanf("%s", d.name);
    printf("所属科室ID（1-5，1内科 2外科 3儿科 4妇产科 5男科）: ");
    scanf("%d", &d.deptId);
    if (d.deptId < 1 || d.deptId > 5) {
        printf("科室ID无效！\n");
        return false;
    }
    d.registerFee = 10.0f;      // 默认挂号费
    d.todayVisits = 0;
    d.totalVisits = 0;
    d.currentPatients = 0;
    d.maxPatients = 30;
    d.currentQueueNumber = 0;
    d.lastCalledId = 0;
    addDoctorNode(d);
    printf("医生 %s 添加成功！\n", d.name);
    return true;
}

bool admin_delete_doctor(int doctor_id) {
    // 1. 检查医生是否存在
    Doctor* d = findDoctorById(doctor_id);
    if (!d) {
        printf("医生ID %d 不存在！\n", doctor_id);
        return false;
    }

    // 2. 可选检查：该医生是否有未完成的挂号（待就诊或接诊中）
    RegNode* rn = regHead;
    while (rn) {
        if (rn->data.doctorId == doctor_id &&
            (rn->data.status == REG_STATUS_REGISTERED || rn->data.status == REG_STATUS_IN_DIAGNOSIS)) {
            printf("该医生尚有未完成的挂号记录，请先处理后重试！\n");
            return false;
        }
        rn = rn->next;
    }

    // 3. 从链表中删除医生节点
    DoctorNode* prev = NULL;
    DoctorNode* curr = doctorHead;
    while (curr) {
        if (curr->data.id == doctor_id) {
            if (prev) prev->next = curr->next;
            else doctorHead = curr->next;
            free(curr);
            doctorCount--;
            printf("医生ID %d 删除成功！\n", doctor_id);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}
bool modify_doctor_dept(int doctor_id, int new_deptId) {
    Doctor* d = findDoctorById(doctor_id);
    if (!d) {
        printf("医生ID %d 不存在！\n", doctor_id);
        return false;
    }
    if (new_deptId < 1 || new_deptId > 5) {
        printf("科室ID无效（1-5）！\n");
        return false;
    }
    d->deptId = new_deptId;
    printf("医生 %s 科室已修改为 %s\n", d->name, dept_names[new_deptId - 1]);
    return true;
}

void query_all_doctors(void) {
    printf("\n========== 全院医生信息 ==========\n");
    printf("ID\t姓名\t科室\t\t今日接诊\t总接诊\n");
    DoctorNode* cur = doctorHead;
    while (cur) {
        printf("%d\t%s\t%s\t\t%d\t\t%d\n",
            cur->data.id,
            cur->data.name,
            dept_names[cur->data.deptId - 1],
            cur->data.todayVisits,
            cur->data.totalVisits);
        cur = cur->next;
    }
    pause();
}

// ------------------------- 床位管理 -------------------------
void query_all_beds(void) {
    printf("\n========== 全院床位状态 ==========\n");
    WardNode* cur = wardHead;
    int total_occ = 0, total_free = 0;
    while (cur) {
        Ward* w = &cur->data;
        printf("【%s】（ID:%d 日费:%.2f元）\n", w->name, w->id, w->pricePerDay);
        int occ = 0;
        for (int i = 0; i < BEDS_PER_WARD; i++) {
            if (w->beds[i] > 0) {
                occ++;
                Patient* p = findPatientById(w->beds[i]);
                printf("  床位%d: 已占用 - 患者 %s(ID:%d)\n", i + 1, p ? p->name : "未知", w->beds[i]);
            }
            else {
                printf("  床位%d: 空闲\n", i + 1);
            }
        }
        printf("  本病房占用:%d 空闲:%d\n", occ, BEDS_PER_WARD - occ);
        total_occ += occ;
        total_free += (BEDS_PER_WARD - occ);
        cur = cur->next;
    }
    printf("\n全院总计: 占用 %d 床, 空闲 %d 床\n", total_occ, total_free);
    pause();
}

bool modify_bed_status(int wardId, int bedNum, bool occupy, int patientId, int doctorId) {
    // 查找病房
    Ward* ward = findWardById(wardId);
    if (!ward) {
        printf("病房ID %d 不存在！\n", wardId);
        return false;
    }
    if (bedNum < 1 || bedNum > BEDS_PER_WARD) {
        printf("床位号无效（1-%d）！\n", BEDS_PER_WARD);
        return false;
    }
    int idx = bedNum - 1;
    if (occupy) {
        if (ward->beds[idx] > 0) {
            printf("床位已被占用！\n");
            return false;
        }
        ward->beds[idx] = patientId;
        ward->occupiedBeds++;
        // 更新患者住院信息
        Patient* p = findPatientById(patientId);
        if (p) {
            p->type = 1;   // 住院
            p->wardId = wardId;
            p->bedId = bedNum;
            p->currentDoctorId = doctorId;
        }
        printf("床位已分配，患者ID %d 入住成功。\n", patientId);
    }
    else {
        if (ward->beds[idx] == 0) {
            printf("床位本来空闲！\n");
            return false;
        }
        int pid = ward->beds[idx];
        ward->beds[idx] = 0;
        ward->occupiedBeds--;
        // 患者出院
        Patient* p = findPatientById(pid);
        if (p) {
            p->type = 0;
            p->wardId = -1;
            p->bedId = -1;
        }
        printf("床位已释放。\n");
    }
    return true;
}

// ------------------------- 药品管理 -------------------------
void query_all_medicines(void) {
    printf("\n========== 全院药品库存 ==========\n");
    printf("ID\t名称\t\t库存\t单价\t关联科室\n");
    MedicineNode* cur = medicineHead;
    while (cur) {
        Medicine* m = &cur->data;
        char dept[20];
        if (m->deptId == 0) strcpy(dept, "通用");
        else strcpy(dept, dept_names[m->deptId - 1]);
        printf("%d\t%-12s\t%d\t%.2f\t%s\n", m->id, m->name, m->stock, m->price, dept);
        cur = cur->next;
    }
    pause();
}

bool medicine_stock_in(int med_id, int count, Date in_date, const char* batch) {
    if (count <= 0) {
        printf("入库数量必须为正数！\n");
        return false;
    }
    Medicine* m = findMedicineById(med_id);
    if (!m) {
        printf("药品ID %d 不存在！\n", med_id);
        return false;
    }
    m->stock += count;
    m->inDate = in_date;
    strncpy(m->batchNo, batch, 19);
    m->batchNo[19] = '\0';
    printf("药品 %s 入库成功，当前库存: %d\n", m->name, m->stock);
    return true;
}

void stat_medicine_consume(void) {
    // 简单统计：初始库存20 - 当前库存 = 消耗（假设初始20）
    printf("\n========== 药品消耗统计 ==========\n");
    printf("ID\t名称\t\t初始库存\t当前库存\t消耗数量\n");
    MedicineNode* cur = medicineHead;
    while (cur) {
        Medicine* m = &cur->data;
        int init = 20;   // 初始统一为20
        int consumed = init - m->stock;
        printf("%d\t%-12s\t%d\t\t%d\t\t%d\n", m->id, m->name, init, m->stock, consumed);
        cur = cur->next;
    }
    pause();
}

// ------------------------- 统计功能 -------------------------
void stat_patients(void) {
    int total = patientCount;
    int outpatient = 0, inpatient = 0, elderly = 0;
    PatientNode* cur = patientHead;
    while (cur) {
        Patient* p = &cur->data;
        if (p->type == 1) inpatient++;
        else outpatient++;
        if (p->age >= 60) elderly++;
        cur = cur->next;
    }

    // 打印汇总统计
    printf("\n========== 患者统计(汇总) ==========\n");
    printf("总患者数: %d\n", total);
    printf("门诊患者: %d (%.1f%%)\n", outpatient, total ? (float)outpatient / total * 100 : 0);
    printf("住院患者: %d (%.1f%%)\n", inpatient, total ? (float)inpatient / total * 100 : 0);
    printf("60岁以上老人: %d (%.1f%%)\n", elderly, total ? (float)elderly / total * 100 : 0);

    // 打印所有患者详细信息
    printf("\n========== 患者详细信息列表 ==========\n");
    printf("ID\t姓名\t年龄\t性别\t电话\t\t累计费用\t状态\n");
    cur = patientHead;
    while (cur) {
        Patient* p = &cur->data;
        char* statusStr;
        switch (p->status) {
        case PATIENT_STATUS_NOT_REGISTERED: statusStr = "未挂号"; break;
        case PATIENT_STATUS_WAITING:        statusStr = "等待接诊"; break;
        case PATIENT_STATUS_IN_DIAGNOSIS:   statusStr = "接诊中"; break;
        case PATIENT_STATUS_WAITING_PAY:    statusStr = "待缴费"; break;
        case PATIENT_STATUS_IN_HOSPITAL:    statusStr = "住院中"; break;
        case PATIENT_STATUS_DISCHARGED:     statusStr = "已出院"; break;
        default: statusStr = "未知";
        }
        // 格式化输出，电话号码占15个字符宽度（左对齐）
        printf("%d\t%s\t%d\t%c\t%-15s\t%.2f\t%s\n",
            p->id, p->name, p->age, p->gender, p->phone, p->totalCost, statusStr);
        cur = cur->next;
    }
    pause();
}

void stat_finance(void) {
    double total_fee = 0, total_self = 0, total_medicare = 0;
    PatientNode* cur = patientHead;
    while (cur) {
        Patient* p = &cur->data;
        total_fee += p->totalCost;
        // 医保报销: 60岁以上50%
        double medicare = (p->age >= 60) ? p->totalCost * 0.5 : 0;
        total_medicare += medicare;
        total_self += p->totalCost - medicare;
        cur = cur->next;
    }
    printf("\n========== 财务统计 ==========\n");
    printf("总费用: %.2f 元\n", total_fee);
    printf("医保报销总额: %.2f 元 (%.1f%%)\n", total_medicare, total_fee ? total_medicare / total_fee * 100 : 0);
    printf("患者自付总额: %.2f 元\n", total_self);
}

void print_all_stats(void) {
    printf("\n==================== 全院统计报表 ====================\n");
    stat_patients();
    stat_finance();
    query_all_beds();      // 复用床位显示
    stat_medicine_consume();
    printf("======================================================\n");
    pause();
}