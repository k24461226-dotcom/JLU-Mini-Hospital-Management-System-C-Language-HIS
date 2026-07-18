#include "file.h"
#include "data.h"
#include "utils.h"

void initSampleData(void) {
     
    char* deptNames[] = { "内科", "外科", "儿科", "妇产科", "男科" };//  初始化科室（5个）
    for (int i = MAX_DEPTS - 1; i >= 0; i--) {
        Department d;
        d.id = i + 1;
        strcpy(d.name, deptNames[i]);
        sprintf(d.location, "%d楼", i + 1);
        d.doctorCount = DOCTORS_PER_DEPT;
        d.todayVisits = 0;
        addDepartmentNode(d);   
    }

    Doctor d;
    d.registerFee = 10.0;
    d.todayVisits = 0;
    d.totalVisits = 0;
    d.currentPatients = 0;
    d.maxPatients = 30;
    d.currentQueueNumber = 0;
    d.lastCalledId = 0;
    strcpy(d.password, "123456");
    //内科四人
    d.id = 1; strcpy(d.name, "张建国"); d.deptId = 1; addDoctorNode(d);
    d.id = 2; strcpy(d.name, "李翠花"); d.deptId = 1; addDoctorNode(d);
    d.id = 3; strcpy(d.name, "王明"); d.deptId = 1; addDoctorNode(d);
    d.id = 4; strcpy(d.name, "赵红"); d.deptId = 1; addDoctorNode(d);
    //外科四人
    d.id = 5; strcpy(d.name, "张A"); d.deptId = 2; addDoctorNode(d);
    d.id = 6; strcpy(d.name, "李B"); d.deptId = 2; addDoctorNode(d);
    d.id = 7; strcpy(d.name, "王C"); d.deptId = 2; addDoctorNode(d);
    d.id = 8; strcpy(d.name, "赵D"); d.deptId = 2; addDoctorNode(d);
    //儿科四人
    d.id = 9; strcpy(d.name, "张E"); d.deptId = 3; addDoctorNode(d);
    d.id = 10; strcpy(d.name, "李F"); d.deptId = 3; addDoctorNode(d);
    d.id = 11; strcpy(d.name, "王G"); d.deptId = 3; addDoctorNode(d);
    d.id = 12; strcpy(d.name, "赵H"); d.deptId = 3; addDoctorNode(d);
    //妇产科四人
    d.id = 13; strcpy(d.name, "张I"); d.deptId = 4; addDoctorNode(d);
    d.id = 14; strcpy(d.name, "李J"); d.deptId = 4; addDoctorNode(d);
    d.id = 15; strcpy(d.name, "王K"); d.deptId = 4; addDoctorNode(d);
    d.id = 16; strcpy(d.name, "赵L"); d.deptId = 4; addDoctorNode(d);
    //男科四人
    d.id = 17; strcpy(d.name, "张M"); d.deptId = 5; addDoctorNode(d);
    d.id = 18; strcpy(d.name, "李N"); d.deptId = 5; addDoctorNode(d);
    d.id = 19; strcpy(d.name, "王O"); d.deptId = 5; addDoctorNode(d);
    d.id = 20; strcpy(d.name, "赵P"); d.deptId = 5; addDoctorNode(d);

    // 初始化病房
    Ward w3 = { 3, "急诊病房", 200.0, 10, 0, {0} };
    addWardNode(w3);
    
    Ward w2 = { 2, "重症病房", 300.0, 10, 0, {0} };
    addWardNode(w2);
    
    Ward w1 = { 1, "普通病房", 100.0, 10, 0, {0} };
    addWardNode(w1);

    char* medicineNames[] = { "阿莫西林","布洛芬","头孢克肟","感冒灵","维生素C",    //  初始化药品（20类）
                             "板蓝根","罗红霉素","奥美拉唑","阿奇霉素","黄连素",
                             "双黄连","蒲地蓝","氨溴索","蒙脱石散","氯雷他定",
                             "对乙酰氨基酚","氨酚黄那敏","阿司匹林","复方甘草片","牛黄解毒片" };
    srand((unsigned)time(NULL));
    for (int i = MAX_MEDICINES - 1; i >= 0; i--) {
        Medicine m;
        m.id = i + 1;
        strcpy(m.name, medicineNames[i]);
        m.price = (float)(rand() % 500 + 100) / 10;
        m.stock = 20;
        m.deptId = (i < 10) ? (i % MAX_DEPTS + 1) : 0;
        addMedicineNode(m);  
    }

    
    ExamItem examList[] = {  //  初始化检查项目
        {1, "CT", 350.0}, {2, "B超", 120.0}, {3, "X光", 80.0},
        {4, "核磁共振", 600.0}, {5, "心电图", 50.0}, {6, "胃镜", 280.0}
    };
    for (int i = 0; i < 6; i++) {
        addExamItemNode(examList[i]);
    }

    TestItem testList[] = {   //  初始化检验项目
        {1, "血常规", 30.0,1, 3.5, 9.5, "10^9/L"},
        {2, "尿常规", 25.0,2, 0, 0, ""},
        {3, "肝功能", 60.0,3, 0, 40, "U/L"},
        {4, "肾功能", 50.0,4, 44, 104, "umol/L"},
        {5, "血糖", 20.0,5, 3.9, 6.1, "mmol/L"}
    };
    for (int i = 0; i < 5; i++) {
        addTestItemNode(testList[i]);
    }

    Patient p;
    strcpy(p.password, "123456");
    p.type = 0;                     // 门诊
    p.wardId = -1;                  // 未住院
    p.bedId = -1;
    p.admitDate = (Date){ 0,0,0 };
    p.dischargeDate = (Date){ 0,0,0 };
    p.totalCost = 0;
    p.paid = 0;
    p.status = PATIENT_STATUS_NOT_REGISTERED;  // 未挂号
    p.currentDoctorId = -1;
    p.currentDeptId = -1;
    p.id = 1; strcpy(p.name, "张三"); p.age = 35; p.gender = 'M'; strcpy(p.phone, "13800000001"); addPatientNode(p);
    p.id = 2; strcpy(p.name, "李四"); p.age = 65; p.gender = 'F'; strcpy(p.phone, "13800000002"); addPatientNode(p);
    p.id = 3; strcpy(p.name, "王芳"); p.age = 28; p.gender = 'F'; strcpy(p.phone, "13800000003"); addPatientNode(p);
    p.id = 4; strcpy(p.name, "刘强"); p.age = 72; p.gender = 'M'; strcpy(p.phone, "13800000004"); addPatientNode(p);
    p.id = 5; strcpy(p.name, "陈丽"); p.age = 42; p.gender = 'F'; strcpy(p.phone, "13800000005"); addPatientNode(p);
    p.id = 6; strcpy(p.name, "赵伟"); p.age = 53; p.gender = 'M'; strcpy(p.phone, "13800000006"); addPatientNode(p);
    p.id = 7; strcpy(p.name, "孙梅"); p.age = 29; p.gender = 'F'; strcpy(p.phone, "13800000007"); addPatientNode(p);
    p.id = 8; strcpy(p.name, "周杰"); p.age = 48; p.gender = 'M'; strcpy(p.phone, "13800000008"); addPatientNode(p);
    p.id = 9; strcpy(p.name, "吴敏"); p.age = 61; p.gender = 'F'; strcpy(p.phone, "13800000009"); addPatientNode(p);
    p.id = 10; strcpy(p.name, "郑涛"); p.age = 37; p.gender = 'M'; strcpy(p.phone, "13800000010"); addPatientNode(p);
    p.id = 11; strcpy(p.name, "刘涛"); p.age = 5; p.gender = 'M'; strcpy(p.phone, "13800000011"); addPatientNode(p);
    // 假设患者id=1张三，id=2李四已存在
    Patient* p1 = findPatientById(1);
    if (p1) {
        // 让张三住院
        Ward* ward = findWardById(1); // 普通病房
        for (int i = 0; i < ward->totalBeds; i++) {
            if (ward->beds[i] == 0) {
                ward->beds[i] = 1;
                ward->occupiedBeds++;
                p1->type = 1;
                p1->wardId = 1;
                p1->bedId = i + 1;
                p1->admitDate = getCurrentDate();
                p1->status = PATIENT_STATUS_IN_HOSPITAL;
                break;
            }
        }
        // 添加住院医疗记录
        MedicalRecord mr;
        mr.id = ++recordCount;
        mr.patientId = 1;
        mr.doctorId = 1;   // 主治医生ID，假设内科医生张建国ID=1
        mr.recordType = 4;
        mr.recordDate = getCurrentDate();
        sprintf(mr.detail, "办理住院，病房: %s，床位: %d号", ward->name, p1->bedId);
        mr.cost = 0;
        mr.status = 1;
        addMedicalRecordNode(mr);
        // 添加一条检查记录
        ExamRecord er = { ++examRecordCount, 1, 1, 1, getCurrentDate(), "双肺清晰", "正常", 350.0 };
        addExamRecordNode(er);
        updatePatientCostById(1, 350.0);
    }


}

// ==================== 保存各链表到文件 ====================
static void savePatients(void) {
    FILE* fp = fopen("patients.dat", "wb");
    if (!fp) return;
    fwrite(&patientCount, sizeof(int), 1, fp);
    PatientNode* p = patientHead;
    while (p) {
        fwrite(&p->data, sizeof(Patient), 1, fp);
        p = p->next;
    }
    fclose(fp);
}

static void saveDoctors(void) {
    FILE* fp = fopen("doctors.dat", "wb");
    if (!fp) return;
    fwrite(&doctorCount, sizeof(int), 1, fp);
    DoctorNode* d = doctorHead;
    while (d) {
        fwrite(&d->data, sizeof(Doctor), 1, fp);
        d = d->next;
    }
    fclose(fp);
}

static void saveDepartments(void) {
    FILE* fp = fopen("departments.dat", "wb");
    if (!fp) return;
    fwrite(&deptCount, sizeof(int), 1, fp);
    DeptNode* d = deptHead;
    while (d) {
        fwrite(&d->data, sizeof(Department), 1, fp);
        d = d->next;
    }
    fclose(fp);
}

static void saveWards(void) {
    FILE* fp = fopen("wards.dat", "wb");
    if (!fp) return;
    fwrite(&wardCount, sizeof(int), 1, fp);
    WardNode* w = wardHead;
    while (w) {
        fwrite(&w->data, sizeof(Ward), 1, fp);
        w = w->next;
    }
    fclose(fp);
}

static void saveMedicines(void) {
    FILE* fp = fopen("medicines.dat", "wb");
    if (!fp) return;
    fwrite(&medicineCount, sizeof(int), 1, fp);
    MedicineNode* m = medicineHead;
    while (m) {
        fwrite(&m->data, sizeof(Medicine), 1, fp);
        m = m->next;
    }
    fclose(fp);
}

static void saveMedicalRecords(void) {
    FILE* fp = fopen("records.dat", "wb");
    if (!fp) return;
    fwrite(&recordCount, sizeof(int), 1, fp);
    RecordNode* r = recordHead;
    while (r) {
        fwrite(&r->data, sizeof(MedicalRecord), 1, fp);
        r = r->next;
    }
    fclose(fp);
}

static void saveExamItems(void) {
    FILE* fp = fopen("examitems.dat", "wb");
    if (!fp) return;
    fwrite(&examItemCount, sizeof(int), 1, fp);
    ExamItemNode* e = examItemHead;
    while (e) {
        fwrite(&e->data, sizeof(ExamItem), 1, fp);
        e = e->next;
    }
    fclose(fp);
}

 void saveTestItems(void) {
    FILE* fp = fopen("testitems.dat", "wb");
    if (!fp) return;
    fwrite(&testItemCount, sizeof(int), 1, fp);
    TestItemNode* t = testItemHead;
    while (t) {
        fwrite(&t->data, sizeof(TestItem), 1, fp);
        t = t->next;
    }
    fclose(fp);
}

 void saveExamRecords(void) {
    FILE* fp = fopen("examrecords.dat", "wb");
    if (!fp) return;
    fwrite(&examRecordCount, sizeof(int), 1, fp);
    ExamRecordNode* p = examRecordHead;
    while (p) {
        fwrite(&p->data, sizeof(ExamRecord), 1, fp);
        p = p->next;
    }
    fclose(fp);
}

 void loadExamRecords(void) {
    FILE* fp = fopen("examrecords.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        ExamRecord temp;
        fread(&temp, sizeof(ExamRecord), 1, fp);
        addExamRecordNode(temp);
    }
    fclose(fp);
}

static void saveRegistrations(void) {
    FILE* fp = fopen("registrations.dat", "wb");
    if (!fp) return;
    fwrite(&regCount, sizeof(int), 1, fp);
    RegNode* r = regHead;
    while (r) {
        fwrite(&r->data, sizeof(Registration), 1, fp);
        r = r->next;
    }
    fclose(fp);
}

static void saveFollowUps(void) {
    FILE* fp = fopen("followups.dat", "wb");
    if (!fp) return;
    fwrite(&followUpCount, sizeof(int), 1, fp);
    FollowUpNode* f = followUpHead;
    while (f) {
        fwrite(&f->data, sizeof(FollowUpReminder), 1, fp);
        f = f->next;
    }
    fclose(fp);
}

static void saveDailyStats(void) {
    FILE* fp = fopen("dailystats.dat", "wb");
    if (!fp) return;
    fwrite(&dailyStatCount, sizeof(int), 1, fp);
    DailyStatNode* ds = dailyStatHead;
    while (ds) {
        fwrite(&ds->data, sizeof(DailyStatistics), 1, fp);
        ds = ds->next;
    }
    fclose(fp);
}

// ==================== 从文件加载各链表 ====================

static void loadPatients(void) {
    FILE* fp = fopen("patients.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        Patient temp;
        fread(&temp, sizeof(Patient), 1, fp);
        addPatientNode(temp);
    }
    fclose(fp);
}

static void loadDoctors(void) {
    FILE* fp = fopen("doctors.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        Doctor temp;
        fread(&temp, sizeof(Doctor), 1, fp);
        addDoctorNode(temp);
    }
    fclose(fp);
}

static void loadDepartments(void) {
    FILE* fp = fopen("departments.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        Department temp;
        fread(&temp, sizeof(Department), 1, fp);
        addDepartmentNode(temp);
    }
    fclose(fp);
}

static void loadWards(void) {
    FILE* fp = fopen("wards.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        Ward temp;
        fread(&temp, sizeof(Ward), 1, fp);
        addWardNode(temp);
    }
    fclose(fp);
}

static void loadMedicines(void) {
    FILE* fp = fopen("medicines.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        Medicine temp;
        fread(&temp, sizeof(Medicine), 1, fp);
        addMedicineNode(temp);
    }
    fclose(fp);
}

static void loadMedicalRecords(void) {
    FILE* fp = fopen("records.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        MedicalRecord temp;
        fread(&temp, sizeof(MedicalRecord), 1, fp);
        addMedicalRecordNode(temp);
    }
    fclose(fp);
}

static void loadExamItems(void) {
    FILE* fp = fopen("examitems.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        ExamItem temp;
        fread(&temp, sizeof(ExamItem), 1, fp);
        addExamItemNode(temp);
    }
    fclose(fp);
}

static void loadTestItems(void) {
    FILE* fp = fopen("testitems.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        TestItem temp;
        fread(&temp, sizeof(TestItem), 1, fp);
        addTestItemNode(temp);
    }
    fclose(fp);
}

 void freeExamRecords(void) {
    ExamRecordNode* p = examRecordHead;
    while (p) {
        ExamRecordNode* next = p->next;
        free(p);
        p = next;
    }
    examRecordHead = NULL;
    examRecordCount = 0;
}

 void freeTestRecords(void) {
    TestRecordNode* p = testRecordHead;
    while (p) {
        TestRecordNode* next = p->next;
        free(p);
        p = next;
    }
    testRecordHead = NULL;
    testRecordCount = 0;
}

static void loadRegistrations(void) {
    FILE* fp = fopen("registrations.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        Registration temp;
        fread(&temp, sizeof(Registration), 1, fp);
        addRegistrationNode(temp);
    }
    fclose(fp);
}

static void loadFollowUps(void) {
    FILE* fp = fopen("followups.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        FollowUpReminder temp;
        fread(&temp, sizeof(FollowUpReminder), 1, fp);
        addFollowUpNode(temp);
    }
    fclose(fp);
}

static void loadDailyStats(void) {
    FILE* fp = fopen("dailystats.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        DailyStatistics temp;
        fread(&temp, sizeof(DailyStatistics), 1, fp);
        addDailyStatNode(temp);
    }
    fclose(fp);
}

// ==================== 释放所有链表内存 ====================

static void freePatients(void) {
    PatientNode* p = patientHead;
    while (p) {
        PatientNode* next = p->next;
        free(p);
        p = next;
    }
    patientHead = NULL;
    patientCount = 0;
}

static void freeDoctors(void) {
    DoctorNode* d = doctorHead;
    while (d) {
        DoctorNode* next = d->next;
        free(d);
        d = next;
    }
    doctorHead = NULL;
    doctorCount = 0;
}

static void freeDepartments(void) {
    DeptNode* d = deptHead;
    while (d) {
        DeptNode* next = d->next;
        free(d);
        d = next;
    }
    deptHead = NULL;
    deptCount = 0;
}

static void freeWards(void) {
    WardNode* w = wardHead;
    while (w) {
        WardNode* next = w->next;
        free(w);
        w = next;
    }
    wardHead = NULL;
    wardCount = 0;
}

static void freeMedicines(void) {
    MedicineNode* m = medicineHead;
    while (m) {
        MedicineNode* next = m->next;
        free(m);
        m = next;
    }
    medicineHead = NULL;
    medicineCount = 0;
}

static void freeMedicalRecords(void) {
    RecordNode* r = recordHead;
    while (r) {
        RecordNode* next = r->next;
        free(r);
        r = next;
    }
    recordHead = NULL;
    recordCount = 0;
}

static void freeExamItems(void) {
    ExamItemNode* e = examItemHead;
    while (e) {
        ExamItemNode* next = e->next;
        free(e);
        e = next;
    }
    examItemHead = NULL;
    examItemCount = 0;
}

static void freeTestItems(void) {
    TestItemNode* t = testItemHead;
    while (t) {
        TestItemNode* next = t->next;
        free(t);
        t = next;
    }
    testItemHead = NULL;
    testItemCount = 0;
}


 void loadTestRecords(void) {
    FILE* fp = fopen("testrecords.dat", "rb");
    if (!fp) return;
    int count;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        TestRecord temp;
        fread(&temp, sizeof(TestRecord), 1, fp);
        addTestRecordNode(temp);
    }
    fclose(fp);
}

static void freeRegistrations(void) {
    RegNode* r = regHead;
    while (r) {
        RegNode* next = r->next;
        free(r);
        r = next;
    }
    regHead = NULL;
    regCount = 0;
}

static void freeFollowUps(void) {
    FollowUpNode* f = followUpHead;
    while (f) {
        FollowUpNode* next = f->next;
        free(f);
        f = next;
    }
    followUpHead = NULL;
    followUpCount = 0;
}

static void freeDailyStats(void) {
    DailyStatNode* ds = dailyStatHead;
    while (ds) {
        DailyStatNode* next = ds->next;
        free(ds);
        ds = next;
    }
    dailyStatHead = NULL;
    dailyStatCount = 0;
}
 void saveTestRecords(void) {
    FILE* fp = fopen("testrecords.dat", "wb");
    if (!fp) return;
    fwrite(&testRecordCount, sizeof(int), 1, fp);
    TestRecordNode* p = testRecordHead;
    while (p) {
        fwrite(&p->data, sizeof(TestRecord), 1, fp);
        p = p->next;
    }
    fclose(fp);
}

// ==================== 对外接口 ====================

void loadData(void) {
    // 尝试加载所有数据，如果某个文件不存在则跳过
    loadPatients();
    loadDoctors();
    loadDepartments();
    loadWards();
    loadMedicines();
    loadMedicalRecords();
    loadExamItems();
    loadTestItems();
    loadExamRecords();
    loadTestRecords();
    loadRegistrations();
    loadFollowUps();
    loadDailyStats();

    // 如果没有任何数据，则初始化示例数据
    if (patientCount == 0 && doctorCount == 0 && deptCount == 0) {
        initSampleData();
    }
}

void saveData(void) {
    savePatients();
    saveDoctors();
    saveDepartments();
    saveWards();
    saveMedicines();
    saveMedicalRecords();
    saveExamItems();
    saveTestItems();
    saveExamRecords();
    saveTestRecords();
    saveRegistrations();
    saveFollowUps();
    saveDailyStats();
}

void freeAllMemory(void) {
    freePatients();
    freeDoctors();
    freeDepartments();
    freeWards();
    freeMedicines();
    freeMedicalRecords();
    freeExamItems();
    freeTestItems();
    freeExamRecords();
    freeTestRecords();
    freeRegistrations();
    freeFollowUps();
    freeDailyStats();
}
