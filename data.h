#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PATIENTS 150   //患者人数
#define MAX_OUTPATIENTS 120//门诊患者最大人数
#define MAX_INPATIENTS 30 //住院患者最大人数
#define MAX_DOCTORS 20    //医生人数
#define MAX_DEPTS 5       //科室数
#define DOCTORS_PER_DEPT 4 //每个科室医生人数
#define WARD_TYPES 3       //普通 重症 急诊
#define BEDS_PER_WARD 10   //每个病房床位数
#define MAX_MEDICINES 20  //药品书
#define MAX_RECORDS 500    //医疗记录最大数
#define PATIENT_STATUS_NOT_REGISTERED 0  //未挂号
#define PATIENT_STATUS_WAITING 1         //已挂号（等待中）
#define PATIENT_STATUS_IN_DIAGNOSIS 2    //接诊中
#define PATIENT_STATUS_WAITING_PAY 3     //代缴费
#define PATIENT_STATUS_IN_HOSPITAL 4     //住院中
#define PATIENT_STATUS_DISCHARGED 5      //已出院
// 挂号记录状态
#define REG_STATUS_REGISTERED 1   // 已挂号（待就诊）
#define REG_STATUS_IN_DIAGNOSIS 2 // 接诊中（已叫号）
#define REG_STATUS_CANCELED   3   // 已退号
#define REG_STATUS_FINISHED   4   // 已完成
#define REG_STATUS_NOSHOW     5   // 爽约（预留）
// 挂号类型
#define REG_TYPE_ONLINE  1   // 预约/线上
#define REG_TYPE_OFFLINE 2   // 现场
#define REG_STATUS_CANCELED 3//取消挂号

typedef struct {           //时间结构体
    int year, month, day;  //年月日
} Date;

typedef struct Patient {   //患者结构体
    int id;
    char name[20];          // 姓名（可能重名，用id区分）
    int age;                //年龄
    char gender;            // 'M' 男, 'F' 女
    char phone[15];         //电话号码
    char password[20];      //密码
    int type;               // 0门诊 1住院
    int wardId;             //病房号
    int bedId;             //床位号
    Date admitDate;         //入院时期
    Date dischargeDate;     //出院时期
    float totalCost;        // 累计费用（不超过1000元）
    float paid;             // 已缴费用
    int status;             // 0未挂号 1等待接诊 2接诊中 3待缴费 4住院中 5已出院
    int currentDoctorId;    //医生id
    int currentDeptId;      //科室id
} Patient;

typedef struct Doctor {   //医生结构体
    int id;               //工号
    char name[20];        //姓名
    int deptId;           //所属科室
    float registerFee;      // 挂号费
    char password[20];     //密码
    int todayVisits;       //今日访客数
    int totalVisits;       //总访客数
    int currentPatients;    // 当前接诊人数
    int maxPatients;        // 最大接诊上限（30人/天）
    int currentQueueNumber;  //当前正在叫号的排队序号（0表示未叫号）
        int lastCalledId;   //最后叫号的患者挂号记录ID
} Doctor;

typedef struct Department {     
    int id;                 
    char name[20];
    char location[30];  
    int doctorCount;      //医生数量的动态管理
    int todayVisits;
} Department;

typedef struct Ward {   //病房结构体
    int id;
    char name[20];             // 1普通 2重症 3急诊
    float pricePerDay;        
    int totalBeds;           //总床位数
    int occupiedBeds;        //占用床位个数
    int beds[BEDS_PER_WARD]; // 床位占用状态：0空闲，>0患者ID
} Ward;

typedef struct Medicine {    //药品结构体
    int id;                 //编号
    char name[30];          //药品名
    char batchNo[20];       // 批号
    Date inDate;            // 入库时间
    Date expiryDate;        // 有效期
    float price;
    int stock;              //库存数量
    int warningLevel;       //
    int deptId;             // 关联科室，0表示通用
} Medicine;

typedef struct MedicalRecord {
    int id;
    int patientId;
    int doctorId;
    int recordType;         // 1挂号 2看诊 3检查 4住院
    Date recordDate;
    char detail[500];
    float cost;
    int status;             // 0进行中 1已完成
} MedicalRecord;

typedef struct ExamItem {   //检查模块结构体
    int id;
    char name[30];          // CT、B超、X光等
    float price;
} ExamItem;

typedef struct TestItem {  //检验模块结构体
    int id;
    char name[30];          // 血常规、尿常规等
    float price;
    int deptId;
    float normalMin;        // 正常范围最小值
    float normalMax;        // 正常范围最大值
    char unit[10];
} TestItem;

// ==================== 检查记录 ====================
typedef struct ExamRecord {
    int id;
    int patientId;
    int doctorId;
    int examItemId;         // 检查项目ID（关联ExamItem）
    Date examDate;          // 检查日期
    char result[500];       // 检查结果
    char conclusion[200];   // 检查结论
    float cost;
} ExamRecord;

typedef struct ExamRecordNode {
    ExamRecord data;
    struct ExamRecordNode* next;
} ExamRecordNode;
extern ExamRecordNode* examRecordHead;
extern int examRecordCount;

// ==================== 检验记录 ====================
typedef struct TestRecord {
    int id;
    int patientId;
    int doctorId;
    int testItemId;         // 检验项目ID（关联TestItem）
    Date testDate;          // 检验日期
    float resultValue;      // 数值结果
    char resultText[200];   // 文字结果
    int isAbnormal;         // 0正常 1异常
    char abnormalFlag[10];  // "↑偏高" / "↓偏低"
    float cost;
} TestRecord;

typedef struct TestRecordNode {
    TestRecord data;
    struct TestRecordNode* next;
} TestRecordNode;
extern TestRecordNode* testRecordHead;
extern int testRecordCount;

//挂号与复诊模块结构体 
typedef struct Registration {
    int id;
    int patientId;
    int doctorId;
    int deptId;
    Date regDate;          //挂号日期
    int timeSlot;          //1上午 2下午
    int type;               // 1线上 2线下
    int status;             // 1已挂号 2已接诊 3已退号 4已完成
    int queueNumber;        //排队序号（同医生独立计数）
    float fee;
    float actualFee;        // 实际支付（扣除医保后）
    float reimbursement;    // 医保报销金额
} Registration;

typedef struct FollowUpReminder {
    int id;
    int patientId;
    int doctorId;
    Date remindDate;
    char reason[100];
    int isReminded;
} FollowUpReminder;

typedef struct DailyStatistics {    // 统计模块结构体
    Date date;
    int totalVisits;
    float totalIncome;
    int onlineRegistrations;
    int offlineRegistrations;
    int cancellations;
    int elderlyCount;
    float elderlyReimbursement;
} DailyStatistics;

typedef struct PatientNode {      // 患者链表
    Patient data;
    struct PatientNode* next;
} PatientNode;
extern PatientNode* patientHead;
extern int patientCount;

typedef struct DoctorNode {      // 医生链表
    Doctor data;
    struct DoctorNode* next;
} DoctorNode;
extern DoctorNode* doctorHead;
extern int doctorCount;

typedef struct DeptNode {      // 科室链表
    Department data;
    struct DeptNode* next;
} DeptNode;
extern DeptNode* deptHead;
extern int deptCount;

typedef struct WardNode {     // 病房链表
    Ward data;
    struct WardNode* next;
} WardNode;
extern WardNode* wardHead;
extern int wardCount;

typedef struct MedicineNode {   // 药品链表
    Medicine data;
    struct MedicineNode* next;
} MedicineNode;
extern MedicineNode* medicineHead;
extern int medicineCount;

typedef struct RecordNode {    // 医疗记录链表
    MedicalRecord data;
    struct RecordNode* next;
} RecordNode;
extern RecordNode* recordHead;
extern int recordCount;

typedef struct RegNode {      // 挂号记录链表
    Registration data;
    struct RegNode* next;
} RegNode;
extern RegNode* regHead;
extern int regCount;

typedef struct FollowUpNode {  // 复诊提醒链表
    FollowUpReminder data;
    struct FollowUpNode* next;
} FollowUpNode;
extern FollowUpNode* followUpHead;
extern int followUpCount;

// 检查项目节点
typedef struct ExamItemNode {
    ExamItem data;
    struct ExamItemNode* next;
} ExamItemNode;
extern ExamItemNode* examItemHead;
extern int examItemCount;

// 检验项目节点
typedef struct TestItemNode {
    TestItem data;
    struct TestItemNode* next;
} TestItemNode;
extern TestItemNode* testItemHead;
extern int testItemCount;

// 每日统计节点
typedef struct DailyStatNode {
    DailyStatistics data;
    struct DailyStatNode* next;
} DailyStatNode;
extern DailyStatNode* dailyStatHead;
extern int dailyStatCount;
extern char adminPassword[20];

#endif
