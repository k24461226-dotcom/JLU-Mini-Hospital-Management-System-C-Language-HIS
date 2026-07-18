#include "patient.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================= 辅助静态函数 ========================= */

// 添加挂号记录到链表（头插法）
static void addRegistration(Registration reg) {
    RegNode* newNode = (RegNode*)malloc(sizeof(RegNode));
    if (!newNode) {
        printf("内存分配失败！\n");
        return;
    }
    newNode->data = reg;
    newNode->next = regHead;
    regHead = newNode;
    regCount++;
}

// 获取指定医生的下一个排队序号（遍历现有挂号记录，取最大序号+1）
static int getNextQueueNumber(int doctorId) {
    int max = 0;
    RegNode* cur = regHead;
    while (cur) {
        if (cur->data.doctorId == doctorId && cur->data.queueNumber > max) {
            max = cur->data.queueNumber;
        }
        cur = cur->next;
    }
    return max + 1;
}

// 检查患者是否有过挂号记录
static int hasRegisteredBefore(int patientId) {
    RegNode* cur = regHead;
    while (cur) {
        if (cur->data.patientId == patientId) {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/* ========================= 公共函数实现 ========================= */

// 患者登录
void patientLogin(void) {
    int id;
    char pwd[20];
    printf("\n===== 患者登录 =====\n");
    printf("请输入患者ID: ");
    if (scanf("%d", &id) != 1) {
        printf("输入无效！\n");
        pause();
        return;
    }
    printf("请输入密码: ");
    scanf("%s", pwd);

    Patient* p = findPatientById(id);
    if (!p || strcmp(p->password, pwd) != 0) {
        printf("患者ID或密码错误！\n");
        pause();
        return;
    }
    printf("登录成功！\n");
    pause();
    patientMenu(id);
}
//患者注册
void patientRegister(void) {
    Patient p;
    char pwd1[20], pwd2[20];

    printf("\n===== 患者注册 =====\n");
    // 简单分配 ID（取当前最大ID+1）
    int maxId = 0;
    PatientNode* cur = patientHead;
    while (cur) {
        if (cur->data.id > maxId) maxId = cur->data.id;
        cur = cur->next;
    }
    p.id = maxId + 1;
    printf("您的患者ID将自动分配为: %d\n", p.id);
    printf("请设置密码: ");
    scanf("%s", pwd1);
    printf("请确认密码: ");
    scanf("%s", pwd2);
    if (strcmp(pwd1, pwd2) != 0) {
        printf("两次密码不一致！注册失败。\n");
        pause();
        return;
    }
    strcpy(p.password, pwd1);

    printf("请输入姓名: ");
    scanf("%s", p.name);
    printf("请输入年龄: ");
    scanf("%d", &p.age);
    printf("请输入性别(M/F): ");
    scanf(" %c", &p.gender);
    printf("请输入电话: ");
    scanf("%s", p.phone);

    // 其他字段初始化
    p.type = 0;                     // 门诊
    p.wardId = -1;
    p.bedId = -1;
    p.admitDate = (Date){ 0,0,0 };
    p.dischargeDate = (Date){ 0,0,0 };
    p.totalCost = 0;
    p.paid = 0;
    p.status = PATIENT_STATUS_NOT_REGISTERED;
    p.currentDoctorId = -1;
    p.currentDeptId = -1;

    addPatientNode(p);
    printf("注册成功！请牢记您的患者ID: %d 和密码。\n", p.id);
    pause();
}
//修改密码
void changePatientPassword(int patientId) {
    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者不存在！\n");
        pause();
        return;
    }
    char old[20], new1[20], new2[20];
    printf("===== 修改密码 =====\n");
    printf("请输入原密码: ");
    scanf("%s", old);
    if (strcmp(p->password, old) != 0) {
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
    strcpy(p->password, new1);
    printf("密码修改成功！\n");
    pause();
}

// 挂号（现场/预约）
void registerVisit(int patientId) {
    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者ID %d 不存在！\n", patientId);
        pause();
        return;
    }

    // 状态检查
    if (p->status == PATIENT_STATUS_WAITING ||
        p->status == PATIENT_STATUS_IN_DIAGNOSIS ||
        p->status == PATIENT_STATUS_WAITING_PAY) {
        printf("您已有未完成的挂号或就诊，请先完成当前流程。\n");
        pause();
        return;
    }
    if (p->status == PATIENT_STATUS_IN_HOSPITAL) {
        printf("您正在住院中，不能挂号门诊。\n");
        pause();
        return;
    }
    // 选择挂号类型
    int regType;
    printf("\n请选择挂号类型：\n");
    printf("1. 现场挂号（今日就诊）\n");
    printf("2. 预约挂号（预约未来日期）\n");
    printf("请选择（1-2）: ");
    if (scanf("%d", &regType) != 1 || (regType != 1 && regType != 2)) {
        printf("输入无效！请输入1或2。\n");
        while (getchar() != '\n');
        pause();
        return;
    }

    // 确定就诊日期
    Date visitDate;
    if (regType == 2) {
        printf("请输入预约就诊日期（年 月 日）: ");
        scanf("%d %d %d", &visitDate.year, &visitDate.month, &visitDate.day);
        // 可选：简单校验日期合理性（月份1-12，日1-31）
        if (visitDate.month < 1 || visitDate.month > 12 || visitDate.day < 1 || visitDate.day > 31) {
            printf("日期无效！\n");
            pause();
            return;
        }
        printf("预约就诊日期: %d-%d-%d\n", visitDate.year, visitDate.month, visitDate.day);
    }
    else {
        // 现场挂号：使用当前日期
        visitDate = getCurrentDate();
    }

    // 显示科室列表
    printf("\n=== 科室列表 ===\n");
    DeptNode* dcur = deptHead;
    if (!dcur) {
        printf("暂无科室信息，请联系管理员。\n");
        pause();
        return;
    }
    while (dcur) {
        printf("%d. %s\n", dcur->data.id, dcur->data.name);
        dcur = dcur->next;
    }
    int deptId;
    printf("请选择科室ID: ");
    if (scanf("%d", &deptId) != 1) {
        printf("输入无效！\n");
        while (getchar() != '\n');
        pause();
        return;
    }
    Department* dept = findDeptById(deptId);
    if (!dept) {
        printf("无效科室ID，请重新选择。\n");
        pause();
        return;
    }

    // 显示该科室下的医生
    printf("\n=== %s 医生列表 ===\n", dept->name);
    DoctorNode* docCur = doctorHead;
    int hasDoctor = 0;
    while (docCur) {
        if (docCur->data.deptId == deptId) {
            hasDoctor = 1;
            printf("%d. %s (当前叫号: %d, 今日接诊: %d)\n",
                docCur->data.id, docCur->data.name,
                docCur->data.currentQueueNumber, docCur->data.todayVisits);
        }
        docCur = docCur->next;
    }
    if (!hasDoctor) {
        printf("该科室暂无医生，请稍后再试。\n");
        pause();
        return;
    }

    int docId;
    printf("请选择医生ID: ");
    if (scanf("%d", &docId) != 1) {
        printf("输入无效！\n");
        while (getchar() != '\n');
        pause();
        return;
    }
    Doctor* doc = findDoctorById(docId);
    if (!doc || doc->deptId != deptId) {
        printf("医生不存在或不属于该科室，请重新选择。\n");
        pause();
        return;
    }

    // 创建挂号记录
    Registration newReg;
    newReg.id = regCount + 1;
    newReg.patientId = patientId;
    newReg.doctorId = docId;
    newReg.deptId = deptId;
    newReg.regDate = visitDate;
    newReg.timeSlot = 1;
    newReg.type = (regType == 2) ? 1 : 2;  // 1线上(预约) 2线下(现场)
    newReg.status = 1;
    newReg.queueNumber = getNextQueueNumber(docId);
    newReg.fee = 10.0f;
    if (p->age > 60) {
        newReg.reimbursement = newReg.fee * 0.5f;
        newReg.actualFee = newReg.fee - newReg.reimbursement;
    }
    else {
        newReg.reimbursement = 0;
        newReg.actualFee = newReg.fee;
    }

    addRegistration(newReg);

    // 更新患者状态
    p->status = PATIENT_STATUS_WAITING;
    p->currentDoctorId = docId;
    p->currentDeptId = deptId;

    // 添加医疗记录（挂号）
    MedicalRecord rec;
    rec.id = recordCount + 1;
    rec.patientId = patientId;
    rec.doctorId = docId;
    rec.recordType = 1;
    rec.recordDate = getCurrentDate();
    snprintf(rec.detail, sizeof(rec.detail),
        "%s 挂号到医生 %s (就诊日期: %d-%d-%d, 排队号: %d)",
        (regType == 2 ? "预约" : "现场"),
        doc->name,
        visitDate.year, visitDate.month, visitDate.day,
        newReg.queueNumber);
    rec.cost = newReg.actualFee;
    rec.status = 1;
    addMedicalRecordNode(rec);

    // 输出结果
    if (regType == 1) {
        printf("\n现场挂号成功！排队号: %d\n", newReg.queueNumber);
        int waitMinutes = (newReg.queueNumber - doc->currentQueueNumber) * 15;
        if (waitMinutes < 0) waitMinutes = 0;
        printf("前面还有 %d 人，预计等待 %d 分钟。\n",
            newReg.queueNumber - doc->currentQueueNumber, waitMinutes);
    }
    else {
        printf("\n预约挂号成功！排队号: %d\n", newReg.queueNumber);
        printf("请于 %d-%d-%d 就诊当天提前10分钟到医院报到。\n",
            visitDate.year, visitDate.month, visitDate.day);
    }
    pause();
}
//退号
void cancelRegistration(int patientId) {
    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者不存在！\n");
        pause();
        return;
    }

    // 查找该患者最新且状态为“已挂号”的挂号记录
    RegNode* cur = regHead;
    Registration* targetReg = NULL;
    while (cur) {
        if (cur->data.patientId == patientId && cur->data.status == REG_STATUS_REGISTERED) {
            if (!targetReg || cur->data.id > targetReg->id) {
                targetReg = &cur->data;
            }
        }
        cur = cur->next;
    }

    if (!targetReg) {
        printf("没有可退号的挂号记录！\n");
        pause();
        return;
    }

    // 退号处理
    targetReg->status = REG_STATUS_CANCELED;

    // 退费（实际支付金额退还给患者，扣5元手续费）
    float refund = targetReg->actualFee;
    if (targetReg->type == REG_TYPE_ONLINE) {
        refund -= 5.0f;
        printf("线上挂号退号扣除5元手续费。\n");
    }
    p->paid -= refund;
    if (p->paid < 0) p->paid = 0;

    // 释放医生工作量
    updateDoctorWorkloadById(targetReg->doctorId, -1);

    // 如果患者状态是等待接诊，恢复为未挂号
    if (p->status == PATIENT_STATUS_WAITING) {
        p->status = PATIENT_STATUS_NOT_REGISTERED;
    }

    printf("退号成功！退款金额：%.2f元\n", refund);
    pause();
}

// 显示个人信息
void showPersonalInfo(int patientId) {
    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者ID %d 不存在！\n", patientId);
        pause();
        return;
    }
    printf("\n===== 个人信息 =====\n");
    printf("ID: %d\n", p->id);
    printf("姓名: %s\n", p->name);
    printf("年龄: %d\n", p->age);
    printf("性别: %c\n", p->gender);
    printf("电话: %s\n", p->phone);
    printf("累计费用: %.2f 元\n", p->totalCost);
    printf("状态: ");
    switch (p->status) {
    case PATIENT_STATUS_NOT_REGISTERED: printf("未挂号\n"); break;
    case PATIENT_STATUS_WAITING:        printf("等待接诊\n"); break;
    case PATIENT_STATUS_IN_DIAGNOSIS:   printf("接诊中\n"); break;
    case PATIENT_STATUS_WAITING_PAY:    printf("待缴费\n"); break;
    case PATIENT_STATUS_IN_HOSPITAL:    printf("住院中\n"); break;
    case PATIENT_STATUS_DISCHARGED:     printf("已出院\n"); break;
    default: printf("未知状态\n"); break;
    }
    if (p->currentDoctorId != -1) {
        Doctor* doc = findDoctorById(p->currentDoctorId);
        if (doc) printf("当前医生: %s\n", doc->name);
    }
    if (p->wardId != -1 && p->bedId != -1) {
        printf("病房号: %d, 床位号: %d\n", p->wardId, p->bedId);
    }
    pause();
}

// 显示医疗记录
void showMedicalRecords(int patientId) {
    printf("\n===== 医疗记录 =====\n");
    RecordNode* cur = recordHead;
    int hasAny = 0;
    while (cur) {
        if (cur->data.patientId == patientId) {
            hasAny = 1;
            printf("[%d-%d-%d] ", cur->data.recordDate.year,
                cur->data.recordDate.month, cur->data.recordDate.day);
            switch (cur->data.recordType) {
            case 1: printf("挂号: "); break;
            case 2: printf("看诊: "); break;
            case 3: printf("检查/检验: "); break;
            case 4: printf("住院: "); break;
            default: printf("其他: ");
            }
            printf("%s\n", cur->data.detail);
            printf("      费用: %.2f 元\n", cur->data.cost);
        }
        cur = cur->next;
    }
    if (!hasAny) {
        printf("暂无任何医疗记录。\n");
    }
    pause();
}

// 显示住院信息
void showHospitalInfo(int patientId) {
    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者ID %d 不存在！\n", patientId);
        pause();
        return;
    }
    if (p->wardId == -1 || p->bedId == -1) {
        printf("您当前未住院。\n");
        pause();
        return;
    }
    Ward* ward = findWardById(p->wardId);
    if (!ward) {
        printf("病房信息异常（病房ID %d 不存在）\n", p->wardId);
        pause();
        return;
    }
    printf("\n===== 住院信息 =====\n");
    printf("病房名称: %s\n", ward->name);
    printf("病房ID: %d, 床位号: %d\n", p->wardId, p->bedId);
    printf("日费用: %.2f 元\n", ward->pricePerDay);
    Doctor* doc = findDoctorById(p->currentDoctorId);
    if (doc) {
        printf("主治医生: %s\n", doc->name);
    }
    else {
        printf("主治医生信息缺失\n");
    }
    pause();
}

// 显示费用明细
void showCostDetail(int patientId) {
    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者ID %d 不存在！\n", patientId);
        pause();
        return;
    }
    float total = 0.0f;
    printf("\n===== 费用明细 =====\n");
    RecordNode* cur = recordHead;
    while (cur) {
        if (cur->data.patientId == patientId) {
            printf("%s: %.2f 元\n", cur->data.detail, cur->data.cost);
            total += cur->data.cost;
        }
        cur = cur->next;
    }
    double pay = applyMedicare(p->age, total);
    printf("总费用: %.2f 元\n", total);
    if (total > 1000.0f) {
        printf("警告：该患者治疗费用已超过1000元上限！\n");
    }
    if (p->age > 60) {
        printf("(60岁以上老人享受50%%医保报销) 报销后应付: %.2f 元\n", pay);
    }
    else {
        printf("应付: %.2f 元\n", pay);
    }
    pause();
}

// 医保报销计算
double applyMedicare(int age, double totalCost) {
    return (age > 60) ? totalCost * 0.5 : totalCost;
}

// 查询排队叫号状态
void checkQueueStatus(int patientId) {
    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者ID %d 不存在！\n", patientId);
        pause();
        return;
    }
    if (p->status != PATIENT_STATUS_WAITING) {
        printf("您当前没有正在等待的挂号记录。\n");
        pause();
        return;
    }
    // 查找该患者最新的有效挂号记录（status == 1）
    RegNode* cur = regHead;
    Registration* myReg = NULL;
    while (cur) {
        if (cur->data.patientId == patientId && cur->data.status == 1) {
            if (!myReg || cur->data.id > myReg->id) {
                myReg = &cur->data;
            }
        }
        cur = cur->next;
    }
    if (!myReg) {
        printf("未找到有效的挂号记录，请重新挂号。\n");
        pause();
        return;
    }
    Doctor* doc = findDoctorById(myReg->doctorId);
    if (!doc) {
        printf("医生信息异常，无法查询排队状态。\n");
        pause();
        return;
    }
    int myQueue = myReg->queueNumber;
    int currentQueue = doc->currentQueueNumber;
    if (myQueue < currentQueue) {
        printf("您的号（%d）已过，请重新挂号或联系医生。\n", myQueue);
    }
    else if (myQueue == currentQueue) {
        printf("轮到您就诊了！请前往医生诊室。\n");
    }
    else {
        int ahead = myQueue - currentQueue;
        printf("您当前在候诊队列中，前面还有 %d 人。\n", ahead);
        printf("预计等待时间约 %d 分钟。\n", ahead * 15);
    }
    pause();
}

// 患者主菜单
void patientMenu(int patientId) {
    // 首次进入时，若从未挂过号，询问是否挂号
    if (!hasRegisteredBefore(patientId)) {
        printf("\n您还没有任何挂号记录，是否现在挂号？(1-是, 0-否): ");
        int choice;
        if (scanf("%d", &choice) == 1 && choice == 1) {
            registerVisit(patientId);
        }
        else {
            while (getchar() != '\n');  // 清除无效输入
        }
    }

    int op;
    do {
        clearScreen();
        printf("\n===== 患者菜单 =====\n");
        printf("1. 挂号（现场/预约）\n");
        printf("2. 查看个人信息\n");
        printf("3. 查看医疗记录\n");
        printf("4. 查看住院信息\n");
        printf("5. 查看费用明细\n");
        printf("6. 查看排队叫号状态\n");
        printf("7. 修改密码\n");
        printf("8. 退号\n");
        printf("0. 返回上级\n");
        printf("请选择: ");
        if (scanf("%d", &op) != 1) {
            printf("输入无效，请输入数字。\n");
            while (getchar() != '\n');
            pause();
            continue;
        }
        switch (op) {
        case 1: registerVisit(patientId); break;
        case 2: showPersonalInfo(patientId); break;
        case 3: showMedicalRecords(patientId); break;
        case 4: showHospitalInfo(patientId); break;
        case 5: showCostDetail(patientId); break;
        case 6: checkQueueStatus(patientId); break;
        case 7: changePatientPassword(patientId); break;
        case 8: cancelRegistration(patientId); break;
        case 0: break;
        default: printf("无效选择，请重新输入。\n"); pause(); break;
        }
    } while (op != 0);
}