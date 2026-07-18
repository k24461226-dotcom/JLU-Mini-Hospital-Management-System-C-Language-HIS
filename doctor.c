#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "utils.h"
#include "file.h"
#include "doctor.h" 
// 医生登录
void doctorLogin(void) {
    int id;
    char pwd[20];
    clearScreen();
    printf("\n========== 医护登录 ==========\n");
    printf("请输入医生ID: ");
    scanf("%d", &id);
    printf("请输入密码: ");
    scanf("%s", pwd);

    Doctor* d = findDoctorById(id);
    if (d && strcmp(d->password, pwd) == 0) {
        printf("\n登录成功！欢迎 %s医生\n", d->name);
        pause();
        doctorMenu(d);
    }
    else {
        printf("\n医生ID或密码错误！\n");
        pause();
    }
}

// 医生主菜单
void doctorMenu(Doctor* d) {
    int choice;
    do {
        clearScreen();
        printf("\n========== 医生菜单 ==========\n");
        printf("医生: %s (科室ID: %d)\n", d->name, d->deptId);
        printf("今日接诊: %d / %d\n", d->todayVisits, d->maxPatients);
        printf("当前待接诊: %d 人\n", d->currentPatients);
        printf("当前叫号: %d\n\n", d->currentQueueNumber);
        printf("  1. 查看今日排班\n");
        printf("  2. 查看等待队列\n");
        printf("  3. 叫号接诊\n");
        printf("  4. 查看我的患者\n");
        printf("  5. 查看患者病史\n");
        printf("  6. 设置最大接诊人数\n");
        printf("  7. 录入检查结果\n");
        printf("  8. 录入检验结果\n");
        printf("  9. 办理出院\n");
        printf("  10. 修改密码\n");
        printf("  0. 返回上级\n");
        printf("请选择: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: viewTodaySchedule(d); break;
        case 2: viewWaitingQueue(d); break;
        case 3: startDiagnosis(d); break;
        case 4: viewMyPatients(d); break;
        case 5: viewMedicalHistory(d); break;
        case 6: setMaxPatients(d); break;
        case 7: enterExamResult(d); break;
        case 8: enterTestResult(d); break;
        case 9: dischargePatient(d); break;
        case 10: changeDoctorPassword(d); break;
        }
    } while (choice != 0);
}
void changeDoctorPassword(Doctor* d) {
    char old[20], new1[20], new2[20];
    printf("\n========== 修改密码 ==========\n");
    printf("请输入原密码: ");
    scanf("%s", old);
    if (strcmp(d->password, old) != 0) {
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
    strcpy(d->password, new1);
    printf("密码修改成功！\n");
    pause();
}
// 录入检验结果（自动判断异常）
void enterTestResult(Doctor* d) {
    int patientId;
    printf("请输入患者ID: ");
    scanf("%d", &patientId);

    TestRecordNode* cur = testRecordHead;
    TestRecord* target = NULL;
    while (cur) {
        if (cur->data.patientId == patientId && cur->data.resultValue == 0 && cur->data.resultText[0] == '\0') {
            target = &cur->data;
            break;
        }
        cur = cur->next;
    }

    if (!target) {
        printf("没有找到待录入结果的检验记录。\n");
        pause();
        return;
    }

    // 获取检验项目的正常范围
    TestItem* ti = findTestItemById(target->testItemId);
    if (!ti) {
        printf("检验项目信息不存在！\n");
        pause();
        return;
    }

    printf("检验项目: %s\n", ti->name);
    printf("正常范围: %.1f - %.1f %s\n", ti->normalMin, ti->normalMax, ti->unit);
    printf("请输入检验数值结果: ");
    scanf("%f", &target->resultValue);

    // 判断异常
    if (ti->normalMin == 0 && ti->normalMax == 0) {
        // 无参考范围的项目（如尿常规），不判断异常
        target->isAbnormal = 0;
        strcpy(target->abnormalFlag, "正常");
    }
    else {
        if (target->resultValue < ti->normalMin) {
            target->isAbnormal = 1;
            strcpy(target->abnormalFlag, "↓偏低");
            printf("? 异常：结果偏低\n");
        }
        else if (target->resultValue > ti->normalMax) {
            target->isAbnormal = 1;
            strcpy(target->abnormalFlag, "↑偏高");
            printf("? 异常：结果偏高\n");
        }
        else {
            target->isAbnormal = 0;
            strcpy(target->abnormalFlag, "正常");
        }
    }

    printf("请输入补充说明（文字结果，可选）: ");
    getchar();
    fgets(target->resultText, 200, stdin);
    target->resultText[strcspn(target->resultText, "\n")] = 0;

    printf("检验结果录入成功！\n");
    pause();
}
// 录入检查结果
void enterExamResult(Doctor* d) {
    int patientId;
    printf("请输入患者ID: ");
    scanf("%d", &patientId);

    // 查找该患者的检查记录（status=0表示已申请未出结果）
    ExamRecordNode* cur = examRecordHead;
    ExamRecord* target = NULL;
    while (cur) {
        if (cur->data.patientId == patientId && cur->data.result[0] == '\0') {
            target = &cur->data;
            break;
        }
        cur = cur->next;
    }

    if (!target) {
        printf("没有找到待录入结果的检查记录。\n");
        pause();
        return;
    }

    printf("检查项目ID: %d\n", target->examItemId);
    printf("请输入检查结果: ");
    getchar();
    fgets(target->result, 500, stdin);
    target->result[strcspn(target->result, "\n")] = 0;

    printf("请输入检查结论: ");
    fgets(target->conclusion, 200, stdin);
    target->conclusion[strcspn(target->conclusion, "\n")] = 0;

    printf("检查结果录入成功！\n");
    pause();
}
// 查看今日排班
void dischargePatient(Doctor* d) {
    int patientId;
    printf("请输入要办理出院的患者ID: ");
    scanf("%d", &patientId);

    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者不存在！\n");
        pause();
        return;
    }

    if (p->type != 1 || p->status != PATIENT_STATUS_IN_HOSPITAL) {
        printf("该患者当前未住院，无法办理出院。\n");
        pause();
        return;
    }

    // 获取病房信息
    Ward* ward = findWardById(p->wardId);
    if (!ward) {
        printf("病房信息错误！\n");
        pause();
        return;
    }

    // 计算住院天数（从入院日期到当前日期）
    Date today = getCurrentDate();
    int days = dateDiff(p->admitDate, today);
    if (days < 0) days = 0;

    float inpatientCost = days * ward->pricePerDay;

    // 增加住院费用
    updatePatientCostById(patientId, inpatientCost);

    // 释放床位
    int bedIndex = p->bedId - 1;
    if (bedIndex >= 0 && bedIndex < BEDS_PER_WARD) {
        if (ward->beds[bedIndex] == patientId) {
            ward->beds[bedIndex] = 0;
            ward->occupiedBeds--;
        }
        else {
            printf("警告：床位患者ID不匹配，请检查数据。\n");
        }
    }

    // 更新患者信息
    p->type = 0;
    p->wardId = -1;
    p->bedId = -1;
    p->dischargeDate = today;
    p->status = PATIENT_STATUS_DISCHARGED;

    // 生成出院医疗记录
    MedicalRecord mr;
    mr.id = ++recordCount;
    mr.patientId = patientId;
    mr.doctorId = d->id;
    mr.recordType = 4;          // 住院记录
    mr.recordDate = today;
    sprintf(mr.detail, "办理出院，住院天数%d天，总费用%.2f元", days, inpatientCost);
    mr.cost = inpatientCost;
    mr.status = 1;              // 已完成
    addMedicalRecordNode(mr);

    printf("\n========== 出院结算 ==========\n");
    printf("患者: %s\n", p->name);
    printf("住院天数: %d 天\n", days);
    printf("床位费: %.2f 元/天\n", ward->pricePerDay);
    printf("住院总费用: %.2f 元\n", inpatientCost);
    printf("出院办理成功！\n");
    pause();
}
void viewTodaySchedule(Doctor* d) {
    clearScreen();
    printf("\n========== 今日排班 ==========\n");
    printf("医生: %s\n", d->name);
    printf("今日已接诊: %d\n", d->todayVisits);
    printf("剩余可接诊: %d\n", d->maxPatients - d->todayVisits);
    printf("当前待接诊: %d\n", d->currentPatients);
    printf("当前叫号: %d\n", d->currentQueueNumber);
    printf("挂号费: %.2f\n", d->registerFee);

    // 显示下一个待接诊患者
    if (d->currentQueueNumber == 0) {
        int nextQueue = 31;
        char nextName[20] = "";
        RegNode* rn = regHead;
        while (rn) {
            if (rn->data.doctorId == d->id && rn->data.status == REG_STATUS_REGISTERED)//已挂号
            {
                if (rn->data.queueNumber < nextQueue) {
                    nextQueue = rn->data.queueNumber;
                    Patient* p = findPatientById(rn->data.patientId);
                    if (p) strcpy(nextName, p->name);
                }
            }
            rn = rn->next;
        }
        if (nextQueue != 31) {
            printf("\n下一位待接诊: %d号 %s\n", nextQueue, nextName);
        }
        else {
            printf("\n暂无等待患者\n");
        }
    }
    else {
        // 显示当前接诊的患者
        RegNode* rn = regHead;
        while (rn) {
            if (rn->data.id == d->lastCalledId) {
                Patient* p = findPatientById(rn->data.patientId);
                if (p) {
                    printf("\n当前接诊: %d号 %s\n", d->currentQueueNumber, p->name);
                }
                break;
            }
            rn = rn->next;
        }
    }
    pause();
}

// 查看等待队列（按排队序号排序）
void viewWaitingQueue(Doctor* d) {
    clearScreen();
    printf("\n========== 等待接诊队列 ==========\n");
    printf("医生: %s\n", d->name);
    printf("当前叫号: %d\n\n", d->currentQueueNumber);

    // 收集排队信息
    typedef struct {
        int queueNumber;
        int patientId;
        char name[20];
        int age;
        int status;
        int regId;
    } QueueInfo;

    QueueInfo queue[50];
    int count = 0;

    RegNode* rn = regHead;
    while (rn && count < 50) {
        if (rn->data.doctorId == d->id &&
            (rn->data.status == REG_STATUS_REGISTERED || rn->data.status == REG_STATUS_IN_DIAGNOSIS)) {
            Patient* p = findPatientById(rn->data.patientId);
            if (p) {
                queue[count].queueNumber = rn->data.queueNumber;
                queue[count].patientId = p->id;
                strcpy(queue[count].name, p->name);
                queue[count].age = p->age;
                queue[count].status = rn->data.status;
                queue[count].regId = rn->data.id;
                count++;
            }
        }
        rn = rn->next;
    }

    // 按排队序号排序
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (queue[j].queueNumber > queue[j + 1].queueNumber) {
                QueueInfo temp = queue[j];
                queue[j] = queue[j + 1];
                queue[j + 1] = temp;
            }
        }
    }

    if (count == 0) {
        printf("当前无等待接诊的患者\n");
    }
    else {
        printf("序号\t患者ID\t姓名\t年龄\t状态\n");
        printf("========================================\n");
        for (int i = 0; i < count; i++) {
            printf("%d\t%d\t%s\t%d\t%s\n",
                queue[i].queueNumber,
                queue[i].patientId,
                queue[i].name,
                queue[i].age,
                queue[i].status == REG_STATUS_REGISTERED ? "等待中" : "接诊中");
        }
        printf("========================================\n");
        printf("共 %d 位患者在等待\n", count);
    }
    pause();
}

// 叫号功能
void callNextPatient(Doctor* d) {
    clearScreen();
    printf("\n========== 叫号接诊 ==========\n");

    // 检查是否有等待的患者
    int waitingCount = 0;
    RegNode* rn = regHead;
    while (rn) {
        if (rn->data.doctorId == d->id && rn->data.status == REG_STATUS_REGISTERED) {
            waitingCount++;
        }
        rn = rn->next;
    }

    if (waitingCount == 0) {
        printf("\n【提示】当前无等待接诊的患者！\n");
        printf("请提醒患者先完成挂号\n");
        pause();
        return;
    }

    // 找出最小排队序号的患者
    int nextQueueNumber = 9999;
    int nextRegId = -1;
    int nextPatientId = -1;
    char nextPatientName[20] = "";

    rn = regHead;
    while (rn) {
        if (rn->data.doctorId == d->id && rn->data.status == REG_STATUS_REGISTERED) {
            if (rn->data.queueNumber < nextQueueNumber) {
                nextQueueNumber = rn->data.queueNumber;
                nextRegId = rn->data.id;
                nextPatientId = rn->data.patientId;
                Patient* p = findPatientById(nextPatientId);
                if (p) {
                    strcpy(nextPatientName, p->name);
                }
            }
        }
        rn = rn->next;
    }

    // 显示叫号信息
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║              【叫号信息】                  ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║                                            ║\n");
    printf("║    请 %d 号患者 %s 就诊！                  ║\n", nextQueueNumber, nextPatientName);
    printf("║    请到 %d 号诊室                          ║\n", d->id);
    printf("║                                            ║\n");
    printf("╚════════════════════════════════════════════╝\n");

    // 更新医生叫号状态
    d->currentQueueNumber = nextQueueNumber;
    d->lastCalledId = nextRegId;

    // 更新挂号记录状态为已接诊
    rn = regHead;
    while (rn) {
        if (rn->data.id == nextRegId) {
            rn->data.status = REG_STATUS_IN_DIAGNOSIS; // 已接诊
            break;
        }
        rn = rn->next;
    }

    // 更新患者状态为接诊中
    Patient* patient = findPatientById(nextPatientId);
    if (patient) {
        patient->status = PATIENT_STATUS_IN_DIAGNOSIS; // 接诊中
    }
    updateDoctorWorkloadById(d->id, 1);
    printf("\n 叫号成功！\n");
    printf("当前叫号: %d\n", d->currentQueueNumber);
    printf("\n请引导患者就诊...\n");
    pause();

    // 叫号后进入接诊界面
    performDiagnosis(d, nextPatientId);
}

// 执行接诊（核心接诊逻辑）
void performDiagnosis(Doctor* d, int patientId) {
    Patient* patient = findPatientById(patientId);
    if (!patient) {
        printf("患者不存在！\n");
        pause();
        return;
    }

    // 验证是否是当前叫号的患者
    int isValid = 0;
    RegNode* rn = regHead;
    while (rn) {
        if (rn->data.patientId == patientId && rn->data.doctorId == d->id &&
            rn->data.status == REG_STATUS_IN_DIAGNOSIS) {
            isValid = 1;
            break;
        }
        rn = rn->next;
    }

    if (!isValid && d->currentQueueNumber > 0) {
        printf("\n【警告】当前叫号是 %d 号患者，请按顺序接诊！\n", d->currentQueueNumber);
        printf("请先完成当前叫号患者的诊疗\n");
        pause();
        return;
    }

    clearScreen();
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║            【患者接诊】                    ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("\n患者信息:\n");
    printf("  姓名: %s\n", patient->name);
    printf("  患者ID: %d\n", patient->id);
    printf("  年龄: %d\n", patient->age);
    printf("  性别: %c\n", patient->gender);
    printf("  电话: %s\n", patient->phone);
    printf("  类型: %s\n", patient->type == 0 ? "门诊" : "住院");
    printf("  叫号: %d号\n", d->currentQueueNumber);
    printf("  累计费用: %.2f 元\n", patient->totalCost);
    printf("  已缴费用: %.2f 元\n", patient->paid);
    printf("  欠费金额: %.2f 元\n", patient->totalCost - patient->paid);

    // 接诊操作循环
    int choice;
    do {
        printf("\n╔════════════════════════════════════════════╗\n");
        printf("║              【接诊操作】                  ║\n");
        printf("╠════════════════════════════════════════════╣\n");
        printf("║  1. 开具药品                               ║\n");
        printf("║  2. 申请检查 (CT/B超/X光等)               ║\n");
        printf("║  3. 申请检验 (血常规/尿常规等)            ║\n");
        printf("║  4. 办理住院                               ║\n");
        printf("║  5. 设置复诊提醒                     ║\n");
        printf("║  6. 完成诊断并结算                          ║\n");
        printf("║  0. 暂存退出                               ║\n");
        printf("╚════════════════════════════════════════════╝\n");
        printf("请选择: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: prescribeMedicine(patient->id, d->id); break;
        case 2: requestExam(patient->id, d->id); break;
        case 3: requestTest(patient->id, d->id); break;
        case 4: admitPatient(patient->id, d->id); break;
        case 5: setFollowUpReminder(d, patient->id); break;
        case 6:  completeDiagnosis(d, patient); break;
        case 0:
            printf("\n已保存当前接诊状态，可稍后继续\n");
            break;
        default:
            printf("无效选择！\n");
            break;
        }
    } while (choice != 5 && choice != 0);
}

// 开始接诊（入口函数）
void startDiagnosis(Doctor* d) {
    // 检查是否有等待的患者
    int waitingCount = 0;
    RegNode* rn = regHead;
    while (rn) {
        if (rn->data.doctorId == d->id && rn->data.status == REG_STATUS_REGISTERED) {
            waitingCount++;
        }
        rn = rn->next;
    }

    if (waitingCount == 0) {
        printf("\n【提示】当前无等待接诊的患者！\n");
        printf("请提醒患者先完成挂号\n");
        pause();
        return;
    }

    // 如果有正在接诊但未完成的患者，提示继续
    if (d->currentQueueNumber > 0) {
        printf("\n【提示】当前正在接诊 %d 号患者\n", d->currentQueueNumber);
        printf("是否继续接诊当前患者？(y/n): ");
        char choice;
        getchar();
        scanf("%c", &choice);
        if (choice == 'y' || choice == 'Y') {
            // 找到当前接诊的患者ID
            RegNode* rn2 = regHead;
            while (rn2) {
                if (rn2->data.id == d->lastCalledId) {
                    performDiagnosis(d, rn2->data.patientId);
                    return;
                }
                rn2 = rn2->next;
            }
        }
    }

    // 叫下一个患者
    callNextPatient(d);
}

// 开具药品
void prescribeMedicine(int patientId, int doctorId) {
    clearScreen();
    printf("\n========== 开具药品 ==========\n");

    // 显示可用药品
    printf("\n可用药品列表:\n");
    printf("ID\t药品名称\t\t单价\t库存\n");
    printf("----------------------------------------\n");
    MedicineNode* m = medicineHead;
    while (m) {
        if (m->data.stock > 0) {
            printf("%d\t%-12s\t%.2f\t%d\n",
                m->data.id, m->data.name, m->data.price, m->data.stock);
        }
        m = m->next;
    }

    int medId, quantity;
    printf("\n请输入药品ID: ");
    scanf("%d", &medId);

    Medicine* med = findMedicineById(medId);
    if (!med) {
        printf("药品不存在！\n");
        pause();
        return;
    }

    if (med->stock <= 0) {
        printf("该药品库存不足！\n");
        pause();
        return;
    }

    printf("药品: %s, 单价: %.2f, 库存: %d\n", med->name, med->price, med->stock);
    printf("请输入数量: ");
    scanf("%d", &quantity);

    if (quantity <= 0) {
        printf("数量无效！\n");
        pause();
        return;
    }

    if (quantity > med->stock) {
        printf("库存不足！当前库存: %d\n", med->stock);
        pause();
        return;
    }
    // 计算费用
    float cost = med->price * quantity;

    // 更新库存
    med->stock -= quantity;

    // 更新患者费用
    updatePatientCostById(patientId, cost);

    // 创建医疗记录
    MedicalRecord mr;
    mr.id = ++recordCount;
    mr.patientId = patientId;
    mr.doctorId = doctorId;
    mr.recordType = 3;
    mr.recordDate = getCurrentDate();
    sprintf(mr.detail, "开具药品: %s x%d, 单价: %.2f, 合计: %.2f",
        med->name, quantity, med->price, cost);
    mr.cost = cost;
    mr.status = 1;
    addMedicalRecordNode(mr);

    printf("\n 药品开具成功！\n");
    printf("  药品: %s x%d\n", med->name, quantity);
    printf("  费用: %.2f 元\n", cost);
    pause();
}

// 申请检查（使用新的ExamRecord结构体）
void requestExam(int patientId, int doctorId) {
    clearScreen();
    printf("\n========== 申请检查 ==========\n");
    printf("\n检查项目列表:\n");
    printf("  1. CT         - 350.00 元\n");
    printf("  2. B超        - 120.00 元\n");
    printf("  3. X光        - 80.00 元\n");
    printf("  4. 核磁共振   - 600.00 元\n");
    printf("  5. 心电图     - 50.00 元\n");
    printf("  6. 胃镜       - 280.00 元\n");

    int examId;
    printf("\n请输入检查项目编号: ");
    scanf("%d", &examId);

    float cost = 0;
    char examName[30];

    switch (examId) {
    case 1: strcpy(examName, "CT"); cost = 350; break;
    case 2: strcpy(examName, "B超"); cost = 120; break;
    case 3: strcpy(examName, "X光"); cost = 80; break;
    case 4: strcpy(examName, "核磁共振"); cost = 600; break;
    case 5: strcpy(examName, "心电图"); cost = 50; break;
    case 6: strcpy(examName, "胃镜"); cost = 280; break;
    default:
        printf("无效选择！\n");
        pause();
        return;
    }

    // 创建检查记录（使用ExamRecord结构体）
    ExamRecord er;
    er.id = ++examRecordCount;
    er.patientId = patientId;
    er.doctorId = doctorId;
    er.examItemId = examId;
    er.examDate = getCurrentDate();
    strcpy(er.result, "");
    strcpy(er.conclusion, "");
    er.cost = cost;
    addExamRecordNode(er);

    // 更新患者费用
    updatePatientCostById(patientId, cost);

    // 创建医疗记录
    MedicalRecord mr;
    mr.id = ++recordCount;
    mr.patientId = patientId;
    mr.doctorId = doctorId;
    mr.recordType = 3;
    mr.recordDate = getCurrentDate();
    sprintf(mr.detail, "申请检查: %s, 费用: %.2f", examName, cost);
    mr.cost = cost;
    mr.status = 0;
    addMedicalRecordNode(mr);

    printf("\n 检查申请成功！\n");
    printf("  项目: %s\n", examName);
    printf("  费用: %.2f 元\n", cost);
    pause();
}

// 申请检验（使用新的TestRecord结构体）
void requestTest(int patientId, int doctorId) {
    clearScreen();
    printf("\n========== 申请检验 ==========\n");
    printf("\n检验项目列表:\n");
    printf("  1. 血常规 - 30.00 元 (参考: 3.5-9.5 10^9/L)\n");
    printf("  2. 尿常规 - 25.00 元\n");
    printf("  3. 肝功能 - 60.00 元 (参考: 0-40 U/L)\n");
    printf("  4. 肾功能 - 50.00 元 (参考: 44-104 umol/L)\n");
    printf("  5. 血糖   - 20.00 元 (参考: 3.9-6.1 mmol/L)\n");

    int testId;
    printf("\n请输入检验项目编号: ");
    scanf("%d", &testId);

    float cost = 0;
    char testName[30];

    switch (testId) {
    case 1: strcpy(testName, "血常规"); cost = 30; break;
    case 2: strcpy(testName, "尿常规"); cost = 25; break;
    case 3: strcpy(testName, "肝功能"); cost = 60; break;
    case 4: strcpy(testName, "肾功能"); cost = 50; break;
    case 5: strcpy(testName, "血糖"); cost = 20; break;
    default:
        printf("无效选择！\n");
        pause();
        return;
    }

    // 创建检验记录（使用TestRecord结构体）
    TestRecord tr;
    tr.id = ++testRecordCount;
    tr.patientId = patientId;
    tr.doctorId = doctorId;
    tr.testItemId = testId;
    tr.testDate = getCurrentDate();
    tr.resultValue = 0;
    strcpy(tr.resultText, "");
    tr.isAbnormal = 0;
    strcpy(tr.abnormalFlag, "");
    tr.cost = cost;
    addTestRecordNode(tr);

    // 更新患者费用
    updatePatientCostById(patientId, cost);

    // 创建医疗记录
    MedicalRecord mr;
    mr.id = ++recordCount;
    mr.patientId = patientId;
    mr.doctorId = doctorId;
    mr.recordType = 3;
    mr.recordDate = getCurrentDate();
    sprintf(mr.detail, "申请检验: %s, 费用: %.2f", testName, cost);
    mr.cost = cost;
    mr.status = 0;
    addMedicalRecordNode(mr);

    printf("\n 检验申请成功！\n");
    printf("  项目: %s\n", testName);
    printf("  费用: %.2f 元\n", cost);
    pause();
}
// 设置复诊提醒
void setFollowUpReminder(Doctor* d, int patientId) {
    FollowUpReminder fur;
    fur.id = followUpCount + 1;
    fur.patientId = patientId;
    fur.doctorId = d->id;
    fur.isReminded = 0;

    printf("请输入复诊日期（年 月 日）: ");
    scanf("%d %d %d", &fur.remindDate.year, &fur.remindDate.month, &fur.remindDate.day);
    printf("请输入复诊原因: ");
    getchar();
    fgets(fur.reason, 100, stdin);
    fur.reason[strcspn(fur.reason, "\n")] = 0;

    addFollowUpNode(fur);
    printf("复诊提醒已设置！\n");
    pause();
}
// 办理住院
void admitPatient(int patientId, int doctorId) {
    clearScreen();
    printf("\n========== 办理住院 ==========\n");

    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者不存在！\n");
        pause();
        return;
    }

    if (p->type == 1) {
        printf("患者已在住院中！\n");
        pause();
        return;
    }

    printf("\n病房列表:\n");
    printf("ID\t病房名称\t床位费/天\t占用情况\n");
    printf("----------------------------------------\n");
    WardNode* w = wardHead;
    while (w) {
        printf("%d\t%s\t\t%.2f\t%d/%d\n",
            w->data.id, w->data.name, w->data.pricePerDay,
            w->data.occupiedBeds, w->data.totalBeds);
        w = w->next;
    }

    int wardId;
    printf("\n请选择病房ID: ");
    scanf("%d", &wardId);

    Ward* ward = findWardById(wardId);
    if (!ward) {
        printf("病房不存在！\n");
        pause();
        return;
    }

    if (ward->occupiedBeds >= ward->totalBeds) {
        printf("该病房已满！\n");
        pause();
        return;
    }

    // 分配床位
    int bedId = -1;
    for (int i = 0; i < ward->totalBeds; i++) {
        if (ward->beds[i] == 0) {
            bedId = i + 1;
            ward->beds[i] = patientId;
            break;
        }
    }

    if (bedId == -1) {
        printf("床位分配失败！\n");
        pause();
        return;
    }

    // 更新患者信息
    p->type = 1;
    p->wardId = wardId;
    p->bedId = bedId;
    p->admitDate = getCurrentDate();
    p->status = 4;
    ward->occupiedBeds++;

    // 创建医疗记录
    MedicalRecord mr;
    mr.id = ++recordCount;
    mr.patientId = patientId;
    mr.doctorId = doctorId;
    mr.recordType = 4;
    mr.recordDate = getCurrentDate();
    sprintf(mr.detail, "办理住院，病房: %s，床位: %d号，床位费: %.2f/天",
        ward->name, bedId, ward->pricePerDay);
    mr.cost = 0;
    mr.status = 0;
    addMedicalRecordNode(mr);

    printf("\n 住院办理成功！\n");
    printf("  病房: %s\n", ward->name);
    printf("  床位: %d号\n", bedId);
    printf("  床位费: %.2f 元/天\n", ward->pricePerDay);
    pause();
}

// 完成诊断
void completeDiagnosis(Doctor* d, Patient* p) {
    clearScreen();
    printf("\n========== 完成诊断 ==========\n");
    printf("患者: %s (ID: %d)\n", p->name, p->id);
    printf("累计费用: %.2f 元\n", p->totalCost);
    printf("已缴费用: %.2f 元\n", p->paid);
    printf("欠费金额: %.2f 元\n", p->totalCost - p->paid);

    char diagnosis[500];
    printf("\n请输入诊断意见: ");
    getchar();
    fgets(diagnosis, 500, stdin);
    diagnosis[strcspn(diagnosis, "\n")] = 0;

    if (strlen(diagnosis) == 0) {
        strcpy(diagnosis, "已完成诊疗");
    }

    // 创建诊断记录
    MedicalRecord mr;
    mr.id = ++recordCount;
    mr.patientId = p->id;
    mr.doctorId = d->id;
    mr.recordType = 2;
    mr.recordDate = getCurrentDate();
    strcpy(mr.detail, diagnosis);
    mr.cost = 0;
    mr.status = 1;
    addMedicalRecordNode(mr);

    // 获取挂号记录ID
    int regId = d->lastCalledId;

    // 更新患者状态
    if (p->type == 1 && p->status == 4) {
        p->status = 4; // 住院患者继续住院
        printf("\n患者继续住院治疗\n");
    }
    else {
        if (p->totalCost - p->paid <= 0) {
            p->status = 5; // 已出院/完成
            printf("\n患者已完成诊疗，无欠费\n");
        }
        else {
            p->status = 3; // 待缴费
            printf("\n患者已完成诊疗，尚有 %.2f 元待缴费\n", p->totalCost - p->paid);
        }
    }

    // 更新医生统计数据
    d->todayVisits++;
    d->totalVisits++;
    d->currentPatients--;

    // 更新挂号记录状态为已完成
    RegNode* rn = regHead;
    while (rn) {
        if (rn->data.id == regId) {
            rn->data.status = REG_STATUS_FINISHED;
            break;
        }
        rn = rn->next;
    }

    // 更新科室接诊数
    updateDeptVisitsById(d->deptId);

    // 重置叫号状态
    d->currentQueueNumber = 0;
    d->lastCalledId = 0;

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║           【诊断完成】                     ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("诊断意见: %s\n", diagnosis);
    printf("\n 接诊完成！\n");
    printf("可以继续叫号接诊下一位患者\n");
    pause();
}

// 查看我的患者
void viewMyPatients(Doctor* d) {
    clearScreen();
    printf("\n========== 我的患者 ==========\n");
    printf("医生: %s\n\n", d->name);

    int found = 0;
    RegNode* rn = regHead;

    while (rn) {
        if (rn->data.doctorId == d->id) {
            Patient* p = findPatientById(rn->data.patientId);
            if (p) {
                found = 1;
                printf("患者ID: %d\n", p->id);
                printf("  姓名: %s\n", p->name);
                printf("  年龄: %d\n", p->age);
                printf("  排队号: %d\n", rn->data.queueNumber);
                printf("  状态: ");
                switch (rn->data.status) {
                case REG_STATUS_REGISTERED: printf("等待接诊\n"); break;
                case REG_STATUS_IN_DIAGNOSIS: printf("接诊中\n"); break;
                case REG_STATUS_CANCELED: printf("已退号\n"); break;
                case REG_STATUS_FINISHED: printf("已完成\n"); break;
                default: printf("未知\n");
                }
                printf("  -------------------------\n");
            }
        }
        rn = rn->next;
    }

    if (!found) {
        printf("暂无患者\n");
    }
    pause();
}

// 查看患者病史
void viewMedicalHistory(Doctor* d) {
    clearScreen();
    printf("\n========== 查看患者病史 ==========\n");

    int patientId;
    printf("请输入患者ID: ");
    scanf("%d", &patientId);

    Patient* p = findPatientById(patientId);
    if (!p) {
        printf("患者不存在！\n");
        pause();
        return;
    }

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║           【患者基本信息】                  ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("姓名: %s\n", p->name);
    printf("ID: %d\n", p->id);
    printf("年龄: %d\n", p->age);
    printf("性别: %c\n", p->gender);
    printf("电话: %s\n", p->phone);

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║           【病史记录】                      ║\n");
    printf("╚════════════════════════════════════════════╝\n");

    RecordNode* r = recordHead;
    int found = 0;
    while (r) {
        if (r->data.patientId == patientId) {
            found = 1;
            printf("\n日期: %d-%d-%d\n", r->data.recordDate.year,
                r->data.recordDate.month, r->data.recordDate.day);
            printf("类型: %s\n",
                r->data.recordType == 1 ? "挂号" :
                r->data.recordType == 2 ? "看诊" :
                r->data.recordType == 3 ? "检查/检验" : "住院");
            printf("详情: %s\n", r->data.detail);
            printf("费用: %.2f 元\n", r->data.cost);
            printf("----------------------------------------\n");
        }
        r = r->next;
    }

    if (!found) {
        printf("暂无病史记录\n");
    }
    pause();
}

// 设置最大接诊人数
void setMaxPatients(Doctor* d) {
    clearScreen();
    printf("\n========== 设置最大接诊人数 ==========\n");
    printf("当前最大接诊人数: %d\n", d->maxPatients);
    printf("请输入新的最大接诊人数 (1-50): ");

    int newMax;
    scanf("%d", &newMax);

    if (newMax < 1 || newMax > 50) {
        printf("无效输入！范围: 1-50\n");
    }
    else {
        d->maxPatients = newMax;
        printf("设置成功！\n");
    }
    pause();
}