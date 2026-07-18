#include <stdio.h>
#include <time.h>
#include "data.h"
#include "utils.h"
#include "file.h"
#include "patient.h"
#include "doctor.h"
#include "admin.h"

int main() {
    system("chcp 936");
    // 设置控制台
    system("title 小医院医疗管理系统");
    system("mode con cols=100 lines=35");

    // 加载数据
    printf("正在加载数据...\n");
    loadData();
    // ---------- 自动保存相关变量（必须放在 do-while 之前）----------
    time_t last_save_time = time(NULL);
    const int SAVE_INTERVAL = 300;   // 5分钟 = 300秒
    // ------------------------------------------------------------

    int choice;
    do {
        // ---------- 自动保存检查 ----------
        if (difftime(time(NULL), last_save_time) >= SAVE_INTERVAL) {
            saveData();
            last_save_time = time(NULL);
            printf("\n[系统自动保存数据]\n");
            // 可选：短暂停顿，让用户看到提示
            // Sleep(1000);
        }
        // --------------------------------
        clearScreen();
        printf("\n");
        printf("╔══════════════════════════════════════════════════════════════╗\n");
        printf("║                小医院医疗管理系统                           ║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n");
        printf("\n");
        printf("  1. 患者登录\n");
        printf("  2. 医护登录\n");
        printf("  3. 管理员登录\n");
        printf("  4. 患者注册\n");
        printf("  0. 退出系统\n");
        printf("\n");
        printf("  请选择(输入数字0-4): ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: patientLogin(); break;
        case 2: doctorLogin(); break;
        case 3: adminLogin(); break;
        case 4: patientRegister(); break;
        case 0:
            printf("\n正在保存数据...\n");
            saveData();
            freeAllMemory();
            printf("感谢使用，再见！\n");
            break;
        default:
            printf("无效选择，请重新输入\n");
            pause();
        }

    } while (choice != 0);

    return 0;
}