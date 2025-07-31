#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define COL1      1
#define PAGE1     0xB0
#define PAGE2     0xB1
#define PAGE3     0xB2
#define PAGE4     0xB3
#define PAGE5     0xB4
#define PAGE6     0xB5
#define PAGE7     0xB6
#define PAGE8     0xB7

// ==============================
// 📦 1. Struct Declarations
// ==============================

typedef struct {
    int LTEStatus;
    int I2CStatus;
    int AWSStatus;
} ChipConfigStruct;

typedef struct {
  int StockStatus;
  int PStockStatus;
  int HeaterATemp;
  int HeaterBTemp;
  int HeaterAStatus;
  int HeaterBStatus;
} MachineReadingStruct;

typedef struct {
    int techConfig;
} AWSTopicsStruct;

typedef struct {
    int IncinBatchID;
    int IncinBurnNapkinTime;
} IncinDataStruct;

// ==============================
// 🧾 2. Global Struct Variables
// ==============================
extern MachineReadingStruct machineReadings;
extern ChipConfigStruct chipConfig;
extern AWSTopicsStruct awsTopic;

// ==============================
// 🔧 3. Technical Config Data
// ==============================
extern int sta, stb, ham, hbo, bct, bcc, hur, min;

// ==============================
// 🏢 4. Business Config Data
// ==============================
extern int iid, itp;
extern char qrb_data[1024];
extern uint8_t qrb_array[512];

// ==============================
// 🔥 5. Incinerator Config & Status
// ==============================
extern char IncinBatchID[50];
extern int Incin_cycle;
extern int IncinTriggerState;
extern int IncinTotalNapkinBurn;
extern char IncinStartTime[30];
extern char IncinEndTime[20];
extern int isIncinerationPaused;
extern bool IncinTriggeredByRestart;
extern bool IncinTriggerBySchedule;
extern bool IncinTriggerByResume;
extern bool IncinTriggerByInitial;

// ==============================
// ⏱️ 6. Time & Logging
// ==============================
extern char CurrentTimeString[22];
extern char CurrentTime[30];
extern time_t current_epoch;
extern int64_t incinTime;
extern int64_t insun_burn_time;

// ==============================
// 💡 7. Hardware/Peripheral Status
// ==============================
extern bool I2C_Connected;
extern bool I2cBusy;
extern bool i2c_state_change;
extern bool heaterA_status;
extern bool heaterB_status;

// ==============================
// 🌐 8. Network & Coin Status
// ==============================
extern bool NetworkStateChanged;
extern bool CoinStateChanged;
extern int coin_pulse;
extern bool PaymentScreenActive;

// ==============================
// ⚙️ 9. System State Flags
// ==============================
extern bool CustomCode;
extern bool technicalConfigFound;
extern bool businessConfigFound;
extern bool isMachineConfigFound;
extern bool ValueChanged;
extern bool already_triggered_today;

// ==============================
// 📦 10. Stock & Monitoring
// ==============================
extern int StockLevel;
extern int TBackNoStock;
extern int TBackLowStock;
extern bool UpdateStockToServer;

// ==============================
// 🌡️ 11. Chamber Sensors
// ==============================
extern int chamberA_temp;
extern int chamberA_disp;
extern int chamberB_temp;
extern int chamberB_disp;

// ==============================
// 🧾 12. Identity and Metadata
// ==============================
extern char MAC_ID[10];
extern char MERCHANT_ID[15];
extern char MERCH_KEY[5];
extern char VersionNumber[6];

// ==============================
// 📁 13. FTP Configuration
// ==============================
extern char FTP_PATH[128];
extern char FTP_USER[64];
extern char FTP_PASS[64];
extern char FTP_IP[64];

// ==============================
// 📝 14. Utilities
// ==============================
extern char json_line[1024];
extern bool napkinOfflineLogDataState;
extern bool incineratorOfflineLogDataState;

extern bool NeedToCheckInsunTime;
extern bool NeededDefaultScreen;
extern bool NeededInsuinScreen;
extern bool InterruptForDispense;
extern bool FotaUpdate;
extern bool IsInstulationOperation;

extern bool PublishTrigger;
extern int PublishCode;
extern int PublishCodeA;
extern int PublishCodeB;

extern int64_t lcd_last_display_time;
extern int64_t lcd_insun_last_display_time;

#endif // GLOBALS_H
